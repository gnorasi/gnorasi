/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#ifdef VRN_MODULE_PYTHON
// Must come first!
#include "modules/python/pythonmodule.h"
#endif

#include "voreenmainwindow.h"

#include "tgt/gpucapabilities.h"
#include "tgt/filesystem.h"
#include "tgt/qt/qtcanvas.h"

#include "voreen/core/processors/processor.h"
#include "voreen/core/network/processornetwork.h"
#include "voreen/core/processors/processorfactory.h"

#include "voreen/core/network/workspace.h"
#include "voreen/core/network/networkevaluator.h"
#include "voreen/core/processors/canvasrenderer.h"

#include "voreen/core/datastructures/volume/volumecontainer.h"

#include "voreen/qt/aboutbox.h"
#include "voreen/qt/helpbrowser.h"

#include "voreen/qt/widgets/consoleplugin.h"
#include "voreen/qt/widgets/inputmappingdialog.h"
#include "voreen/qt/widgets/propertystatewidget.h"
#include "voreen/qt/widgets/rendertargetviewer.h"
#include "voreen/qt/widgets/snapshotwidget.h"
#include "voreen/qt/widgets/volumecontainerwidget.h"
#include "settingsdialog.h"
#include "voreen/qt/widgets/voreentoolwindow.h"
#include "voreen/qt/widgets/animation/animationeditor.h"
#include "voreen/qt/widgets/processor/canvasrendererwidget.h"
#include "voreen/qt/widgets/performancerecordwidget.h"
#include "voreen/qt/widgets/processorlistwidget.h"
#include "voreen/qt/widgets/propertylistwidget.h"
#include "networkeditor/networkeditor.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/qt/voreenapplicationqt.h"
#include "voreenveapplication.h"
#include "voreenveplugin.h"
#include "voreenmoduleve.h"
#include "startupscreen.h"
#include "voreen/core/version.h"
#include "voreen/core/utils/stringconversion.h"

#include <QDesktopServices>
#include <QLabel>
#include <QPropertyAnimation>

namespace voreen {

namespace {

const int MAX_RECENT_FILES = 10;

// Version number of restoring state of the main window.
// Increase when incompatible changes happen.
const int WINDOW_STATE_VERSION = 14;  // V2.6.1

} // namespace

////////// VoreenSplashScreen //////////////////////////////////////////////////////////

VoreenSplashScreen::VoreenSplashScreen()
    : QWidget(0, Qt::WindowStaysOnTopHint | Qt::ToolTip)
{
    setWindowModality(Qt::ApplicationModal);

    QPixmap pm = QPixmap(":/voreenve/image/splash.png");
    qreal screenWidth = QApplication::desktop()->screenGeometry().width();
    qreal screenHeight = QApplication::desktop()->screenGeometry().height();

    move(screenWidth / 2.f - pm.size().width() / 2.f, screenHeight / 2.f - pm.size().height());
    layout_ = new QVBoxLayout(this);
    layout_->setMargin(0);
    layout_->setSpacing(0);

    pixmap_ = new QLabel;
    pixmap_->setPixmap(pm);
    layout_->addWidget(pixmap_);
    
    QString version = QString::fromStdString("<font color='white'>Version " + VoreenVersion::getVersion() + "</font>");
    version_ = new QLabel(version, this);
    version_->move(21, 85);

    message_ = new QLabel(this);
    message_->move(5, 255);
}

void VoreenSplashScreen::showMessage(const QString& message) {
    message_->setText("<font color='white'>" + message + "</font>");
}

QPoint VoreenSplashScreen::getPixmapPosition() const {
    return mapToGlobal(pixmap_->pos());
}

////////// VoreenMdiSubWindow //////////////////////////////////////////////////////////

VoreenMdiSubWindow::VoreenMdiSubWindow(QWidget* widget, QWidget* parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    setWidget(widget);
    setAttribute(Qt::WA_DeleteOnClose, false);
}

// Adapted from QWidget::saveGeometry()
QByteArray VoreenMdiSubWindow::saveGeometry() const {
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    const quint32 magicNumber = 0x1234FFFF;
    quint16 majorVersion = 1;
    quint16 minorVersion = 0;
    stream << magicNumber
           << majorVersion
           << minorVersion
           << frameGeometry()
           << normalGeometry()
           << quint8(windowState() & Qt::WindowMaximized);

    return array;
}

// Adapted from QWidget::restoreGeometry(). Ignores multiple screen handling as this introduces
// problems for MDI windows (see #65).
bool VoreenMdiSubWindow::restoreGeometry(const QByteArray& geometry) {
    if (geometry.size() < 4)
        return false;
    QDataStream stream(geometry);
    stream.setVersion(QDataStream::Qt_4_0);

    const quint32 magicNumber = 0x1234FFFF;
    quint32 storedMagicNumber;
    stream >> storedMagicNumber;
    if (storedMagicNumber != magicNumber)
        return false;

    const quint16 currentMajorVersion = 1;
    quint16 majorVersion = 0;
    quint16 minorVersion = 0;

    stream >> majorVersion >> minorVersion;

    if (majorVersion != currentMajorVersion)
        return false;
    // (Allow all minor versions.)

    QRect restoredFrameGeometry;
    QRect restoredNormalGeometry;
    quint8 maximized;

    stream >> restoredFrameGeometry
           >> restoredNormalGeometry
           >> maximized;

    const int frameHeight = 20;
    if (!restoredFrameGeometry.isValid())
        restoredFrameGeometry = QRect(QPoint(0,0), sizeHint());

    if (!restoredNormalGeometry.isValid())
        restoredNormalGeometry = QRect(QPoint(0, frameHeight), sizeHint());

    if (maximized) {
        // set geometry before setting the window state to make
        // sure the window is maximized to the right screen.
        setGeometry(restoredNormalGeometry);
        Qt::WindowStates ws = windowState();
        if (maximized)
            ws |= Qt::WindowMaximized;
        setWindowState(ws);
    } else {
        QPoint offset;
        setWindowState(windowState() & ~(Qt::WindowMaximized | Qt::WindowFullScreen));
        move(restoredFrameGeometry.topLeft() + offset);
        resize(restoredNormalGeometry.size());
    }
    return true;
}

void VoreenMdiSubWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    showMinimized();
}

////////// VoreenMainWindow ////////////////////////////////////////////////////////////

namespace {

// Add glass highlight effect to standard menubar
class FancyMenuBar : public QMenuBar {
protected:
    void paintEvent(QPaintEvent* event) {
        QMenuBar::paintEvent(event);

        // draw semi-transparent glass highlight over upper half of menubar
        QPainter painter(this);
        painter.setBrush(QColor(255, 255, 255, 76));
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, rect().width(), rect().height() / 2);
    }
};

} // namespace

const std::string VoreenMainWindow::loggerCat_("voreenve.VoreenMainWindow");

VoreenMainWindow::VoreenMainWindow(const std::string& workspace, bool resetSettings)
    : QMainWindow()
    , networkEditorWindow_(0)
    , networkEditorWidget_(0)
    , processorListWidget_(0)
    , propertyListWidget_(0)
    , volumeContainerWidget_(0)
    , consolePlugin_(0)
    , inputMappingDialog_(0)
    , animationEditor_(0)
    , renderTargetViewer_(0)
    , propertyStateWidget_(0)
    , performanceRecordWidget_(0)
    , snapshotWidget_(0)
    , guiMode_(MODE_NONE)
    , canvasPos_(0, 0)
    , canvasSize_(0, 0)
    , errorMessageDialog_(0)
{

    setDockOptions(QMainWindow::AnimatedDocks); // disallow tabbed docks

    // initialize the console early so it gets all the interesting messages
    consolePlugin_ = new ConsolePlugin(this, VoreenApplication::app()->getLogLevel());

    // retrieve VoreenVE plugins from application/modules
    VoreenVEApplication* veApp = VoreenVEApplication::veApp();
    if (!veApp) {
        LERROR("VoreenVEApplication not instantiated");
    }
    else {
        std::vector<std::string> pluginNames;
        for (size_t i=0; i<veApp->getVEModules().size(); i++) {
            VoreenModuleVE* veModule = veApp->getVEModules().at(i);
            const std::vector<VoreenVEPlugin*>& modPlugins = veModule->getVoreenVEPlugins(); 
            for (size_t j=0; j<modPlugins.size(); j++) {
                plugins_.push_back(modPlugins.at(j));
                pluginNames.push_back(modPlugins.at(j)->getName());
            }
        }
        LINFO("VoreenVE plugins: " << strJoin(pluginNames, ", "));
    }

    // if we have a stylesheet we want the fancy menu bar, please
    if (!qApp->styleSheet().isEmpty())
        setMenuBar(new FancyMenuBar());

    // clear session settings (window states, paths, ...), if specified by cmd line parameter
    if (resetSettings) {
        settings_.clear();
        LWARNING("Restored session settings");
    }
    else { 
        loadSettings();
    }
        
    if (!workspace.empty())
        currentWorkspace_ = workspace.c_str();

    setMinimumSize(600, 400);
    setWindowIcon(QIcon(":/voreenve/icons/voreen-logo_64x64.png"));
    setAcceptDrops(true);

    // show tooltips (for icons in toolbar) even if the window is inactive, as often the case
    // when a canvas window is active
    setAttribute(Qt::WA_AlwaysShowToolTips);

    ignoreWindowTitleModified_ = true;
    updateWindowTitle();
}

VoreenMainWindow::~VoreenMainWindow() {
    VoreenApplication::app()->deinitialize();
}

void VoreenMainWindow::initialize(VoreenSplashScreen* splash, bool showStartup) {
    if (splash)
        splash->showMessage("Initializing OpenGL...");

    // initGL requires a valid OpenGL context
    sharedContext_ = new tgt::QtCanvas("Init Canvas", tgt::ivec2(32, 32), tgt::GLCanvas::RGBADD, this, true);
    sharedContext_->init(); //necessary?

    // initialize OpenGL
    try {
        VoreenApplication::app()->initializeGL();
    }
    catch(VoreenException& e) {
        if (tgt::LogManager::isInited())
            LFATALC("voreenve.MainWindow", "OpenGL initialization failed: " << e.what());
        else
            std::cerr << "OpenGL initialization failed: " << e.what();

        if (splash)
            splash->close();
        qApp->processEvents();
        QMessageBox::critical(this, tr("Initialization Error"), tr("OpenGL initialization failed. Quit."));

        exit(EXIT_FAILURE);
    }

    sharedContext_->hide();

    // some hardware/driver checks
    if (!GpuCaps.isOpenGlVersionSupported(tgt::GpuCapabilities::GlVersion::TGT_GL_VERSION_2_0)) {
        if (splash)
            splash->close();
        qApp->processEvents();
        std::ostringstream glVersion;
        glVersion << GpuCaps.getGlVersion();
        QMessageBox::critical(this, tr("Incompatible OpenGL Version"),
                              tr("Voreen requires OpenGL version 2.0 or higher, which does not seem be "
                                 "supported on this system (reported version: %1). Therefore, the application "
                                 "will most likely not work properly.").arg(glVersion.str().c_str()));
        qApp->processEvents();
    }
    // deactivated on intel until we have a reliable detection. (stefan)
    else if ( !GpuCaps.isShaderModelSupported(tgt::GpuCapabilities::SHADER_MODEL_3) && (GpuCaps.getVendor() != tgt::GpuCapabilities::GPU_VENDOR_INTEL) ) {
        if (splash)
            splash->close();
        qApp->processEvents();
        QMessageBox::critical(this, tr("Incompatible Shader Model"),
                              tr("Voreen requires Shader Model 3 or higher, which does not seem be "
                                 "supported on this system. Therefore, the application will most likely not "
                                 "work properly."));
        qApp->processEvents();
    }
    else if (!GpuCaps.areFramebufferObjectsSupported()) {
        if (splash)
            splash->close();
        qApp->processEvents();
        QMessageBox::critical(this, tr("Framebuffer Objects Missing"),
                              tr("Voreen uses OpenGL framebuffer objects, which do not seem be supported "
                                 "on this system. Therefore, the application will most likely not work properly."));
        qApp->processEvents();
    }

    if (splash)
        splash->showMessage("Creating visualization...");

    // create visualization object
    vis_ = new VoreenVisualization(sharedContext_);

    if (splash)
        splash->showMessage("Creating GUI...");

    // mdi area
    mdiArea_ = new QMdiArea(this);
    mdiArea_->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);
    setCentralWidget(mdiArea_);

    // put network editor in mdi area
    networkEditorWidget_ = new NetworkEditor(this, vis_->getWorkspace()->getProcessorNetwork(), vis_->getEvaluator());
    networkEditorWidget_->setWindowTitle(tr("Processor Network"));
    networkEditorWindow_ = new VoreenMdiSubWindow(networkEditorWidget_, this, Qt::FramelessWindowHint);
    networkEditorWindow_->setWindowState(networkEditorWindow_->windowState() | Qt::WindowFullScreen);
    networkEditorWindow_->hide(); // hide initially to prevent flicker
    mdiArea_->addSubWindow(networkEditorWindow_);
    vis_->setNetworkEditorWidget(networkEditorWidget_);
    qApp->processEvents();

    // create tool windows now, after everything is initialized, but before window settings are restored
    createMenus();
    createToolBars();

    // create built-in tools and add plugins
    createToolWindows();
    addVEPlugins();
    qApp->processEvents();

    // signals indicating a change in network
    connect(vis_, SIGNAL(networkModified(ProcessorNetwork*)), this, SLOT(adjustSnapshotMenu()));
    connect(vis_, SIGNAL(modified()), this, SLOT(updateWindowTitle()));
    connect(vis_, SIGNAL(newNetwork(ProcessorNetwork*)), this, SLOT(adjustSnapshotMenu()));

    loadWindowSettings();

    setGuiMode(guiMode_);

    if (splash) {
        splash->showMessage("Loading workspace...");
        qApp->processEvents();
    }

    GpuCaps.setCurrentAvailableTextureMem(settings_.value("maximumGPUMemory", QVariant(0)).toInt());
    GpuCaps.setRetrieveAvailableTextureMem(settings_.value("constraintGPUMemoryEnabled", QVariant(true)).toBool());

    //
    // now the GUI is complete
    //
    if (!currentWorkspace_.isEmpty()) {
        // load workspace passed as program parameter
        openWorkspace(currentWorkspace_);
    }
    else if (!lastWorkspace_.isEmpty() && startupWorkspace_) {
        // load last workspace, but only if loading has been successful last time
        openWorkspace(lastWorkspace_);
    }
    else {
        // load an initial workspace
        //openWorkspace(VoreenApplication::app()->getWorkspacePath("templates/standard.vws").c_str());
    }
    startupComplete("workspace");

    if (showStartup && settings_.value("showStartupScreen", QVariant(true)).toBool()) {
        StartupScreen* startupWidget = createStartupScreen(splash);
        startupWidget->show();
        qApp->processEvents();
    }
    
    if (splash)
        splash->close();
}

void VoreenMainWindow::deinitialize() {
    // save widget settings first
    saveSettings();

    // render target viewer is an OpenGL widget,
    // so destruct before OpenGL deinitialization
    delete renderTargetViewer_;
    renderTargetViewer_ = 0;

    // deinitialize plugins (deleting not necessary, since done by the Qt parent)
    for (size_t i=0; i<plugins_.size(); i++) {
        VoreenVEPlugin* plugin = plugins_.at(i);
        if (plugin->isInitialized()) {
            try {
                LINFO("Deinitializing VoreenVE plugin '" << plugin->getName() << "'");
                plugin->deinitialize();
                plugin->initialized_ = false;        
            }
            catch (tgt::Exception& e) {
                LERROR("Failed to deinitialize VoreenVE plugin '" << plugin->getName() << "': " << e.what());
            }
        }
    }

    // free workspace, unregister network/volumecontainer from widgets
    delete vis_;
    vis_ = 0;

    // finalize OpenGL
    VoreenApplication::app()->deinitializeGL();
    delete sharedContext_;
    sharedContext_ = 0;
}

////////// GUI setup ///////////////////////////////////////////////////////////////////

void VoreenMainWindow::createMenus() {
    menu_ = menuBar();

    //
    // File menu
    //
    fileMenu_ = menu_->addMenu(tr("&File"));

    // Workspace
    workspaceNewAction_ = new QAction(QIcon(":/voreenve/icons/clear.png"), tr("&New Workspace"),  this);
    workspaceNewAction_->setShortcut(QKeySequence::New);
    connect(workspaceNewAction_, SIGNAL(triggered()), this, SLOT(newWorkspace()));
    fileMenu_->addAction(workspaceNewAction_);

    workspaceOpenAction_ = new QAction(QIcon(":/voreenve/icons/open.png"), tr("&Open Workspace..."),  this);
    workspaceOpenAction_->setShortcut(QKeySequence::Open);
    connect(workspaceOpenAction_, SIGNAL(triggered()), this, SLOT(openWorkspace()));
    fileMenu_->addAction(workspaceOpenAction_);

    workspaceSaveAction_ = new QAction(QIcon(":/voreenve/icons/save.png"), tr("&Save Workspace"),  this);
    workspaceSaveAction_->setShortcut(QKeySequence::Save);
    connect(workspaceSaveAction_, SIGNAL(triggered()), this, SLOT(saveWorkspace()));
    fileMenu_->addAction(workspaceSaveAction_);

    workspaceSaveAsAction_ = new QAction(QIcon(":/voreenve/icons/saveas.png"), tr("Save Workspace &As..."),  this);
    connect(workspaceSaveAsAction_, SIGNAL(triggered()), this, SLOT(saveWorkspaceAs()));
    fileMenu_->addAction(workspaceSaveAsAction_);

    workspaceSaveCopyAsAction_ = new QAction(tr("Save &Copy As..."), this);
    connect(workspaceSaveCopyAsAction_, SIGNAL(triggered()), this, SLOT(saveCopyAs()));
    fileMenu_->addAction(workspaceSaveCopyAsAction_);

    fileMenu_->addSeparator();

    // Volumes

    openDatasetAction_ = new QAction(QIcon(":/voreenve/icons/open-volume.png"), tr("Load Vo&lume..."), this);
    openDatasetAction_->setStatusTip(tr("Load a volume data set"));
    openDatasetAction_->setShortcut(tr("Ctrl+L"));
    connect(openDatasetAction_, SIGNAL(triggered()), this, SLOT(openDataset()));
    //gnorasi fileMenu_->addAction(openDatasetAction_);

    //gnorasi fileMenu_->addSeparator();

#ifdef VRN_MODULE_ZIP
    // ZIP-Workspaces
    workspaceExtractAction_ = new QAction(QIcon(":/voreenve/icons/extract-workspace.png"),
                                          tr("&Extract Workspace Archive..."), this);
    connect(workspaceExtractAction_, SIGNAL(triggered()), this, SLOT(extractWorkspaceArchive()));
    fileMenu_->addAction(workspaceExtractAction_);
#endif // VRN_MODULE_ZIP

    // Network
    importNetworkAction_ = new QAction(QIcon(":/voreenve/icons/import.png"), tr("&Import Network..."), this);
    connect(importNetworkAction_, SIGNAL(triggered()), this, SLOT(openNetwork()));
    fileMenu_->addAction(importNetworkAction_);

    exportNetworkAction_ = new QAction(QIcon(":/voreenve/icons/export.png"), tr("E&xport Network..."), this);
    connect(exportNetworkAction_, SIGNAL(triggered()), this, SLOT(saveNetworkAs()));
    fileMenu_->addAction(exportNetworkAction_);

    fileMenu_->addSeparator();

    quitAction_ = new QAction(QIcon(":/voreenve/icons/exit.png"), tr("&Quit"), this);
    quitAction_->setShortcut(tr("Ctrl+Q"));
    quitAction_->setStatusTip(tr("Exit the application"));
    quitAction_->setToolTip(tr("Exit the application"));
    connect(quitAction_, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu_->addAction(quitAction_);

    fileMenu_->addSeparator();

    // Recent files
    for (int i = 0; i < MAX_RECENT_FILES; i++) {
        recentFileActs_.append(new QAction(this));
        connect(recentFileActs_[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
        fileMenu_->addAction(recentFileActs_[i]);
    }
    updateRecentFiles();

    //
    // View menu
    //
    viewMenu_ = menu_->addMenu(tr("&View"));

    modeDevelopmentAction_ = new QAction(QIcon(":/voreenve/icons/development-mode.png"),
                                     tr("&Development Mode"), this);
    modeDevelopmentAction_->setCheckable(true);
    modeDevelopmentAction_->setShortcut(tr("F3"));

    modeApplicationAction_ = new QAction(QIcon(":/voreenve/icons/visualization-mode.png"),
                                           tr("&Application Mode"), this);
    modeApplicationAction_->setCheckable(true);
    modeApplicationAction_->setShortcut(tr("F4"));

    QActionGroup* guiModeGroup = new QActionGroup(this);
    guiModeGroup->addAction(modeApplicationAction_);
    guiModeGroup->addAction(modeDevelopmentAction_);
    modeApplicationAction_->setChecked(true);
    connect(guiModeGroup, SIGNAL(triggered(QAction*)), this, SLOT(guiModeChanged()));

    viewMenu_->addAction(modeDevelopmentAction_);
    viewMenu_->addAction(modeApplicationAction_);
    viewMenu_->addSeparator();

    //
    // Tools menu
    //
    toolsMenu_ = menu_->addMenu(tr("&Tools"));

    //
    // Action menu
    //
    /* gnorasi
    actionMenu_ = menu_->addMenu(tr("&Action"));
    rebuildShadersAction_ = new QAction(QIcon(":/voreenve/icons/rebuild.png"), tr("&Rebuild Shaders"),  this);
    rebuildShadersAction_->setShortcut(tr("F5"));
    rebuildShadersAction_->setStatusTip(tr("Rebuilds all currently used shaders"));
    rebuildShadersAction_->setToolTip(tr("Rebuild all shaders"));
    connect(rebuildShadersAction_, SIGNAL(triggered()), this, SLOT(rebuildShaders()));
    actionMenu_->addAction(rebuildShadersAction_);

    enterWhatsThisAction_ = QWhatsThis::createAction();
    actionMenu_->addAction(enterWhatsThisAction_);
    */
    //
    // Options menu
    //
    /* gnorasi
    optionsMenu_ = menu_->addMenu(tr("&Options"));
    showStartupScreen_ = new QAction(tr("Show startup widget"), this);
    connect(showStartupScreen_, SIGNAL(triggered()), this, SLOT(showStartupScreen()));
    if (optionsMenu_)
        optionsMenu_->addAction(showStartupScreen_);

    // settings editor
    settingsEditor_ = new SettingsDialog(this);
    settingsAction_ = new QAction(QIcon(":/icons/saveas.png"), tr("&Settings"), this);
    connect(settingsAction_, SIGNAL(triggered()), this, SLOT(optionsSettings()));
    if (optionsMenu_)
        optionsMenu_->addAction(settingsAction_);
    */
    //
    // Help menu
    //
    /* gnorasi
    helpMenu_ = menu_->addMenu(tr("&Help"));

    helpFirstStepsAct_ = new QAction(QIcon(":/voreenve/icons/help.png"), tr("&Getting Started Guide..."), this);
    helpFirstStepsAct_->setShortcut(tr("F1"));
    connect(helpFirstStepsAct_, SIGNAL(triggered()), this, SLOT(helpFirstSteps()));
    helpMenu_->addAction(helpFirstStepsAct_);

    helpTutorialSlidesAct_ = new QAction(QIcon(":/voreenve/icons/pdf.png"), tr("&Tutorial Slides..."), this);
    connect(helpTutorialSlidesAct_, SIGNAL(triggered()), this, SLOT(helpTutorialSlides()));
    helpMenu_->addAction(helpTutorialSlidesAct_);

    helpAnimationAct_ = new QAction(QIcon(":/voreenve/icons/help.png"), tr("&Animation Manual..."), this);
    connect(helpAnimationAct_, SIGNAL(triggered()), this, SLOT(helpAnimation()));
    helpMenu_->addAction(helpAnimationAct_);

    helpMenu_->addSeparator();

    // Add some web links to the menu. Use the redirects (in the "go" directory) to be
    // independent of website reorganization.
    QAction* websiteAct = new QAction(tr("Voreen Website..."), this);
    websiteAct->setData(tr("http://www.voreen.org/go/start"));
    connect(websiteAct, SIGNAL(triggered()), this, SLOT(helpWebsite()));
    helpMenu_->addAction(websiteAct);

    QAction* videosAct = new QAction(tr("Video Tutorials..."), this);
    videosAct->setData(tr("http://www.voreen.org/go/video-tutorials"));
    connect(videosAct, SIGNAL(triggered()), this, SLOT(helpWebsite()));
    helpMenu_->addAction(videosAct);

    QAction* faqAct = new QAction(tr("FAQ..."), this);
    faqAct->setData(tr("http://www.voreen.org/go/faq"));
    connect(faqAct, SIGNAL(triggered()), this, SLOT(helpWebsite()));
    helpMenu_->addAction(faqAct);

    QAction* mailAct = new QAction(tr("Mailing List..."), this);
    mailAct->setData(tr("http://www.voreen.org/go/mailinglist"));
    connect(mailAct, SIGNAL(triggered()), this, SLOT(helpWebsite()));
    helpMenu_->addAction(mailAct);

    helpMenu_->addSeparator();

    aboutAction_ = new QAction(QIcon(":/voreenve/icons/about.png"), tr("&About VoreenVE..."), this);
    connect(aboutAction_, SIGNAL(triggered()), this, SLOT(helpAbout()));
    helpMenu_->addAction(aboutAction_);
    */
}

void VoreenMainWindow::createToolBars() {
#ifdef __APPLE__
    const QSize iconSize = QSize(23,23);
#endif

    // file toolbar
    fileToolBar_ = addToolBar(tr("File"));
#ifdef __APPLE__
    fileToolBar_->setIconSize(iconSize);
#endif
    fileToolBar_->setObjectName("file-toolbar");
    fileToolBar_->addAction(workspaceNewAction_);
    fileToolBar_->addAction(workspaceOpenAction_);
    fileToolBar_->addAction(workspaceSaveAction_);
    //gnorasi fileToolBar_->addAction(openDatasetAction_);

    // view toolbar
    viewToolBar_ = addToolBar(tr("View"));
#ifdef __APPLE__
    viewToolBar_->setIconSize(iconSize);
#endif
    viewToolBar_->setObjectName("view-toolbar");
    QLabel* label = new QLabel(tr("   View  "));
    label->setObjectName("toolBarLabel");
    viewToolBar_->addWidget(label);

    viewToolBar_->addAction(modeDevelopmentAction_);
    viewToolBar_->addAction(modeApplicationAction_);
    viewToolBar_->addSeparator();

    // tools toolbar
    toolsToolBar_ = addToolBar(tr("Tools"));
#ifdef __APPLE__
    toolsToolBar_->setIconSize(iconSize);
#endif
    toolsToolBar_->setObjectName("tools-toolbar");
    label = new QLabel(tr("   Tools "));
    label->setObjectName("toolBarLabel");
    toolsToolBar_->addWidget(label);

    // action toolbar
    /* gnorasi
    actionToolBar_ = addToolBar(tr("Action"));
#ifdef __APPLE__
    actionToolBar_->setIconSize(iconSize);
#endif

    actionToolBar_->setObjectName("action-toolbar");
    label = new QLabel(tr("   Action "));
    label->setObjectName("toolBarLabel");
    actionToolBar_->addWidget(label);
    actionToolBar_->addAction(rebuildShadersAction_);
    */
#ifdef __APPLE__ // we are on a mac system
    // HACK (Workaround) for Qt Mac Bug, makes MainWindow reappear
    // see for details:
    // http://bugreports.qt.nokia.com/browse/QTBUG-5069?page=com.atlassian.jira.plugin.system.issuetabpanels%3Aall-tabpanel
    show();
#endif

}

VoreenToolWindow* VoreenMainWindow::addToolWindow(QAction* action, QWidget* widget, const QString& name,
                                                      Qt::DockWidgetArea dockarea, Qt::DockWidgetAreas allowedAreas,
                                                      bool basic)
{
    action->setCheckable(true);
    action->setChecked(false);

    bool dockable = (allowedAreas != Qt::NoDockWidgetArea);

    VoreenToolWindow* window = new VoreenToolWindow(action, this, widget, name, dockable);

    if (basic) {
        if (viewMenu_)
            viewMenu_->addAction(action);
        if (viewToolBar_)
            viewToolBar_->addAction(action);
    }
    else {
        if (toolsMenu_)
            toolsMenu_->addAction(action);
        if (toolsToolBar_)
            toolsToolBar_->addAction(action);
    }

    window->setAllowedAreas(allowedAreas);
    if (dockable) {
        if (dockarea == Qt::NoDockWidgetArea) {
            addDockWidget(Qt::LeftDockWidgetArea, window);
            window->setFloating(true);
        }
        else {
            addDockWidget(dockarea, window);
        }
    }

    toolWindows_ << window;

    return window;
}

VoreenToolWindow* VoreenMainWindow::getToolWindow(QWidget* childWidget) const {
    if (!childWidget)
        return 0;
    foreach(VoreenToolWindow* toolWindow, toolWindows_) {
        if (toolWindow->child() == childWidget)
            return toolWindow;
    }
    return 0;
}

void VoreenMainWindow::createToolWindows() {
    // processor list
    processorListWidget_ = new ProcessorListWidget(this);
    processorListWidget_->setMinimumSize(200, 200);
    QAction* processorListAction = new QAction(QIcon(":/voreenve/icons/show-processors.png"), tr("&Processors"), this);
    addToolWindow(processorListAction, processorListWidget_, "ProcessorList",
        Qt::LeftDockWidgetArea, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    vis_->setProcessorListWidget(processorListWidget_);

    // property list
    propertyListWidget_ = new PropertyListWidget(this, 0);
    addToolWindow(new QAction(QIcon(":/voreenve/icons/show-properties.png"), tr("P&roperties"), this),
        propertyListWidget_, "Properties", Qt::RightDockWidgetArea, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    vis_->setPropertyListWidget(propertyListWidget_);

    // VolumeContainerWidget
    /* gnorasi
    volumeContainerWidget_ = new VolumeContainerWidget(vis_->getVolumeContainer(), this);
    addToolWindow(new QAction(QIcon(":/voreenve/icons/show-volumecontainer.png"), tr("V&olumes"), this),
                      volumeContainerWidget_, "VolumeContainer", Qt::RightDockWidgetArea);
    vis_->setVolumeContainerWidget(volumeContainerWidget_);
    */

    // console (note: has been created in constructor!)
    QAction* consoleAction = new QAction(QIcon(":/voreenve/icons/show-console.png"), tr("&Debug Console"), this);
    consoleAction->setShortcut(tr("Ctrl+D"));
    VoreenToolWindow* consoleTool = addToolWindow(consoleAction, consolePlugin_, "Console", Qt::BottomDockWidgetArea, Qt::BottomDockWidgetArea);
    consoleTool->setMinimumHeight(100);

    // input mapping
    /* gnorasi
    inputMappingDialog_ =  new InputMappingDialog(this, vis_->getWorkspace()->getProcessorNetwork());
    QAction* inputMappingAction = new QAction(QIcon(":/voreenve/icons/show-keymapping.png"), tr("&Input Mapping"), this);
    inputMappingAction->setShortcut(tr("Ctrl+I"));
    VoreenToolWindow* inputWindow = addToolWindow(inputMappingAction, inputMappingDialog_, tr("Show shortcut preferences"), 
        Qt::NoDockWidgetArea, Qt::AllDockWidgetAreas, true);
    inputWindow->resize(520, 400);
    vis_->setInputMappingDialog(inputMappingDialog_);
    */

    // render target debug window
    renderTargetViewer_ = new RenderTargetViewer(sharedContext_);
    renderTargetViewer_->setEvaluator(vis_->getEvaluator());
    renderTargetViewer_->setMinimumSize(200, 200);
    QAction* texContainerAction = new QAction(QIcon(":/voreenve/icons/show-rendertargets.png"),tr("Render &Target Viewer"), this);
    texContainerAction->setShortcut(tr("Ctrl+T"));
    VoreenToolWindow* tc = addToolWindow(texContainerAction, renderTargetViewer_, "RenderTargetViewer",
        Qt::NoDockWidgetArea, Qt::NoDockWidgetArea);
    tc->widget()->setContentsMargins(0,0,0,0);
    tc->widget()->layout()->setContentsMargins(0,0,0,0);
    tc->resize(500, 500);
    vis_->setRenderTargetViewer(renderTargetViewer_);

    // property state
    /* gnorasi
    propertyStateWidget_ = new PropertyStateWidget(vis_->getWorkspace()->getProcessorNetwork(), this);
    connect(propertyStateWidget_, SIGNAL(modifiedNetwork()), vis_, SLOT(setModified()));
    QAction* propertyStateAction = new QAction(QIcon(":/voreenve/icons/alacarte.png"), tr("Property State History"), this);
    VoreenToolWindow* propertyStateWindow = addToolWindow(propertyStateAction, propertyStateWidget_, tr("Property State"),
        Qt::NoDockWidgetArea, (Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea), false);
    propertyStateWindow->resize(350, 400);
    */

    // performance records
    /* gnorasi
    performanceRecordWidget_ = new PerformanceRecordWidget(this);
    performanceRecordWidget_->setEvaluator(vis_->getEvaluator());
    QAction* performanceRecordAction = new QAction(QIcon(":/voreenve/icons/performance.png"), tr("&Performance Record"), this);
    performanceRecordAction->setCheckable(true);
    VoreenToolWindow* performanceRecordWindow = addToolWindow(performanceRecordAction, performanceRecordWidget_, 
        tr("Performance Record"), Qt::NoDockWidgetArea, Qt::AllDockWidgetAreas, false);
    performanceRecordWindow->resize(350, 400);
    vis_->setPerformanceRecordWidget(performanceRecordWidget_);
    */

    // animation editor
    /* gnorasi
    animationEditor_ = new AnimationEditor(vis_->getEvaluator(), vis_->getWorkspace(), this);
    VoreenToolWindow* animationWindow = addToolWindow(new QAction(QIcon(":/icons/video.png"), tr("&Animation"), this), animationEditor_, "Animation",
                      Qt::NoDockWidgetArea, Qt::BottomDockWidgetArea, false);
    animationWindow->resize(925, 400);
    */
    // snapshot widget
#ifdef VRN_MODULE_DEVIL
    snapshotWidget_ = new SnapshotWidget(this);
    QAction* snapshotAction = new QAction(QIcon(":/voreenve/icons/make-snapshot.png"), tr("Snapshot"), this);
    VoreenToolWindow* snapshotTool = addToolWindow(snapshotAction, snapshotWidget_, "Snapshot", Qt::NoDockWidgetArea, Qt::AllDockWidgetAreas, false);
    snapshotTool->resize(400, 250);
#endif

    // create connections between tool widgets
    vis_->createConnections();
}

void VoreenMainWindow::addVEPlugins() {
    /// Initialize all VoreenVEPlugins and add them to the main window
    for (size_t i=0; i<plugins_.size(); i++) {
        VoreenVEPlugin* plugin = plugins_.at(i);

        QAction* action = new QAction(plugin->getIcon(), QString::fromStdString(plugin->getName()), this);
        VoreenToolWindow* pluginWindow = addToolWindow(action, plugin, QString::fromStdString(plugin->getName()), 
            plugin->getInitialDockWidgetArea(), plugin->getAllowedDockWidgetAreas(), false);

        plugin->setParentWindow(pluginWindow); 
        plugin->setMainWindow(this);
        plugin->setNetworkEvaluator(vis_->getEvaluator());
        plugin->createWidgets();

        try {
            LINFO("Initializing VoreenVE plugin '" << plugin->getName() << "'");
            plugin->initialize();
            plugin->initialized_ = true;
        }
        catch (tgt::Exception& e) {
            LERROR("Failed to initialize VoreenVE plugin '" << plugin->getName() << "': " << e.what());
            toolWindows_.removeOne(pluginWindow);
            delete pluginWindow;
        }
    }
}

////////// settings ////////////////////////////////////////////////////////////////////

void VoreenMainWindow::loadSettings() {
    // set defaults
    networkPath_ = VoreenApplication::app()->getNetworkPath().c_str();
    workspacePath_ = VoreenApplication::app()->getWorkspacePath().c_str();
    QSize windowSize = QSize(0, 0);
    QPoint windowPosition = QPoint(0, 0);
    bool windowMaximized = true;

    // restore settings
    settings_.beginGroup("MainWindow");
    windowSize = settings_.value("size", windowSize).toSize();
    windowPosition = settings_.value("pos", windowPosition).toPoint();
    windowMaximized = settings_.value("maximized", windowMaximized).toBool();
    lastWorkspace_ = settings_.value("workspace", "").toString();
    applicationModeState_ = settings_.value("visualizationModeState").toByteArray();
    developmentModeState_ = settings_.value("networkModeState").toByteArray();
    networkEditorWindowState_ = settings_.value("networkEditorWindowState").toByteArray();
    settings_.endGroup();

    settings_.beginGroup("Paths");
    networkPath_ = settings_.value("network", networkPath_).toString();
    workspacePath_ = settings_.value("workspace", workspacePath_).toString();
    settings_.endGroup();

    settings_.beginGroup("Startup");
    // load last startup workspace
    startupWorkspace_ = settings_.value("workspace", true).toBool();
    // set default values for the current startup
    settings_.setValue("workspace", false);
    settings_.endGroup();

    if (windowSize.isNull()) {
        windowMaximized = true;
    } else {
        resize(windowSize);
    }

    // ensure that the main window is restored on a visible screen
    // particular, when switching between different multi desktops modes
    QRect screenGeometry = QApplication::desktop()->screen()->geometry();

    // modify screen geometry to account maximized windows having negative position
    screenGeometry.setRect(screenGeometry.x() - 10, screenGeometry.y() - 10,
                           screenGeometry.width() + 20, screenGeometry.height() + 20);

    if (screenGeometry.contains(windowPosition) &&
        screenGeometry.contains(QPoint(windowPosition.x()+windowSize.width(),
                                       windowPosition.y()+windowSize.height())))
    {
        move(windowPosition);
    }

    if (windowMaximized)
        setWindowState(windowState() | Qt::WindowMaximized);
}

void VoreenMainWindow::loadWindowSettings() {
    // Restore visibility, position and size of tool windows from settings
    for (int i=0; i < toolWindows_.size(); ++i) {
        if (!toolWindows_[i]->objectName().isEmpty()) {
            toolWindows_[i]->setVisible(false);
        }
    }

    settings_.beginGroup("Windows");
    for (int i=0; i < toolWindows_.size(); ++i) {
        if (!toolWindows_[i]->objectName().isEmpty()) {
            settings_.beginGroup(toolWindows_[i]->objectName());
            if (settings_.contains("size"))
                toolWindows_[i]->resize(settings_.value("size").toSize());

            // Ignore position (0, 0) for invisible windows as otherwise all previously
            // invisible windows would be placed at (0, 0) after restarting the application.
            if (settings_.contains("pos") &&
                (settings_.value("pos").toPoint() != QPoint(0, 0) || settings_.value("visible").toBool()))
            {
                toolWindows_[i]->move(settings_.value("pos").toPoint());
            }

            if (settings_.contains("visible"))
                toolWindows_[i]->setVisible(settings_.value("visible").toBool());
            settings_.endGroup();
        }
    }
    settings_.endGroup();

    settings_.beginGroup("MainWindow");
    bool applicationMode = settings_.value("visualizationMode").toBool();
    settings_.endGroup();

    guiMode_ = applicationMode ? MODE_APPLICATION : MODE_DEVELOPMENT;
}

void VoreenMainWindow::saveSettings() {
    // write version number of the config file format (might be useful someday)
    settings_.setValue("ConfigVersion", 1);

    if (guiMode_ == MODE_APPLICATION) {
        applicationModeState_ = saveState(WINDOW_STATE_VERSION);
    }
    else if (guiMode_ == MODE_DEVELOPMENT) {
        developmentModeState_ = saveState(WINDOW_STATE_VERSION);
        networkEditorWindowState_ = networkEditorWindow_->saveGeometry();
    }

    settings_.beginGroup("MainWindow");
    settings_.setValue("size", size());
    settings_.setValue("pos", pos());
    settings_.setValue("maximized", (windowState() & Qt::WindowMaximized) != 0);
    settings_.setValue("workspace", lastWorkspace_);
    settings_.setValue("visualizationModeState", applicationModeState_);
    settings_.setValue("networkModeState", developmentModeState_);
    settings_.setValue("networkEditorWindowState", networkEditorWindowState_);
    settings_.setValue("visualizationMode", (guiMode_ == MODE_APPLICATION));
    settings_.endGroup();

    settings_.beginGroup("Paths");
    settings_.setValue("network", networkPath_);
    settings_.setValue("workspace", workspacePath_);
    settings_.endGroup();

    settings_.beginGroup("Windows");

    for (int i=0; i < toolWindows_.size(); ++i) {
        if (!toolWindows_[i]->objectName().isEmpty()) {
            settings_.beginGroup(toolWindows_[i]->objectName());
            settings_.setValue("visible", toolWindows_[i]->isVisible());
            settings_.setValue("pos", toolWindows_[i]->pos());
            settings_.setValue("size", toolWindows_[i]->size());
            settings_.endGroup();
        }
    }
    settings_.endGroup();
}

void VoreenMainWindow::startupComplete(const std::string& phaseName) {
    settings_.beginGroup("Startup");
    settings_.setValue(phaseName.c_str(), true);
    settings_.endGroup();
}

////////// loading / saving ////////////////////////////////////////////////////////////

void VoreenMainWindow::openNetwork() {
    if (!askSave())
        return;

    QFileDialog fileDialog(this, tr("Import Network..."),
                           QDir(networkPath_).absolutePath(),
                           "Voreen network files (*.vnw)");
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getNetworkPath().c_str());
#ifndef VRN_DEPLOYMENT
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getModulePath().c_str());
#endif
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    fileDialog.setSidebarUrls(urls);

    if (fileDialog.exec()) {
        openNetwork(fileDialog.selectedFiles().at(0));
        networkPath_ = fileDialog.directory().path();
    }
}

void VoreenMainWindow::openNetwork(const QString& filename) {
    try {
        ignoreWindowTitleModified_ = true;
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        vis_->importNetwork(filename);
        showNetworkErrors();
        currentNetwork_ = filename;
        addToRecentFiles(currentNetwork_);
        QApplication::restoreOverrideCursor();
        adaptWidgetsToNetwork();
    }
    catch (SerializationException& e) {
        QApplication::restoreOverrideCursor();

        // Trying to serialize Data of old format?
        if ((typeid(e) == typeid(XmlSerializationFormatException))
            && std::string(e.what()).find("ProcessorNetwork") != std::string::npos)
        {
            QMessageBox::critical(this, tr("VoreenVE"),
                                  tr("The network file you are trying to load uses an old file format incompatible "
                                     "with this version of Voreen.\n"
                                     "Please use an older version of Voreen to save it as a workspace and then "
                                     "use the 'workspaceconverter' application to convert it to the new file format."));
        }
        else {
            errorMessageDialog_ = new QErrorMessage(this);
            errorMessageDialog_->showMessage(e.what());
        }
    }
    ignoreWindowTitleModified_ = false;
    updateWindowTitle();
}

bool VoreenMainWindow::saveNetworkAs() {
    QFileDialog fileDialog(this, tr("Export Network..."), QDir(networkPath_).absolutePath(),
                           tr("Voreen network files (*.vnw)"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setConfirmOverwrite(true);
    fileDialog.setDefaultSuffix("vnw");

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getNetworkPath().c_str());
#ifndef VRN_DEPLOYMENT
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getModulePath().c_str());
#endif
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    fileDialog.setSidebarUrls(urls);

    if (fileDialog.exec()) {
        currentNetwork_ = fileDialog.selectedFiles().at(0);

        try {
            vis_->exportNetwork(currentNetwork_);
        }
        catch (SerializationException& e) {
            QApplication::restoreOverrideCursor();
            QErrorMessage* errorMessageDialog = new QErrorMessage(this);
            errorMessageDialog->showMessage(e.what());
            return false;
        }

        networkPath_ = fileDialog.directory().path();
        vis_->setModified(false);
        updateWindowTitle();
        addToRecentFiles(currentNetwork_);
        return true;
    }
    return false;
}

bool VoreenMainWindow::askSave() {
    if (vis_ && vis_->isModified()) {
        switch (QMessageBox::question(this, tr("Modified Workspace"), tr("Save the current workspace?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes))
        {
        case QMessageBox::Yes:
            return saveWorkspace();
        case QMessageBox::No:
            return true;
        default:
            return false;
        }
    }
    return true;
}

void VoreenMainWindow::exportWorkspace() {
   QFileDialog fileDialog(this, tr("Export Workspace..."), QDir(workspacePath_).absolutePath(),
                           tr("Voreen workspace archives (*.zip)"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setConfirmOverwrite(true);
    fileDialog.setDefaultSuffix("zip");

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getWorkspacePath().c_str());
#ifndef VRN_DEPLOYMENT
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getModulePath().c_str());
#endif
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getDataPath().c_str());
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    fileDialog.setSidebarUrls(urls);

    if (fileDialog.exec() == false)
        return;

    try {
        vis_->exportWorkspaceToZipArchive(fileDialog.selectedFiles().first());
    }
    catch (SerializationException& e) {
        QApplication::restoreOverrideCursor();
        QErrorMessage* errorMessageDialog = new QErrorMessage(this);
        errorMessageDialog->showMessage(e.what());
        return;
    }
}

void VoreenMainWindow::extractWorkspaceArchive() {

    if (!askSave())
        return;

    // query user for workspace archive to extract
    QFileDialog fileDialog(this, tr("Extract Workspace Archive..."),
        QDir(workspacePath_).absolutePath(), "Voreen workspace archives (*.zip)");

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getWorkspacePath().c_str());
#ifndef VRN_DEPLOYMENT
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getModulePath().c_str());
#endif
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getDataPath().c_str());
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    fileDialog.setSidebarUrls(urls);

    // pass queried filename to overloaded function
    if (fileDialog.exec())
        extractWorkspaceArchive(fileDialog.selectedFiles().first());

}

void VoreenMainWindow::extractWorkspaceArchive(QString archivFile) {

    if (archivFile.isEmpty()) {
        LERROR("Passed archive filename is empty");
        return;
    }

    // extract archive file
    QString workspaceFile = "";
    QString workspaceDir = "";
    QString archiveName = QFileInfo(archivFile).baseName();
    QDir archivePath = QFileInfo(archivFile).absoluteDir();
    workspaceDir = QFileDialog::getExistingDirectory(this, tr("Extract Workspace Archive To..."),
        archivePath.path(), (QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));

    if (!workspaceDir.isEmpty()) {
        // create subdirectory with name of archive in target directory
        QDir dir(workspaceDir);
        dir.mkdir(archiveName);
        workspaceDir += "/" + archiveName;
        if (!dir.exists(archiveName)) {
           QMessageBox::critical(this, tr("Failure"),
                tr("Failed to create directory:\n") + workspaceDir);
            return;
        }

        // extract workspace to created subdirectory
        try {
            workspaceFile = vis_->extractWorkspaceArchive(archivFile, workspaceDir, true);
        }
        catch (SerializationException& e) {
            QMessageBox::critical(this, tr("Failed to Extract Workspace Archive"),
                    QString::fromStdString(e.what()));
        }
    }

    // open extracted workspace
    if (!workspaceFile.isEmpty()) {
        if (QMessageBox::question(this, "Open Extracted Workspace",
            "Workspace archive '" + QString::fromStdString(tgt::FileSystem::fileName(archivFile.toStdString())) + "' " +
            "has been extracted to:\n" + workspaceDir + "\n\n" +
            "Do you want to open the extracted workspace now?",
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
            QMessageBox::Yes) == QMessageBox::Yes) {
                openWorkspace(workspaceFile);
        }
    }
}

void VoreenMainWindow::newWorkspace() {
    if (!askSave())
        return;

    vis_->newWorkspace();

    currentWorkspace_ = "";
    currentNetwork_ = "";
    lastWorkspace_ = currentWorkspace_;

    if (propertyListWidget_)
        propertyListWidget_->clear();
    if (renderTargetViewer_)
        renderTargetViewer_->update();

    vis_->setModified(false);
    updateWindowTitle();
    if (animationEditor_)
        animationEditor_->setWorkspace(vis_->getWorkspace());

    for (size_t i=0; i<plugins_.size(); i++)
        if (plugins_[i]->isInitialized())
            plugins_[i]->setWorkspace(vis_->getWorkspace());
}

void VoreenMainWindow::openWorkspace(const QString& filename) {

    // zip workspaces have to be extracted
    if (filename.endsWith(".zip", Qt::CaseInsensitive)) {
        if (QMessageBox::question(this, "Extract Workspace Archive", "Extract workspace archive to directory?",
            QMessageBox::StandardButtons(QMessageBox::Ok | QMessageBox::Cancel), QMessageBox::Ok)
            == QMessageBox::Ok) {
                extractWorkspaceArchive(filename);
        }
        return;
    }

    // disable render target widget during load
    if (renderTargetViewer_)
        renderTargetViewer_->setEvaluator(0);

    if (errorMessageDialog_) {
        delete errorMessageDialog_;
        errorMessageDialog_ = 0;
    }

    // open vws workspace
    try {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        qApp->processEvents();
        vis_->newWorkspace();
        qApp->processEvents();
        vis_->openWorkspace(filename);
    }
    catch (SerializationException& e) {
        vis_->newWorkspace();
        setUpdatesEnabled(true);
        currentWorkspace_ = "";
        lastWorkspace_ = currentWorkspace_;
        QApplication::restoreOverrideCursor();

        QErrorMessage* errorMessageDialog = new QErrorMessage(this);
        errorMessageDialog->showMessage(tr("Could not open workspace:\n") + e.what());
        return;
    }

    ignoreWindowTitleModified_ = false;
    updateWindowTitle();

    // adjust canvas widgets (created during workspace load) to application mode
    if (guiMode_ == MODE_APPLICATION) {
        adjustCanvasWidgets(MODE_APPLICATION);
        setGuiMode(MODE_APPLICATION);
    }

    showWorkspaceErrors();
    showNetworkErrors();

    adaptWidgetsToNetwork();
    currentWorkspace_ = filename;
    lastWorkspace_ = currentWorkspace_;
    currentNetwork_ = "";
    addToRecentFiles(currentWorkspace_);
    vis_->setModified(false);
    ignoreWindowTitleModified_ = false;
    updateWindowTitle();
    if (renderTargetViewer_)
        renderTargetViewer_->setEvaluator(vis_->getEvaluator());
    QApplication::restoreOverrideCursor();

    if (animationEditor_)
        animationEditor_->setWorkspace(vis_->getWorkspace());

    for (size_t i=0; i<plugins_.size(); i++)
        plugins_.at(i)->setWorkspace(vis_->getWorkspace());

}

void VoreenMainWindow::openWorkspace() {
    if (!askSave())
        return;

    QFileDialog fileDialog(this, tr("Open Workspace..."),
                           QDir(workspacePath_).absolutePath());

    QStringList filters;
    filters << "Voreen workspaces (*.vws)";
#ifdef VRN_MODULE_ZIP
    filters << "Voreen workspace archives (*.zip)";
#endif
    fileDialog.setNameFilters(filters);

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getWorkspacePath().c_str());
#ifndef VRN_DEPLOYMENT
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getModulePath().c_str());
#endif
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getDataPath().c_str());
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    fileDialog.setSidebarUrls(urls);

    if (fileDialog.exec()) {
        openWorkspace(fileDialog.selectedFiles().at(0));
        workspacePath_ = fileDialog.directory().path();
    }
}

bool VoreenMainWindow::saveWorkspace(const QString& filename) {
    QString f = filename;
    if (f.isEmpty() && !vis_->readOnlyWorkspace())
        f = currentWorkspace_;

    if (f.isEmpty())
        return saveWorkspaceAs();

    try {
        vis_->blockSignals(true);
        vis_->saveWorkspace(f);
        vis_->blockSignals(false);
        if (f.endsWith(".zip", Qt::CaseInsensitive)) {
            // do not take a zip workspace as current workspace, since it is just an export
            // inform user about successful export instead
            QMessageBox::information(this, "Workspace Export",
                "Exported workspace to archive:\n" + f);
        }
        else {
            currentWorkspace_ = f;
            lastWorkspace_ = currentWorkspace_;
            vis_->setModified(false);
            updateWindowTitle();
            addToRecentFiles(currentWorkspace_);
        }
    }
    catch (SerializationException& e) {
        vis_->blockSignals(false);
        QApplication::restoreOverrideCursor();
        QErrorMessage* errorMessageDialog = new QErrorMessage(this);
        errorMessageDialog->setWindowTitle(tr("Workspace Serialization Failed"));
        errorMessageDialog->showMessage(e.what());
        return false;
    }

    return true;
}

bool VoreenMainWindow::saveWorkspaceAs() {
    QFileDialog fileDialog(this, tr("Save Workspace As..."), QDir(workspacePath_).absolutePath());
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setConfirmOverwrite(true);

    QStringList filters;
    filters << "Voreen workspaces (*.vws)";
#ifdef VRN_MODULE_ZIP
    filters << "Voreen workspace archives (*.zip)";
#endif
    fileDialog.setNameFilters(filters);
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getWorkspacePath().c_str());
#ifndef VRN_DEPLOYMENT
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getModulePath().c_str());
#endif
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getDataPath().c_str());
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    fileDialog.setSidebarUrls(urls);

    if (fileDialog.exec()) {
        bool result;
        QString name = fileDialog.selectedFiles().at(0);
        if (fileDialog.selectedNameFilter() == "Voreen workspace archives (*.zip)") {
            if (!name.endsWith(".zip"))
                result = saveWorkspace(name + ".zip");
            else
                result = saveWorkspace(name);
        }
        else if (!name.endsWith(".vws"))
            result = saveWorkspace(fileDialog.selectedFiles().at(0) + ".vws");
        else
            result = saveWorkspace(fileDialog.selectedFiles().at(0));
        workspacePath_ = fileDialog.directory().path();
        return result;
    }
    else {
        return false;
    }
}

bool VoreenMainWindow::saveCopyAs() {
    QString current = currentWorkspace_;
    bool success = saveWorkspaceAs();
    if (success) {
        currentWorkspace_ = current;
        vis_->setModified(true);
        updateWindowTitle();
    }
    return success;
}

void VoreenMainWindow::openDataset() {
    if (!volumeContainerWidget_) {
        LWARNING("No volume container widget");
        return;
    }
    volumeContainerWidget_->loadVolume();
}

void VoreenMainWindow::openRecentFile() {
    if (!askSave())
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString file(action->data().toString());
        openWorkspace(file);
    }
}

void VoreenMainWindow::addToRecentFiles(const QString& filename) {
    QString f = filename;
    f.replace("\\", "/");
    QStringList files = settings_.value("recentFileList").toStringList();
    files.removeAll("");        // delete empty entries
    files.removeAll(f);
    files.prepend(f);
    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();

    settings_.setValue("recentFileList", files);
    updateRecentFiles();
}

void VoreenMainWindow::updateRecentFiles() {
    QStringList files = settings_.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), MAX_RECENT_FILES);
    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        recentFileActs_[i]->setText(text);
        recentFileActs_[i]->setData(files[i]);
        recentFileActs_[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MAX_RECENT_FILES; ++j)
        recentFileActs_[j]->setVisible(false);
}

////////// network /////////////////////////////////////////////////////////////////////

void VoreenMainWindow::showNetworkErrors() {
    // alert about errors in the Network
    std::vector<std::string> errors = vis_->getNetworkErrors();
    if (!errors.empty()) {
        QString msg;
        for (size_t i=0; i < errors.size(); i++) {
            msg += "<li>" + QString(errors[i].c_str()) + "</li>\n";
            LWARNING(errors[i]);
        }

        errorMessageDialog_ = new QErrorMessage(this);
        errorMessageDialog_->resize(600, 300);
        errorMessageDialog_->setWindowTitle(tr("Network Deserialization"));
        errorMessageDialog_->showMessage(tr("There were <b>%1 errors</b> loading the network:\n<ul>").arg(errors.size())
                                        + msg + "\n</ul>");

        qApp->processEvents();
    }
}

void VoreenMainWindow::showWorkspaceErrors() {
    // alert about errors in the Network
    std::vector<std::string> errors = vis_->getWorkspaceErrors();
    if (!errors.empty()) {
        QString msg;
        for (size_t i=0; i < errors.size(); i++) {
            msg += "<li>" + QString(errors[i].c_str()) + "</li>\n";
            LWARNING(errors[i]);
        }

        errorMessageDialog_ = new QErrorMessage(this);
        errorMessageDialog_->resize(600, 300);
        errorMessageDialog_->setWindowTitle(tr("Workspace Deserialization"));
        errorMessageDialog_->showMessage(tr("There were <b>%1 errors</b> loading the workspace %2:\n<ul>").arg(
            errors.size()).arg(QString::fromStdString(vis_->getWorkspace() ? vis_->getWorkspace()->getFilename() : ""))
            + msg + "\n</ul>");

        qApp->processEvents();
    }
}

void VoreenMainWindow::adaptWidgetsToNetwork() {
    if (propertyStateWidget_) {
        propertyStateWidget_->setProcessorNetwork(vis_->getWorkspace()->getProcessorNetwork());
    }
}

////////// actions /////////////////////////////////////////////////////////////////////

//
// General
//

void VoreenMainWindow::closeEvent(QCloseEvent *event) {
    if (vis_->isModified()) {
        if (askSave()) {
            event->accept();
            emit closeMainWindow();
        } else {
            event->ignore();
            return;
        }
    }

    deinitialize();
}

void VoreenMainWindow::dragEnterEvent(QDragEnterEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();

    if (!urls.empty() && urls.first().toLocalFile().endsWith(".vnw"))
        event->acceptProposedAction();
}

void VoreenMainWindow::dropEvent(QDropEvent* event) {
    openNetwork(event->mimeData()->urls().first().toLocalFile());
}

//
// Action menu
//

void VoreenMainWindow::runScript(const QString& filename) {
#ifdef VRN_MODULE_PYTHON

    if (!PythonModule::getInstance()) {
        LERROR("PythonModule not instantiated");
        return;
    }

    if (!PythonModule::getInstance()->isInitialized()) {
        LERROR("PythonModule not initialized");
        return;
    }

    PythonScript* script = PythonModule::getInstance()->load(filename.toStdString(), false);
    if (!script) {
        LERROR("Failed to load Python script '" << filename.toStdString() << "'");
        //QMessageBox::warning(this, tr("Python Error"), tr("Python script '%1' could not be loaded.").arg(filename));
        return;
    }

    if (script->compile()) {
        LINFO("Running Python script '" << filename.toStdString() << "' ...");
        if (script->run())
            LINFO("Python script finished");
        else
            LERROR("Python runtime error:\n" << script->getLog());
    }
    else {
        LERROR("Python compile error:\n" << script->getLog());
    }
    PythonModule::getInstance()->dispose(script);
#else
    QMessageBox::warning(this, "Voreen", tr("Voreen has been compiled without Python support, "
                                            "can not run '%1'").arg(filename));
#endif // VRN_MODULE_PYTHON
}

void VoreenMainWindow::rebuildShaders() {
    // set to a waiting cursor
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if (vis_->rebuildShaders()) {
        LINFO("Shaders reloaded");
        #ifdef WIN32
        Beep(100, 100);
        #endif
    }
    else {
        LWARNING("Shader reloading failed");
        #ifdef WIN32
        Beep(10000, 100);
        #endif
        QApplication::restoreOverrideCursor();
        //consoleTool_->show();
        qApp->processEvents();
        QMessageBox::critical(this, tr("Shader Reloading"),
                              tr("Shader reloading failed.\n"
                                 "See the Debug Console for details."));
    }
    QApplication::restoreOverrideCursor();
}

//
// Help menu
//

void VoreenMainWindow::optionsSettings() {
    SettingsDialog settings(this);
    settings.exec();
}

//
// Help menu
//

void VoreenMainWindow::helpFirstSteps() {
    QString path(VoreenApplication::app()->getDocumentationPath("gettingstarted/gsg.html").c_str());
    HelpBrowser* help = new HelpBrowser(QUrl::fromLocalFile(path), tr("VoreenVE Help"), this);
    help->resize(1050, 700);
    help->show();
    connect(this, SIGNAL(closeMainWindow()), help, SLOT(close()));
}

void VoreenMainWindow::helpTutorialSlides() {
    QString path(VoreenApplication::app()->getDocumentationPath("vis2010-tutorial-slides.pdf").c_str());
    QDesktopServices::openUrl(QUrl(QString::fromStdString("file:///") + path, QUrl::TolerantMode));
}

void VoreenMainWindow::helpAnimation() {
    QString path(VoreenApplication::app()->getDocumentationPath("animation/animation.html").c_str());
    HelpBrowser* help = new HelpBrowser(QUrl::fromLocalFile(path), tr("VoreenVE Animation Manual"), this);
    help->resize(1050, 700);
    help->show();
    connect(this, SIGNAL(closeMainWindow()), help, SLOT(close()));
}

void VoreenMainWindow::helpWebsite() {
    // URL is stored in data portion of the sender
    if (QAction* act = dynamic_cast<QAction*>(sender()))
        QDesktopServices::openUrl(QUrl(act->data().toString()));
}

void VoreenMainWindow::helpAbout() {
    AboutBox about("VoreenVE", tr("VoreenVE Visualization Environment"), this);
    about.exec();
}

////////// further functions ///////////////////////////////////////////////////////////

void VoreenMainWindow::changeEvent(QEvent* event) {
    // Filter out window title changes which were done outside setWindowTitle (non-virtual) of
    // this class. This is used to prevent MDI windows from adding their title to the main
    // window title when maximized.
    if (event->type() == QEvent::WindowTitleChange) {
        if (windowTitle() != originalWindowTitle_)
            setWindowTitle(originalWindowTitle_);
    }
}
void VoreenMainWindow::setWindowTitle(const QString& title) {
    originalWindowTitle_ = title;
    QMainWindow::setWindowTitle(title);
}

void VoreenMainWindow::updateWindowTitle() {
    QString title = tr("VoreenVE");

    if (!ignoreWindowTitleModified_ && vis_ && vis_->isModified())
        title += " *";

    if (!currentNetwork_.isEmpty() || !currentWorkspace_.isEmpty() ) {
        QFileInfo f(currentWorkspace_.isEmpty() ? currentNetwork_ : currentWorkspace_); // get filename without path
        title += " - " + f.fileName();
    }

    setWindowTitle(title);
}

void VoreenMainWindow::guiModeChanged() {
    if (modeApplicationAction_->isChecked() && guiMode_ != MODE_APPLICATION)
        setGuiMode(MODE_APPLICATION);
    else if (modeDevelopmentAction_->isChecked() && guiMode_ != MODE_DEVELOPMENT)
        setGuiMode(MODE_DEVELOPMENT);
}

void VoreenMainWindow::setGuiMode(GuiMode guiMode) {
    if (guiMode == MODE_APPLICATION) {
        if (guiMode_ == MODE_DEVELOPMENT) {
            developmentModeState_ = saveState(WINDOW_STATE_VERSION);
            networkEditorWindowState_ = networkEditorWindow_->saveGeometry();
        }

        setUpdatesEnabled(false);

        // hide all first to prevent some flicker
        modeApplicationAction_->setChecked(true);
        networkEditorWindow_->hide();
        networkEditorWidget_->setVisible(false);
        if (getToolWindow(processorListWidget_))
            getToolWindow(processorListWidget_)->hide();

        if (!restoreState(applicationModeState_, WINDOW_STATE_VERSION)) {
            if (getToolWindow(processorListWidget_) && getToolWindow(processorListWidget_)->isEnabled()) {
                if (getToolWindow(propertyListWidget_))
                    getToolWindow(propertyListWidget_)->show();
            }
            if (getToolWindow(volumeContainerWidget_) && getToolWindow(volumeContainerWidget_)->isEnabled())
                getToolWindow(volumeContainerWidget_)->show();
            if (getToolWindow(consolePlugin_))
                getToolWindow(consolePlugin_)->hide();
        }
        setUpdatesEnabled(true);

        // resize canvas after gui has been settled
        adjustCanvasWidgets(MODE_APPLICATION);
    }
    else if (guiMode == MODE_DEVELOPMENT) {
        if (guiMode_ == MODE_APPLICATION)
            applicationModeState_ = saveState(WINDOW_STATE_VERSION);

        // first update canvas widget
        adjustCanvasWidgets(MODE_DEVELOPMENT);

        if (!restoreState(developmentModeState_, WINDOW_STATE_VERSION)) {
            if (getToolWindow(processorListWidget_))
                getToolWindow(processorListWidget_)->show();
            if (getToolWindow(propertyListWidget_))
                getToolWindow(propertyListWidget_)->show();
            if (getToolWindow(volumeContainerWidget_))
                getToolWindow(volumeContainerWidget_)->show();
            if (getToolWindow(consolePlugin_))
                getToolWindow(consolePlugin_)->show();
        }

        setUpdatesEnabled(false);

        if (networkEditorWindow_->restoreGeometry(networkEditorWindowState_))
            networkEditorWindow_->show();
        else
             networkEditorWindow_->showMaximized();

        networkEditorWidget_->setVisible(true); // only show now, so it immediately gets the correct size
        modeDevelopmentAction_->setChecked(true);

        setUpdatesEnabled(true);
    }

    setUpdatesEnabled(false);

    // adjust property list widget at last, since this is quite expensive and may flicker
    if (propertyListWidget_) {
        if (guiMode == MODE_APPLICATION)
            propertyListWidget_->setState(PropertyListWidget::LIST, Property::USER);
        else
            propertyListWidget_->setState(PropertyListWidget::SELECTED, Property::DEVELOPER);
    }

    setUpdatesEnabled(true);
    guiMode_ = guiMode;
}

void VoreenMainWindow::adjustCanvasWidgets(GuiMode guiMode) {
    // adjust canvas widgets
    if (vis_->getWorkspace() && vis_->getWorkspace()->getProcessorNetwork()) {
        const ProcessorNetwork* network = vis_->getWorkspace()->getProcessorNetwork();
        const std::vector<CanvasRenderer*>& processors = network->getProcessorsByType<CanvasRenderer>();

        for (size_t i=0; i < processors.size(); ++i) {
            ProcessorWidget* pw = processors[i]->getProcessorWidget();
            if (pw) {
                QProcessorWidget* qpw = dynamic_cast<QProcessorWidget*>(pw);
                if (qpw) {
                    if (guiMode == MODE_APPLICATION) {
                        canvasPos_ = qpw->getPosition();
                        canvasSize_ = qpw->getSize();
                        QMdiSubWindow* subw = mdiArea_->addSubWindow(qpw);
                        subw->showMaximized();
                    }
                    else {
                        QObject* par = qpw->parent();
                        qpw->setParent(this);
                        if (dynamic_cast<QMdiSubWindow*>(par)) {
                            mdiArea_->removeSubWindow(qpw);
                            delete par;
                        }

                        qpw->setParent(this);
                        qpw->setWindowFlags(Qt::Tool);

                        static_cast<QWidget*>(qpw)->setVisible(true);
                        if (canvasSize_.x > 0 && canvasSize_.y > 0) {
                            qpw->setSize(canvasSize_.x,  canvasSize_.y);
                            qpw->setPosition(canvasPos_.x, canvasPos_.y);
                        }
                        qpw->show();
                    }
                    return;
                }
            }
        }
    }
}

void VoreenMainWindow::adjustSnapshotMenu() {
#ifdef VRN_MODULE_DEVIL
    std::vector<CanvasRenderer*> canvasRenderers
        = vis_->getWorkspace()->getProcessorNetwork()->getProcessorsByType<CanvasRenderer>();

    std::vector<SnapshotElement*> pluginRenderer;
    pluginRenderer.push_back(networkEditorWidget_->getSnapshotPlugin());

    snapshotWidget_->setCanvasRenderer(canvasRenderers);
    snapshotWidget_->setPluginRenderer(pluginRenderer);
#endif
}

void VoreenMainWindow::showStartupScreen() {
    StartupScreen* startupWidget = createStartupScreen();
    startupWidget->show();
}

StartupScreen* VoreenMainWindow::createStartupScreen(VoreenSplashScreen* splash) {
    StartupScreen* startupWidget = new StartupScreen(this);
    if (splash) {
        QPoint splashPos = splash->getPixmapPosition();
        startupWidget->move(splashPos);
    }
    else {
        qreal screenWidth = QApplication::desktop()->screenGeometry().width();
        qreal screenHeight = QApplication::desktop()->screenGeometry().height();
        startupWidget->move(screenWidth / 2.f - startupWidget->size().width() / 2.f, screenHeight / 2.f - startupWidget->size().height());
    }
    connect(startupWidget, SIGNAL(selectedRecentWorkspace(const QString&)), this, SLOT(openWorkspace(const QString&)));
    connect(startupWidget, SIGNAL(selectedNewWorkspace()), this, SLOT(newWorkspace()));
    connect(startupWidget, SIGNAL(selectedTemplate(const QString&)), this, SLOT(openWorkspace(const QString&)));
    connect(startupWidget, SIGNAL(gettingStartedSignal()), this, SLOT(helpFirstSteps()));
    connect(startupWidget, SIGNAL(helpTutorialSlides()), this, SLOT(helpTutorialSlides()));
    connect(startupWidget, SIGNAL(helpAnimation()), this, SLOT(helpAnimation()));
    return startupWidget;
}

} // namespace

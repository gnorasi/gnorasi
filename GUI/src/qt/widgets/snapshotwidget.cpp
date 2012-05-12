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

#include "voreen/qt/widgets/snapshotwidget.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/canvasrenderer.h"
#include "voreen/qt/widgets/processor/canvasrendererwidget.h"

#include <QApplication>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QUrl>
#include <QVBoxLayout>

namespace {
    QString whatsThisInfo = "<h3>Snapshot Widget</h3>This widget is used to create snapshots of different Canvases in the network. \
                            Additionally, it is possible to add arbitrary plugins to this list, one of which is the Plugin for creating \
                            snapshots of the NetworkEditor.";
}

namespace voreen {

SnapshotElement::SnapshotElement(QWidget* parent, CanvasRenderer* renderer) 
    : QGroupBox(parent)
    , savingPath_(QString::fromStdString(VoreenApplication::app()->getSnapshotPath()))
    , canvasRenderer_(renderer)
{
    if (renderer)
        setTitle(QString::fromStdString(renderer->getName()));
    
    QStringList resolutions;
    resolutions << "512x512" << "800x600" << "1024x768" << "1024x1024" << "1280x1024" <<
        "1600x1200" << "1920x1080" << "1920x1200" << "2048x2048";
    if (canvasRenderer_)
        resolutions << "canvas size";
    resolutions << "user-defined";

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    layout->setMargin(5);

    layout->addWidget(new QLabel(tr("Resolution:")));

    sizeCombo_ = new QComboBox;
    sizeCombo_->addItems(resolutions);
    layout->addWidget(sizeCombo_);

    layout->addSpacing(35);

    widthSpinBox_ = new QSpinBox;
    widthSpinBox_->setRange(1, GpuCaps.getMaxTextureSize());
    widthSpinBox_->setValue(800);
    layout->addWidget(widthSpinBox_);

    layout->addWidget(new QLabel("x"));

    heightSpinBox_ = new QSpinBox;
    heightSpinBox_->setRange(1, GpuCaps.getMaxTextureSize());
    heightSpinBox_->setValue(600);
    layout->addWidget(heightSpinBox_);

    layout->addStretch(10);

    makeSnapshotButton_ = new QPushButton();
    makeSnapshotButton_->setIcon(QIcon(":/icons/saveas.png"));
    makeSnapshotButton_->setIconSize(QSize(20, 20));
    makeSnapshotButton_->setToolTip(tr("Save snapshot as..."));
    layout->addWidget(makeSnapshotButton_);

    sizeComboChanged(sizeCombo_->currentIndex());

    connect(makeSnapshotButton_, SIGNAL(clicked()), this, SLOT(takeSnapshot()));
    connect(sizeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(sizeComboChanged(int)));
}

void SnapshotElement::sizeComboChanged(int newIndex) {
    // if user-defined
    if (newIndex == sizeCombo_->count() - 1) {
        widthSpinBox_->setEnabled(true);
        heightSpinBox_->setEnabled(true);
        return;
    }
    else {
        widthSpinBox_->setEnabled(false);
        heightSpinBox_->setEnabled(false);
    }

    widthSpinBox_->blockSignals(true);
    heightSpinBox_->blockSignals(true);

    // if canvas-size
    if (canvasRenderer_ && (newIndex == sizeCombo_->count() - 2)) {
        tgt::ivec2 size = canvasRenderer_->getCanvas()->getSize();
        widthSpinBox_->setValue(size.x);
        heightSpinBox_->setValue(size.y);

        if (canvasRenderer_ && canvasRenderer_->getProcessorWidget()) {
            CanvasRendererWidget* widget = dynamic_cast<CanvasRendererWidget*>(canvasRenderer_->getProcessorWidget());
            tgtAssert(widget, "CanvasRenderer widget is no CanvasRendererWidget");
            widget->setSnapshotElement(this);
        }
    }
    else {
        if (canvasRenderer_ && canvasRenderer_->getProcessorWidget()) {
            CanvasRendererWidget* widget = dynamic_cast<CanvasRendererWidget*>(canvasRenderer_->getProcessorWidget());
            tgtAssert(widget, "CanvasRenderer widget is no CanvasRendererWidget");
            widget->setSnapshotElement(0);
        }

        // create int from string and set Spinboxes to that values
        if (!sizeCombo_->currentText().contains("native")) {
            QString curText = sizeCombo_->currentText();
            int xIndex = curText.indexOf("x");
            int width = curText.left(xIndex).toInt();
            int height = curText.right(curText.size()-xIndex-1).toInt();
            widthSpinBox_->setValue(width);
            heightSpinBox_->setValue(height);
        }
    }

    widthSpinBox_->blockSignals(false);
    heightSpinBox_->blockSignals(false);
}

void SnapshotElement::updateFromProcessor() {
    // if canvas-size
    if (sizeCombo_->currentIndex() == sizeCombo_->count()-2) {
        tgt::ivec2 size = canvasRenderer_->getCanvas()->getSize();
        widthSpinBox_->setValue(size.x);
        heightSpinBox_->setValue(size.y);
    }
}

void SnapshotElement::takeSnapshot() {
    QFileDialog filedialog(this);
    filedialog.setWindowTitle(tr("Save Snapshot"));
    filedialog.setDirectory(savingPath_);
    filedialog.setDefaultSuffix(tr("png"));

    QStringList filter;
    filter << tr("PNG image (*.png)");
    filter << tr("JPEG image (*.jpg)");
    filter << tr("Windows Bitmap (*.bmp)");
    filter << tr("TIFF image (*.tif)");
    filedialog.setFilters(filter);
    filedialog.setAcceptMode(QFileDialog::AcceptSave);

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getSnapshotPath().c_str());
    urls << QUrl::fromLocalFile(VoreenApplication::app()->getDocumentsPath().c_str());
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    filedialog.setSidebarUrls(urls);

    struct tm* Tm;
    time_t currentTime = time(NULL);
    Tm = localtime(&currentTime);
    std::stringstream timestamp;
    timestamp << "snapshot " << (Tm->tm_year+1900) << "-" << (Tm->tm_mon+1) << "-" << Tm->tm_mday << "-" << Tm->tm_hour << "-" << Tm->tm_min << "-" << Tm->tm_sec;
    timestamp << ".png";
    filedialog.selectFile(tr(timestamp.str().c_str()));

    QStringList fileList;
    if (filedialog.exec())
        fileList = filedialog.selectedFiles();
    if (fileList.empty())
        return;

    QString filename = fileList.at(0);
    savingPath_ = filedialog.directory().absolutePath();

    if (!filename.endsWith(".jpg", Qt::CaseInsensitive) && !filename.endsWith(".png", Qt::CaseInsensitive) &&
        !filename.endsWith(".tif", Qt::CaseInsensitive) && !filename.endsWith(".bmp", Qt::CaseInsensitive)) {
            QString text = tr("Image file could not be saved.\n");
            int index = filename.lastIndexOf(".");
            if ((index == -1) || (index+1 == fileList[0].size()))
                text += tr("No file extension specified.");
            else
                text += tr("Invalid file extension: ") + filename.right(filename.size() - index - 1);

            QMessageBox::critical(this, tr("Error saving snapshot"), text);
            return;
    }

    saveSnapshot(filename, widthSpinBox_->value(), heightSpinBox_->value());
}

void SnapshotElement::saveSnapshot(const QString& filename, int width, int height) {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qApp->processEvents();
    try {
        bool success = canvasRenderer_->renderToImage(filename.toStdString(), tgt::ivec2(width, height));
        QApplication::restoreOverrideCursor();
        if (!success) {
            QMessageBox::warning(this, tr("Error saving snapshot"),
                tr("Snapshot could not be saved:\n%1").arg(
                QString::fromStdString(canvasRenderer_->getRenderToImageError())));
        }
    }
    catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Error saving snapshot"),
            tr("Snapshot could not be saved:\n%1").arg(e.what()));
    }
}

// ------------------------------------------------------------------------------------------------

SnapshotWidget::SnapshotWidget(QWidget* parent)
    : QWidget(parent)
{
    setWhatsThis(whatsThisInfo);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout_ = new QVBoxLayout(this);
    layout_->setAlignment(Qt::AlignTop);
    layout_->setMargin(5);

    line_ = new QFrame;
    line_->setFrameShape(QFrame::HLine);
    line_->setFrameShadow(QFrame::Sunken);
}

void SnapshotWidget::setCanvasRenderer(const std::vector<CanvasRenderer*>& canvasRenderer) {
    // first remove all old elements which are not present in the new set
    for (size_t i = 0; i < canvasRenderer_.size(); ++i) {
        CanvasRenderer* current = canvasRenderer_[i];
        std::vector<CanvasRenderer*>::const_iterator it = std::find(canvasRenderer.begin(), canvasRenderer.end(), current);
        if (it == canvasRenderer.end()) { // not found
            delete elements_.value(current);
            elements_.remove(current);
        }
    }

    // create new widget for the new renderers
    for (size_t i = 0; i < canvasRenderer.size(); ++i) {
        CanvasRenderer* current = canvasRenderer[i];
        std::vector<CanvasRenderer*>::const_iterator it = std::find(canvasRenderer_.begin(), canvasRenderer_.end(), current);
        if (it == canvasRenderer_.end()) { // not found
            SnapshotElement* element = new SnapshotElement(this, current);
            elements_.insert(current, element);
            canvasRenderer_.push_back(current);
        }
    }

    layoutElements();
}

void SnapshotWidget::clearCanvasRenderer() {
    for (size_t i = 0; i < canvasRenderer_.size(); ++i) {
        CanvasRenderer* renderer = canvasRenderer_[i];
        delete renderer;
    }

    canvasRenderer_.clear();
    elements_.clear();
}

void SnapshotWidget::setPluginRenderer(const std::vector<SnapshotElement*>& pluginRenderer) {
    // first remove all old elements which are not present in the new set
    for (size_t i = 0; i < pluginRenderer_.size(); ++i) {
        SnapshotElement* current = pluginRenderer_[i];
        std::vector<SnapshotElement*>::const_iterator it = std::find(pluginRenderer.begin(), pluginRenderer.end(), current);
        if (it == pluginRenderer.end()) // not found
            delete current;
    }

    // create new widget for the new renderers
    for (size_t i = 0; i < pluginRenderer.size(); ++i) {
        SnapshotElement* current = pluginRenderer[i];
        std::vector<SnapshotElement*>::const_iterator it = std::find(pluginRenderer_.begin(), pluginRenderer_.end(), current);
        if (it == pluginRenderer_.end()) // not found
            pluginRenderer_.push_back(current);
    }

    layoutElements();
}

void SnapshotWidget::clearPluginRenderer() {
    pluginRenderer_.clear();
}

void SnapshotWidget::layoutElements() {
    foreach (SnapshotElement* e, elements_)
        layout_->removeWidget(e);

    foreach (SnapshotElement* e, elements_)
        layout_->addWidget(e);

    if (elements_.size() > 0 && pluginRenderer_.size() > 0)
        layout_->addWidget(line_);

    foreach (SnapshotElement* e, pluginRenderer_)
        layout_->addWidget(e);
}

} // namespace

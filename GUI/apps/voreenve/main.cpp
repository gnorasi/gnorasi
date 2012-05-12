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

#include "voreenveapplication.h"
#include "voreenmainwindow.h"

#include "tgt/filesystem.h"
#include "voreen/core/version.h"

using namespace voreen;

int main(int argc, char** argv) {
    //disable argb visuals (Qt bug) fixes/works around invisible TF (etc) windows
#ifdef __unix__
    setenv ("XLIB_SKIP_ARGB_VISUALS", "1", 1);
#endif

    VoreenVEApplication vapp(argc, argv);

    vapp.setOverrideCursor(Qt::WaitCursor);

#ifdef VRN_SPLASHSCREEN
    VoreenSplashScreen splash;
    splash.showMessage("Creating application...");
    splash.show();
    qApp->processEvents();
#endif

    vapp.initialize(); // also loads modules, but does not initialize them


#if (QT_VERSION >= 0x040400) && !defined(__APPLE__)  && !defined(VRN_NO_STYLESHEET)
    // load and set style sheet (only on Qt 4.4 or newer)
    QFile file(":/voreenve/widgetstyle/voreen.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    vapp.setStyleSheet(styleSheet);
#endif

    // init resources for voreen_qt
    Q_INIT_RESOURCE(vrn_qt);
    // init common application resources
    Q_INIT_RESOURCE(voreenve);

#ifdef VRN_SPLASHSCREEN
    splash.showMessage("Creating main window...");
#endif
    VoreenMainWindow mainWindow(vapp.workspaceFilename_, vapp.resetSettings_);
    vapp.setMainWindow(&mainWindow);
    mainWindow.show();
#ifdef VRN_SPLASHSCREEN
    mainWindow.initialize(&splash, vapp.scriptFilename_.empty());  // also calls VoreenApplication::app()->initializeGL()
#else
    mainWindow.initialize();
#endif

    vapp.restoreOverrideCursor();

    if (!vapp.scriptFilename_.empty()) {
        // first make sure that all Qt events have been processed
        qApp->processEvents();
        mainWindow.runScript(vapp.scriptFilename_.c_str());
    }

    return vapp.exec();
}

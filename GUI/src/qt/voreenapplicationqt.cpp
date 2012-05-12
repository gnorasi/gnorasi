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

#include "voreen/qt/voreenapplicationqt.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/coremoduleqt.h"
#include "voreen/qt/versionqt.h"
#include "voreen/qt/progressdialog.h"
#include "voreen/core/utils/stringconversion.h"
#include "tgt/init.h"
#include "tgt/qt/qttimer.h"
#include "tgt/filesystem.h"
#include "tgt/shadermanager.h"

#ifndef VRN_NO_REGISTRATION_HEADER_GENERATION
    #include "modules/gen_moduleregistration_qt.h"
#else
    #include "modules/moduleregistration_qt.h"
#endif

#include <QApplication>
#include <QMainWindow>
#include <QDir>
#include <QSettings>

using std::string;

namespace {

} // namespace


namespace voreen {

VoreenApplicationQt* VoreenApplicationQt::qtApp_ = 0;
const std::string VoreenApplicationQt::loggerCat_ = "voreenqt.VoreenApplicationQt";

VoreenApplicationQt::VoreenApplicationQt(const std::string& name, const std::string& displayName,
                                         int argc, char** argv, ApplicationFeatures appType)
    : VoreenApplication(name, displayName, argc, argv, appType)
    , mainWindow_(0)
{
    QCoreApplication::setOrganizationName("Voreen");
    QCoreApplication::setOrganizationDomain("voreen.org");
    QCoreApplication::setApplicationName(displayName.c_str());

    qtApp_ = this;
}

VoreenApplicationQt::~VoreenApplicationQt() {
}

void VoreenApplicationQt::initialize() {
    VoreenApplication::initialize();
    if (!initialized_)
        return;

    LINFO("Qt version: " << VoreenVersionQt::getQtVersion());

    // Set the path for temporary files
    temporaryPath_ = QDir::tempPath().toStdString();

    //
    // Path detection
    //

    // shader path
    if (appFeatures_ & APP_SHADER) {
#ifdef VRN_INSTALL_PREFIX
        shaderPathQt_ = basePath_ + "/share/voreen/shaders";
#else
    #if defined(__APPLE__) && defined(VRN_DEPLOYMENT)
        shaderPathQt_ = appBundleResourcesPath_ + "/glsl/qt";
    #elif defined(VRN_DEPLOYMENT)
        shaderPathQt_ = basePath_ + "/glsl";
    #else
        shaderPathQt_ = basePath_ + "/src/qt/glsl";
    #endif
#endif
    }

    //
    // Modules
    //

    // core pseudo module is always included
    addQtModule(new CoreModuleQt());

    // module autoloading
    if (appFeatures_ & APP_AUTOLOAD_MODULES) {
        LDEBUG("Loading Voreen Qt modules from module registration header");
        addAllQtModules(this);
        std::vector<std::string> moduleNames;
        for (size_t i=0; i<qtModules_.size(); i++)
            moduleNames.push_back(qtModules_[i]->getName());
        LINFO("Voreen Qt modules: " << strJoin(moduleNames, ", "));
    }
    else {
        LDEBUG("Module auto loading disabled");
    }
}

void VoreenApplicationQt::deinitialize() {
    VoreenApplication::deinitialize();

    qtModules_.clear(); //< have been deleted by VoreenApplication::deinit();
}

void VoreenApplicationQt::initializeGL() throw (VoreenException) {
    VoreenApplication::initializeGL();
    glewInit();

    ShdrMgr.addPath(getShaderPathQt());
}

void VoreenApplicationQt::deinitializeGL() throw (VoreenException) {
    VoreenApplication::deinitializeGL();
}

void VoreenApplicationQt::setMainWindow(QMainWindow* mainWindow) {
    mainWindow_ = mainWindow;
}

QMainWindow* VoreenApplicationQt::getMainWindow() const {
    return mainWindow_;
}

VoreenApplicationQt* VoreenApplicationQt::qtApp() {
    return qtApp_;
}

tgt::Timer* VoreenApplicationQt::createTimer(tgt::EventHandler* handler) const {
    return new tgt::QtTimer(handler);
}

ProgressDialog* VoreenApplicationQt::createProgressDialog() const {
    return new ProgressDialog(getMainWindow());
}

std::string VoreenApplicationQt::getShaderPathQt(const std::string& filename) const {
    return shaderPathQt_ + (filename.empty() ? "" : "/" + filename);
}

void VoreenApplicationQt::addQtModule(VoreenModuleQt* qtModule) {
    tgtAssert(qtModule, "null pointer passed");

    // qt modules are subject to standard module handling
    VoreenApplication::addModule(qtModule);

    // additionally store qt modules separately (currently no use for this, though)
    if (std::find(qtModules_.begin(), qtModules_.end(), qtModule) == qtModules_.end())
        qtModules_.push_back(qtModule);
    else
        LWARNING("Qt Module '" << qtModule->getName() << "' has already been registered. Skipping.");
}

const std::vector<VoreenModuleQt*>& VoreenApplicationQt::getQtModules() const {
    return qtModules_;
}

VoreenModuleQt* VoreenApplicationQt::getQtModule(const std::string& moduleName) const {
    for (size_t i = 0 ; i < qtModules_.size() ; ++i) {
        VoreenModuleQt* qtModule = qtModules_.at(i);
        if (qtModule->getName() == moduleName)
            return qtModule;
    }
    return 0;
}

} // namespace


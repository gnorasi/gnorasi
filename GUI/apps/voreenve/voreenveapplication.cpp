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

#include "tgt/filesystem.h"
#include "tgt/logmanager.h"
#include "voreen/core/version.h"
#include "voreen/core/utils/stringconversion.h"
#include "voreen/qt/voreenapplicationqt.h"
#include "voreenmoduleve.h"

#ifndef VRN_NO_REGISTRATION_HEADER_GENERATION
#include "../../modules/gen_moduleregistration_ve.h"
#else
#include "../../modules/moduleregistration_ve.h"
#endif

#include <QMessageBox>

namespace voreen {

VoreenVEApplication* VoreenVEApplication::veApp_ = 0;
const std::string VoreenVEApplication::loggerCat_("voreenve.VoreenVEApplication");

VoreenVEApplication::VoreenVEApplication(int& argc, char** argv)
    : QApplication(argc, argv)
#ifdef VRN_ADD_FILE_LOGGER
    , VoreenApplicationQt("voreenve", "VoreenVE" + VoreenVersion::getVersion(), argc, argv, VoreenApplication::APP_ALL)
#else
    , VoreenApplicationQt("voreenve", "VoreenVE" + VoreenVersion::getVersion(), argc, argv, VoreenApplication::APP_DEFAULT)
#endif
{
    resetSettings_ = false;
    veApp_ = this;
}

void VoreenVEApplication::prepareCommandParser() {
    VoreenApplicationQt::prepareCommandParser();

    CommandlineParser* p = getCommandLineParser();
    p->addCommand(new SingleCommand<std::string>(&workspaceFilename_, "--workspace", "-w",
        "Loads a workspace", "<workspace file>"));
    p->addCommand(new SingleCommandZeroArguments(&resetSettings_, "--resetSettings", "",
        "Restores window settings and default paths"));

#ifdef VRN_MODULE_PYTHON
    p->addCommand(new SingleCommand<std::string>(&scriptFilename_, "--script", "-s",
        "Runs a python script", "<script file>"));
#endif
}

void VoreenVEApplication::initialize() {
    VoreenApplicationQt::initialize();
    if (!initialized_)
        return;

    // Register VoreenVE Modules
    if (appFeatures_ & APP_AUTOLOAD_MODULES) {
        LDEBUG("Loading VoreenVE modules from module registration header");
        addAllVEModules(this);
        std::vector<std::string> moduleNames;
        for (size_t i=0; i<veModules_.size(); i++)
            moduleNames.push_back(veModules_[i]->getName());
        LINFO("VoreenVE modules: " << strJoin(moduleNames, ", "));
    }
    else {
        LDEBUG("Module auto loading disabled");
    }
}

void VoreenVEApplication::deinitialize() {
    if (!initialized_) {
        if (tgt::LogManager::isInited())
            LWARNING("deinit() Application not initialized. Skip.");
        return;
    }

    // clear modules
    LDEBUG("Deleting VoreenVE modules ...");
    for (int i=(int)veModules_.size()-1; i>=0; i--) {
        LDEBUG("Deleting VoreenVE module '" << veModules_.at(i)->getName() << "'");
        delete veModules_.at(i);
    }
    veModules_.clear();

    VoreenApplicationQt::deinitialize();
}

void VoreenVEApplication::initializeGL() throw (VoreenException) {
    VoreenApplicationQt::initializeGL();

    // initialize VoreenVE modules
    for (size_t i=0; i<veModules_.size(); i++) {
        try {
            LINFO("Initializing module '" << veModules_.at(i)->getName() << "'");
            veModules_.at(i)->initialize();
            veModules_.at(i)->initialized_ = true;
        }
        catch (const VoreenException& e) {
            LERROR("VoreenException during initialization of module '" << veModules_.at(i)->getName() << "': " << e.what());
            veModules_.at(i)->initialized_ = false;
        }
        catch (const std::exception& e) {
            LERROR("std::exception during initialization of module '" << veModules_.at(i)->getName() << "': " << e.what());
            veModules_.at(i)->initialized_ = false;
        }
        catch (...) {
            LERROR("Unknown exception during initialization of module '" << veModules_.at(i)->getName() << "'");
            veModules_.at(i)->initialized_ = false;
        }
    }
}

void VoreenVEApplication::deinitializeGL() throw (VoreenException) {
    if (!initializedGL_) {
        if (tgt::LogManager::isInited())
            LWARNING("deinitializeGL() OpenGL not initialized. Skip.");
        return;
    }

    LDEBUG("Deinitializing VoreenVE modules ...");
    for (int i=(int)veModules_.size()-1; i>=0; i--) {

        if (veModules_.at(i)->isInitialized()) {
            try {
                LINFO("Deinitializing module '" << veModules_.at(i)->getName() << "'");
                veModules_.at(i)->deinitialize();
                veModules_.at(i)->initialized_ = false;
            }
            catch (const VoreenException& e) {
                LERROR("VoreenException during deinitialization of module '" << veModules_.at(i)->getName() << "': " << e.what());
            }
            catch (const std::exception& e) {
                LERROR("std::exception during deinitialization of module '" << veModules_.at(i)->getName() << "': " << e.what());
            }
            catch (...) {
                LERROR("unknown exception during deinitialization of module '" << veModules_.at(i)->getName() << "'");
            }
        }
        else {
            LWARNING("Skipping deinitialization of module '" << veModules_.at(i)->getName() << "': not initialized");
        }
    }
    
    VoreenApplicationQt::deinitializeGL();
}

VoreenVEApplication* VoreenVEApplication::veApp() {
    return veApp_;
}

bool VoreenVEApplication::notify(QObject* receiver, QEvent* event) {
    bool result = false;
    try {
        result = QApplication::notify(receiver, event);
    }
    catch (const VoreenException& e) {
        LERRORC("voreenve.main", "Caught unhandled VoreenException: " << e.what());
#ifndef TGT_NON_INTERACTIVE_ASSERT
        int choice = QMessageBox::critical(0, tr("VoreenVE"), tr("Caught unhandled VoreenException:\n\"")
                                           + e.what() + +"\"\n" + tr("Continue?"),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        if (choice == QMessageBox::Cancel) {
  #ifdef VRN_DEBUG
            TGT_THROW_BREAKPOINT;
  #else
            exit(1);
  #endif
        }
#else
        exit(1);
#endif // TGT_NON_INTERACTIVE_ASSERT
    }
    catch (const std::exception& e) {
        LERRORC("voreenve.main", "Caught unhandled std::exception: " << e.what());
#ifndef TGT_NON_INTERACTIVE_ASSERT
        int choice = QMessageBox::critical(0, tr("VoreenVE"), tr("Caught unhandled std::exception:\n\"")
                                           + e.what() + "\"\n" + tr("Continue?"),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        if (choice == QMessageBox::Cancel) {
#ifdef VRN_DEBUG
            TGT_THROW_BREAKPOINT;
#else
            exit(1);
#endif
        }
#else
            exit(1);
#endif // TGT_NON_INTERACTIVE_ASSERT
    }
    catch (...) {
        LERRORC("voreenve.main", "Caught unhandled unknown exception!");
#ifndef TGT_NON_INTERACTIVE_ASSERT
        int choice = QMessageBox::critical(0, tr("VoreenVE"), tr("Caught unhandled unknown exception!\nContinue?"),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        if (choice == QMessageBox::Cancel) {
#ifdef VRN_DEBUG
            TGT_THROW_BREAKPOINT;
#else
            exit(1);
#endif
        }
#else
        exit(1);
#endif // TGT_NON_INTERACTIVE_ASSERT
        throw;
    }
    return result;
}

void VoreenVEApplication::addVEModule(VoreenModuleVE* module) {
    tgtAssert(module, "null pointer passed");
    if (std::find(veModules_.begin(), veModules_.end(), module) == veModules_.end())
        veModules_.push_back(module);
    else
        LWARNING("VoreenVE module '" << module->getName() << "' has already been registered. Skipping.");
}

const std::vector<VoreenModuleVE*>& VoreenVEApplication::getVEModules() const {
    return veModules_;
}

} // namespace

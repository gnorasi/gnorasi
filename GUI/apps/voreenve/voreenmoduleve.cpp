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

#include "voreenmoduleve.h"

#include "voreenveplugin.h"

namespace voreen {

const std::string VoreenModuleVE::loggerCat_("voreenve.VoreenModuleVE");

VoreenModuleVE::VoreenModuleVE()
    : name_("undefined")
    , description_("")
    , initialized_(false)
{}

VoreenModuleVE::~VoreenModuleVE() {
    if (isInitialized()) {
        LWARNING("VoreenVE-Module '" << getName() << "' has not been deinitialized before destruction");
    }

    // are deleted by Qt memory management
    vePlugins_.clear();
}

bool VoreenModuleVE::isInitialized() const {
    return initialized_;
}

std::string VoreenModuleVE::getName() const {
    return name_;
}

std::string VoreenModuleVE::getDescription() const {
    return description_;
}

const std::vector<VoreenVEPlugin*>& VoreenModuleVE::getVoreenVEPlugins() const {
    return vePlugins_;
}

void VoreenModuleVE::initialize() throw (tgt::Exception) {
    // plugins are initialized by VoreenMainWindow
}

void VoreenModuleVE::deinitialize() throw (tgt::Exception) {
    // plugins are deinitialized by VoreenMainWindow 
}

void VoreenModuleVE::setName(const std::string& name) {
    name_ = name;
}

void VoreenModuleVE::setDescription(const std::string& desc) {
    description_ = desc;
}

void VoreenModuleVE::addVoreenVEPlugin(VoreenVEPlugin* plugin) {
    tgtAssert(plugin, "null pointer passed");
    vePlugins_.push_back(plugin);
}

} // namespace

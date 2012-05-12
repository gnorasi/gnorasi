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

#ifndef VRN_VOREENMODULEVE_H
#define VRN_VOREENMODULEVE_H

#include "tgt/exception.h"

#include <string>
#include <vector>

namespace voreen {

class VoreenVEPlugin;

/**
 * Responsible for registering VoreenVE resources:
 * - VoreenVEPlugin
 */
class VoreenModuleVE {

    friend class VoreenVEApplication;

public:
    /**
     * Instantiate and register the module's resources
     * in the derived class's constructor.
     */
    VoreenModuleVE();

    virtual ~VoreenModuleVE();

    /**
     * Returns true, if the module has been successfully initialized,
     * and has not yet been deinitialized.
     */
    bool isInitialized() const;

    /**
     * Returns the module's name.
     *
     * @see setName
     */
    std::string getName() const;

    /**
     * Module documentation that is intended
     * to be presented to the user.
     *
     * @see setDescription
     */
    std::string getDescription() const;

    /**
     * Returns the module's VoreenVE plugins.
     *
     * @see addVoreenVEPlugin
     */
    const std::vector<VoreenVEPlugin*>& getVoreenVEPlugins() const;

protected:
    /**
     * OpenGL-dependent or time-consuming initializations
     * should be performed here. However, it is usually
     * not necessary to override this function.
     *
     * @throw tgt::Exception to indicate an initialization failure
     */
    virtual void initialize()
        throw (tgt::Exception);

    /**
     * OpenGL-dependent or time-consuming deinitializations
     * should be performed here. However, it is usually
     * not necessary to override this function.
     *
     * @throw tgt::Exception to indicate a deinitialization failure
     */
    virtual void deinitialize()
        throw (tgt::Exception);

    /**
     * Specifies the module's name. To be called in the
     * derived class's constructor.
     *
     * @note Setting a name is mandatory.
     */
    void setName(const std::string& name);
   
    /**
     * Provides an optional description of the module. 
     * To be called in the derived class's constructor.
     */
    void setDescription(const std::string& desc);

    /**
     * Registers the passed VoreenVE plugin. To be called in the
     * derived class's constructor.
     */
    void addVoreenVEPlugin(VoreenVEPlugin* plugin);

private:
    std::string name_;
    std::string description_;
    std::vector<VoreenVEPlugin*> vePlugins_;

    bool initialized_;  ///< set by VoreenVEApplication

    static const std::string loggerCat_;
};

} // namespace

#endif // VRN_VOREENMODULEVE_H

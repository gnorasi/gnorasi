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

#ifndef VRN_VOREENVEAPPLICATION_H
#define VRN_VOREENVEAPPLICATION_H

#include "voreen/qt/voreenapplicationqt.h"
#include <QApplication>

namespace voreen {

class VoreenModuleVE;

/**
 * Common subclass of QApplication and VoreenApplicationQt. 
 * Extends the CommandLineParser by VoreenVE-specific commands
 * and handles the VoreenVEModules. It also acts as exception catcher 
 * "of last resort".
 */
class VoreenVEApplication : public QApplication, public VoreenApplicationQt {
Q_OBJECT
public:
    VoreenVEApplication(int& argc, char** argv);

    virtual void initialize();
    virtual void deinitialize();

    virtual void initializeGL() throw (VoreenException);
    virtual void deinitializeGL() throw (VoreenException);

    static VoreenVEApplication* veApp();

    /// Adds VoreenVE-specific commands. 
    virtual void prepareCommandParser();

    /// Overridden for catching all uncaught exceptions reaching the application level.
    virtual bool notify(QObject* receiver, QEvent* event);

    /// Registers a VoreenVE module.
    void addVEModule(VoreenModuleVE* module);

    /// Registers a VoreenVE module.
    const std::vector<VoreenModuleVE*>& getVEModules() const;

    std::string workspaceFilename_;
    std::string scriptFilename_;
    bool resetSettings_;

private:
    static VoreenVEApplication* veApp_;
    std::vector<VoreenModuleVE*> veModules_;

    static const std::string loggerCat_;
};

} // namespace

#endif

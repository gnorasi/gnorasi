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

#ifndef VRN_MODULEREGISTRATION_QT_H
#define VRN_MODULEREGISTRATION_QT_H

#include "voreen/qt/voreenapplicationqt.h"

// standard modules
#ifdef VRN_MODULE_BASE
#include "modules/base/basemoduleqt.h"
#endif

#ifdef VRN_MODULE_OPENCL
#include "modules/opencl/openclmoduleqt.h"
#endif

#ifdef VRN_MODULE_PLOTTING
#include "modules/plotting/plottingmoduleqt.h"
#endif

#ifdef VRN_MODULE_PYTHON
#include "modules/python/pythonmoduleqt.h"
#endif



namespace voreen {

void addAllQtModules(VoreenApplicationQt* qtApp) {
    #ifdef VRN_MODULE_BASE
        qtApp->addQtModule(new BaseModuleQt());
    #endif

    #ifdef VRN_MODULE_OPENCL
        qtApp->addQtModule(new OpenCLModuleQt());
    #endif

    #ifdef VRN_MODULE_PLOTTING
        qtApp->addQtModule(new PlottingModuleQt());
    #endif

    #ifdef VRN_MODULE_PYTHON
        qtApp->addQtModule(new PythonModuleQt());
    #endif
    

}

} // namespace

#endif // VRN_MODULEREGISTRATION_QT_H

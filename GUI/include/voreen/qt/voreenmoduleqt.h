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

#ifndef VRN_VOREENMODULEQT_H
#define VRN_VOREENMODULEQT_H

#include "voreen/core/voreenmodule.h"

#include <string>
#include <vector>
#include "voreen/core/utils/exception.h"

namespace voreen {

/**
 * Marker interface for Voreen Qt module classes.
 *
 * A qt module class is responsible for registering the
 * module's widgets at runtime:
 *  - ProcessorWidgets
 *  - PropertyWidgets
 *
 * The registration is to be done in the constructor.
 *
 * @see http://www.voreen.org/402-Adding-a-Module.html
 *
 */
class VoreenModuleQt : public VoreenModule {

    friend class VoreenApplicationQt;

protected:
    static const std::string loggerCat_;
};

} // namespace

#endif // VRN_VOREENMODULEQT_H

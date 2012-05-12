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

#ifndef VRN_DEPENDENCYLINKEVALUATORS_H
#define VRN_DEPENDENCYLINKEVALUATORS_H

#include "voreen/core/properties/link/dependencylinkevaluator.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/vectorproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/volumehandleproperty.h"
#include "voreen/core/datastructures/volume/volumehandle.h"

namespace voreen {

///Special DependencyLinkEvaluator for VolumeHandle that deletes entries in the history if the corresponding VolumeHandle is deleted.
class VRN_CORE_API DependencyLinkEvaluatorVolumeHandle : public DependencyLinkEvaluator, public VolumeHandleObserver {
public:
    std::string getClassName() const { return "DependencyLinkEvaluatorVolumeHandle"; }
    LinkEvaluatorBase* create() const { return new DependencyLinkEvaluatorVolumeHandle(); }

    void eval(Property* src, Property* dest) throw (VoreenException);
    void propertiesChanged(Property* src, Property* dst);

    void volumeChange(const VolumeHandleBase*);
    void volumeHandleDelete(const VolumeHandleBase* source);
};

} // namespace

#endif // VRN_DEPENDENCYLINKEVALUATORS_H

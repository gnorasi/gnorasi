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

#ifndef VRN_VOLUMECOLLECTIONINTERPOLATIONFUNCTIONS_H
#define VRN_VOLUMECOLLECTIONINTERPOLATIONFUNCTIONS_H

#include "voreen/core/animation/interpolationfunction.h"
#include "voreen/core/properties/volumecollectionproperty.h"

namespace voreen {

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API InterpolationFunction<VolumeCollection*>;
#endif

/**
 * This class offers an interpolation function for volumecollections. Interpolation: focus on startvalue.
 */
class VRN_CORE_API VolumeCollectionStartInterpolationFunction : public InterpolationFunction<VolumeCollection*> {
public:
    VolumeCollectionStartInterpolationFunction();
    InterpolationFunction<VolumeCollection*>* clone() const;
    VolumeCollection* interpolate(VolumeCollection* startvalue, VolumeCollection* endvalue, float time) const;

    std::string getMode() const;
    std::string getIdentifier() const;
};

/**
 * This class offers an interpolation function for volumecollections. Interpolation: focus on endvalue.
 */
class VRN_CORE_API VolumeCollectionEndInterpolationFunction : public InterpolationFunction<VolumeCollection*> {
public:
    VolumeCollectionEndInterpolationFunction();
    InterpolationFunction<VolumeCollection*>* clone() const;
    VolumeCollection* interpolate(VolumeCollection* startvalue, VolumeCollection* endvalue, float time) const;

    std::string getMode() const;
    std::string getIdentifier() const;
};

/**
 * This class offers an interpolation function for volumecollections. Interpolation: bisection.
 */
class VRN_CORE_API VolumeCollectionStartEndInterpolationFunction : public InterpolationFunction<VolumeCollection*> {
public:
    VolumeCollectionStartEndInterpolationFunction();
    InterpolationFunction<VolumeCollection*>* clone() const;
    VolumeCollection* interpolate(VolumeCollection* startvalue, VolumeCollection* endvalue, float time) const;

    std::string getMode() const;
    std::string getIdentifier() const;
};

} // namespace voreen
#endif

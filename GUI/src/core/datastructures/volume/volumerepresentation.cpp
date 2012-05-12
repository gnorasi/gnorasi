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

#include "voreen/core/datastructures/volume/volumerepresentation.h"
#include "voreen/core/datastructures/volume/diskrepresentation.h"
#include "voreen/core/datastructures/volume/volumegl.h"

using tgt::vec3;
using tgt::svec3;

namespace voreen {

const std::string VolumeRepresentation::loggerCat_("voreen.VolumeRepresentation");

VolumeRepresentation::VolumeRepresentation(const svec3& dimensions)
    : dimensions_(dimensions), originalDimensions_(dimensions)
{
    borders_ = VolumeBorders();
    updateNumVoxels();
    updateHasBorder();
}

VolumeRepresentation::VolumeRepresentation(const VolumeRepresentation* volRep)
    : dimensions_(volRep->getDimensions()), originalDimensions_(volRep->getOriginalDimensions()), borders_(volRep->getBorder())
{
    updateNumVoxels();
    updateHasBorder();
}

VolumeRepresentation::VolumeRepresentation(const svec3& dimensions, const VolumeBorders& border)
    : dimensions_(dimensions), originalDimensions_(dimensions), borders_(border)
{
    updateNumVoxels();
    updateHasBorder();
}

VolumeRepresentation::VolumeBorders VolumeRepresentation::getBorder() const {
    return borders_;
}

svec3 VolumeRepresentation::getBorderLLF() const{
    return borders_.llf;
}

svec3 VolumeRepresentation::getBorderURB() const {
    return borders_.urb;
}

svec3 VolumeRepresentation::getDimensions() const {
    return dimensions_;
}

svec3 VolumeRepresentation::getOriginalDimensions() const {
    return originalDimensions_;
}

svec3 VolumeRepresentation::getDimensionsWithBorder() const {
    return dimensions_+getBorderLLF()+getBorderURB();
}

size_t VolumeRepresentation::getNumVoxels() const {
    return numVoxels_;
}

size_t VolumeRepresentation::getNumVoxelsWithBorder() const {
    return numVoxels_ + borders_.numVoxels;
}

bool VolumeRepresentation::hasBorder() const {
    return borders_.hasBorder;
}

void VolumeRepresentation::updateNumVoxels() {
    tgt::svec3 trueDimension = dimensions_ + borders_.llf + borders_.urb;
    size_t numVoxelsWithBorder = (size_t) trueDimension.x * (size_t) trueDimension.y * (size_t) trueDimension.z;
    numVoxels_ = (size_t) dimensions_.x * (size_t) dimensions_.y * (size_t) dimensions_.z;
    borders_.numVoxels = numVoxelsWithBorder - numVoxels_;
}

void VolumeRepresentation::updateHasBorder() {
    tgt::svec3 noBorder = tgt::svec3(0,0,0);
    borders_.hasBorder = (borders_.llf != noBorder && borders_.urb != noBorder);
}

//---------------------------------------------------------------------------------

ConverterFactory::ConverterFactory() {
    addConverter(new RepresentationConverterUploadGL());
    addConverter(new RepresentationConverterDownloadGL());
    addConverter(new RepresentationConverterLoadFromDisk());
}

ConverterFactory::~ConverterFactory() {
    while(!converters_.empty()) {
        delete converters_.back();
        converters_.pop_back();
    }
}
} // namespace voreen

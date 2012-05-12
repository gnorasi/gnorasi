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

#include "voreen/core/io/serialization/meta/coremetadatafactory.h"

#include "voreen/core/io/serialization/meta/aggregationmetadata.h"
#include "voreen/core/io/serialization/meta/networkannotation.h"
#include "voreen/core/io/serialization/meta/positionmetadata.h"
#include "voreen/core/io/serialization/meta/primitivemetadata.h"
#include "voreen/core/io/serialization/meta/selectionmetadata.h"
#include "voreen/core/io/serialization/meta/realworldmappingmetadata.h"
#include "voreen/core/io/serialization/meta/windowstatemetadata.h"
#include "voreen/core/io/serialization/meta/zoommetadata.h"

namespace voreen {

CoreMetaDataFactory::CoreMetaDataFactory() {
    // primitive scalar types
    registerType(new BoolMetaData());
    registerType(new StringMetaData());
    registerType(new IntMetaData());
    registerType(new SizeTMetaData());
    registerType(new FloatMetaData());
    registerType(new DoubleMetaData());

    // primitive vector types
    registerType(new Vec2MetaData());
    registerType(new IVec2MetaData());
    registerType(new Vec3MetaData());
    registerType(new IVec3MetaData());
    registerType(new Vec4MetaData());
    registerType(new IVec4MetaData());
    registerType(new Mat2MetaData());
    registerType(new Mat3MetaData());
    registerType(new Mat4MetaData());
    
    registerType(new DateTimeMetaData());

    registerType(new AggregationMetaDataContainer());
    registerType(new NetworkAnnotationContainer());
    registerType(new PositionMetaData());
    registerType(new RealWorldMappingMetaData());
    registerType("SelectionMetaData::Processor", new SelectionMetaData<Processor*>());
    registerType("SelectionMetaData::vector::PortConnection", new SelectionMetaData<std::vector<PortConnection> >());
    registerType(new WindowStateMetaData());
    registerType(new ZoomMetaData());
}

} // namespace voreen

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

#include "voreen/core/coremodule.h"

#include "voreen/core/processors/canvasrenderer.h"

#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/cameraproperty.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/fontproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/matrixproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/propertyvector.h"
#include "voreen/core/properties/shaderproperty.h"
#include "voreen/core/properties/stringexpressionproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/transfuncproperty.h"
#include "voreen/core/properties/vectorproperty.h"
#include "voreen/core/properties/volumecollectionproperty.h"
#include "voreen/core/properties/volumehandleproperty.h"
#include "voreen/core/properties/voxeltypeproperty.h"

#include "voreen/core/io/datvolumereader.h"
#include "voreen/core/io/datvolumewriter.h"
#include "voreen/core/io/rawvolumereader.h"
#include "voreen/core/io/vvdvolumereader.h"
#include "voreen/core/io/vvdvolumewriter.h"

#include "voreen/core/io/serialization/meta/coremetadatafactory.h"
#include "voreen/core/processors/processorfactory.h"
#include "voreen/core/datastructures/geometry/geometryfactory.h"
#include "voreen/core/datastructures/transfunc/transfuncfactory.h"
#include "voreen/core/properties/link/linkevaluatorfactory.h"
#include "voreen/core/datastructures/volume/volumederiveddatafactory.h"
#include "voreen/core/animation/animation.h"

#include "voreen/core/properties/link/corelinkevaluatorfactory.h"

#include "voreen/core/datastructures/volume/operators/volumeoperatorcalcerror.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorhalfsample.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorinvert.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorisuniform.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatormedian.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatormirror.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatormorphology.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatornormalize.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatornumsignificant.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorresample.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorresize.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorsubset.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorswapendianness.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatortranspose.h"

#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string CoreModule::loggerCat_("voreen.CoreModule");

CoreModule::CoreModule()
    : VoreenModule()
{
    setName("Core");

    tgtAssert(VoreenApplication::app(), "VoreenApplicaton not instantiated");
#ifndef VRN_DEPLOYMENT
    setXMLFileName(VoreenApplication::app()->getBasePath() + "/src/core/coremodule.xml", true);
#else
    setXMLFileName("core/coremodule.xml");
#endif

    // core processors
    addProcessor(new CanvasRenderer());

    // core properties
    addProperty(new FloatProperty());
    addProperty(new IntProperty());
    addProperty(new IntVec2Property());
    addProperty(new IntVec3Property());
    addProperty(new IntVec4Property());
    addProperty(new FloatVec2Property());
    addProperty(new FloatVec3Property());
    addProperty(new FloatVec4Property());
    addProperty(new FloatMat2Property());
    addProperty(new FloatMat3Property());
    addProperty(new FloatMat4Property());

    addProperty(new BoolProperty());
    addProperty(new ButtonProperty());
    addProperty(new CameraProperty());
    addProperty(new FileDialogProperty());
    addProperty(new FontProperty());
    addProperty(new PropertyVector());
    addProperty(new ShaderProperty());
    addProperty(new StringExpressionProperty());
    addProperty(new StringProperty());
    addProperty(new TransFuncProperty());
    addProperty(new VolumeCollectionProperty());
    addProperty(new VolumeHandleProperty());
    addProperty(new VoxelTypeProperty());

    addProperty(new IntOptionProperty());
    addProperty(new FloatOptionProperty());
    addProperty(new GLEnumOptionProperty());
    addProperty(new StringOptionProperty());

    // core io
    addVolumeReader(new DatVolumeReader());
    addVolumeReader(new RawVolumeReader());
    addVolumeReader(new VvdVolumeReader());
    addVolumeWriter(new DatVolumeWriter());
    addVolumeWriter(new VvdVolumeWriter());

    // core factories
    addSerializerFactory(new CoreMetaDataFactory());
    addSerializerFactory(ProcessorFactory::getInstance());
    addSerializerFactory(new GeometryFactory());
    addSerializerFactory(new TransFuncFactory());
    addSerializerFactory(new VolumeDerivedDataFactory());
    std::vector<SerializableFactory*> animationFactories = Animation::getSerializerFactories();
    for (size_t i=0; i<animationFactories.size(); i++)
        addSerializerFactory(animationFactories.at(i));

    addLinkEvaluatorFactory(new CoreLinkEvaluatorFactory());

    // Instance operator for all scalar types:
    INST_SCALAR_TYPES(VolumeOperatorInvert, VolumeOperatorInvertGeneric)
    //INST_VECTOR_TYPES(NewVolumeOperatorInvert, VolumeOperatorInvertGeneric)

    INST_SCALAR_TYPES(VolumeOperatorMirrorX, VolumeOperatorMirrorXGeneric)
    INST_VECTOR_TYPES(VolumeOperatorMirrorX, VolumeOperatorMirrorXGeneric)

    INST_SCALAR_TYPES(VolumeOperatorMirrorY, VolumeOperatorMirrorYGeneric)
    INST_VECTOR_TYPES(VolumeOperatorMirrorY, VolumeOperatorMirrorYGeneric)

    INST_SCALAR_TYPES(VolumeOperatorMirrorZ, VolumeOperatorMirrorZGeneric)
    INST_VECTOR_TYPES(VolumeOperatorMirrorZ, VolumeOperatorMirrorZGeneric)

    INST_SCALAR_TYPES(VolumeOperatorTranspose, VolumeOperatorTransposeGeneric)
    INST_VECTOR_TYPES(VolumeOperatorTranspose, VolumeOperatorTransposeGeneric)
    INST_TENSOR_TYPES(VolumeOperatorTranspose, VolumeOperatorTransposeGeneric)

    INST_SCALAR_TYPES(VolumeOperatorSwapEndianness, VolumeOperatorSwapEndiannessGeneric)
    INST_VECTOR_TYPES(VolumeOperatorSwapEndianness, VolumeOperatorSwapEndiannessGeneric)
    INST_TENSOR_TYPES(VolumeOperatorSwapEndianness, VolumeOperatorSwapEndiannessGeneric)

    INST_SCALAR_TYPES(VolumeOperatorMedian, VolumeOperatorMedianGeneric)

    INST_SCALAR_TYPES(VolumeOperatorDilation, VolumeOperatorDilationGeneric)
    INST_SCALAR_TYPES(VolumeOperatorErosion, VolumeOperatorErosionGeneric)

    INST_SCALAR_TYPES(VolumeOperatorResample, VolumeOperatorResampleGeneric)
    INST_VECTOR_TYPES(VolumeOperatorResample, VolumeOperatorResampleGeneric)

    INST_SCALAR_TYPES(VolumeOperatorHalfsample, VolumeOperatorHalfsampleGeneric)
    INST_VECTOR_TYPES(VolumeOperatorHalfsample, VolumeOperatorHalfsampleGeneric)

    INST_SCALAR_TYPES(VolumeOperatorResize, VolumeOperatorResizeGeneric)
    INST_VECTOR_TYPES(VolumeOperatorResize, VolumeOperatorResizeGeneric)
    INST_TENSOR_TYPES(VolumeOperatorResize, VolumeOperatorResizeGeneric)

    INST_SCALAR_TYPES(VolumeOperatorSubset, VolumeOperatorSubsetGeneric)
    INST_VECTOR_TYPES(VolumeOperatorSubset, VolumeOperatorSubsetGeneric)

    INST_SCALAR_TYPES(VolumeOperatorIsUniform, VolumeOperatorIsUniformGeneric)
    INST_VECTOR_TYPES(VolumeOperatorIsUniform, VolumeOperatorIsUniformGeneric)

    INST_SCALAR_TYPES(VolumeOperatorNumSignificant, VolumeOperatorNumSignificantGeneric)
    INST_VECTOR_TYPES(VolumeOperatorNumSignificant, VolumeOperatorNumSignificantGeneric)

    INST_SCALAR_TYPES(VolumeOperatorNormalize, VolumeOperatorNormalizeGeneric)
    //INST_VECTOR_TYPES(VolumeOperatorNormalize, VolumeOperatorNormalizeGeneric)

    INST_SCALAR_TYPES(VolumeOperatorCalcError, VolumeOperatorCalcErrorGeneric)
    INST_VECTOR_TYPES(VolumeOperatorCalcError, VolumeOperatorCalcErrorGeneric)
}

} // namespace

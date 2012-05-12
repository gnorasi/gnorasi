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

#include "modules/base/basemodule.h"

///
/// Processors
///

// data sources
#include "modules/base/processors/datasource/geometrysource.h"
#include "modules/base/processors/datasource/imagesequencesource.h"
#include "modules/base/processors/datasource/imagesource.h"
#include "modules/base/processors/datasource/rawtexturesave.h"
#include "modules/base/processors/datasource/rawtexturesource.h"
#include "modules/base/processors/datasource/textseriessource.h"
#include "modules/base/processors/datasource/textsource.h"
#include "modules/base/processors/datasource/volumecollectionsource.h"
#include "modules/base/processors/datasource/volumeseriessource.h"
#include "modules/base/processors/datasource/volumesource.h"

// entry-exit points
#include "modules/base/processors/entryexitpoints/meshentryexitpoints.h"

// geometry
/*
#include "modules/base/processors/geometry/boundingboxrenderer.h"
#include "modules/base/processors/geometry/camerapositionrenderer.h"
#include "modules/base/processors/geometry/depthpeelingprocessor.h"
#include "modules/base/processors/geometry/eepgeometryintegrator.h"
#include "modules/base/processors/geometry/geometryboundingbox.h"
#include "modules/base/processors/geometry/geometryprocessor.h"
#include "modules/base/processors/geometry/geometryrenderer.h"
#include "modules/base/processors/geometry/geometrysave.h"
#include "modules/base/processors/geometry/isosurfaceextractor.h"
#include "modules/base/processors/geometry/lightwidgetrenderer.h"
#include "modules/base/processors/geometry/meshclipping.h"
#include "modules/base/processors/geometry/meshclippingwidget.h"
#include "modules/base/processors/geometry/meshslabclipping.h"
#include "modules/base/processors/geometry/planewidgetprocessor.h"
#include "modules/base/processors/geometry/pointlistrenderer.h"
#include "modules/base/processors/geometry/pointsegmentlistrenderer.h"
#include "modules/base/processors/geometry/quadricrenderer.h"
#include "modules/base/processors/geometry/slicepositionrenderer.h"
*/

// image
#include "modules/base/processors/image/background.h"
#include "modules/base/processors/image/binaryimageprocessor.h"
#include "modules/base/processors/image/unaryimageprocessor.h"
#include "modules/base/processors/image/gaussian.h"
#include "modules/base/processors/image/median.h"
#include "modules/base/processors/image/mean.h"
#include "modules/base/processors/image/nonminmaxsuppression.h"
#include "modules/base/processors/image/canny.h"
#include "modules/base/processors/image/colordepth.h"
#include "modules/base/processors/image/compositor.h"
#include "modules/base/processors/image/convolution.h"
#include "modules/base/processors/image/depthdarkening.h"
#include "modules/base/processors/image/edgedetect.h"
#include "modules/base/processors/image/explosioncompositor.h"
#include "modules/base/processors/image/fade.h"
#include "modules/base/processors/image/gabor.h"
#include "modules/base/processors/image/grayscale.h"
#include "modules/base/processors/image/imagemasking.h"
#include "modules/base/processors/image/imagemorphology.h"
#include "modules/base/processors/image/imageoverlay.h"
#include "modules/base/processors/image/imagethreshold.h"
#include "modules/base/processors/image/labeling.h"
#include "modules/base/processors/image/orientationoverlay.h"
#include "modules/base/processors/image/regionofinterest2d.h"
#include "modules/base/processors/image/quadview.h"
#include "modules/base/processors/image/targettotexture.h"
#include "modules/base/processors/image/texturetotarget.h"
#include "modules/base/processors/image/unsharpmasking.h"

// proxy geometry
#include "modules/base/processors/proxygeometry/cubemeshproxygeometry.h"
#include "modules/base/processors/proxygeometry/explosionproxygeometry.h"
#include "modules/base/processors/proxygeometry/multiplanarproxygeometry.h"
#include "modules/base/processors/proxygeometry/multivolumeproxygeometry.h"


// render
#include "modules/base/processors/render/cpuraycaster.h"
#include "modules/base/processors/render/curvatureraycaster.h"
#include "modules/base/processors/render/dynamicglslprocessor.h"
#include "modules/base/processors/render/glslraycaster.h"
#include "modules/base/processors/render/halfangleslicer.h"
#include "modules/base/processors/render/idraycaster.h"
#include "modules/base/processors/render/multiplanarslicerenderer.h"
#include "modules/base/processors/render/rgbraycaster.h"
#include "modules/base/processors/render/segmentationraycaster.h"
#include "modules/base/processors/render/simpleraycaster.h"
#include "modules/base/processors/render/singlevolumeraycaster.h"
#include "modules/base/processors/render/multivolumeraycaster.h"
#include "modules/base/processors/render/singlevolumeslicer.h"
#include "modules/base/processors/render/sliceviewer.h"

// utility
#include "modules/base/processors/utility/brickloopimagecompositor.h"
#include "modules/base/processors/utility/brickloopinitiator.h"
#include "modules/base/processors/utility/brickloopvolumecompositor.h"
#include "modules/base/processors/utility/clockprocessor.h"
#include "modules/base/processors/utility/coordinatetransformation.h"
#include "modules/base/processors/utility/distancemeasure.h"
#include "modules/base/processors/utility/intensitymeasure.h"
#include "modules/base/processors/utility/imageselector.h"
#include "modules/base/processors/utility/imagesequenceloopinitiator.h"
#include "modules/base/processors/utility/imagesequenceloopfinalizer.h"
#include "modules/base/processors/utility/metadataextractor.h"
#include "modules/base/processors/utility/multiscale.h"
#include "modules/base/processors/utility/renderloopinitiator.h"
#include "modules/base/processors/utility/renderloopfinalizer.h"
#include "modules/base/processors/utility/renderstore.h"
#include "modules/base/processors/utility/scale.h"
#include "modules/base/processors/utility/segmentationvalidation.h"
#include "modules/base/processors/utility/textoverlay.h"
#include "modules/base/processors/utility/volumecollectionmodalityfilter.h"
#include "modules/base/processors/utility/volumeinformation.h"
#include "modules/base/processors/utility/volumepicking.h"
#include "modules/base/processors/utility/volumeregistration.h"
#include "modules/base/processors/utility/volumeselector.h"

// volume
#include "modules/base/processors/volume/vectormagnitude.h"
#include "modules/base/processors/volume/volumecollectionsave.h"
#include "modules/base/processors/volume/volumecrop.h"
#include "modules/base/processors/volume/volumedistancetransform.h"
#include "modules/base/processors/volume/volumeinversion.h"
#include "modules/base/processors/volume/volumecombine.h"
#include "modules/base/processors/volume/volumecomposer.h"
#include "modules/base/processors/volume/volumecurvature.h"
#include "modules/base/processors/volume/volumedecomposer.h"
#include "modules/base/processors/volume/volumefiltering.h"
#include "modules/base/processors/volume/volumeformatconversion.h"
#include "modules/base/processors/volume/volumegradient.h"
#include "modules/base/processors/volume/volumetransformation.h"
#include "modules/base/processors/volume/volumetranslation.h"
#include "modules/base/processors/volume/volumescaling.h"
#include "modules/base/processors/volume/volumemasking.h"
#include "modules/base/processors/volume/volumemorphology.h"
#include "modules/base/processors/volume/volumenormalization.h"
#include "modules/base/processors/volume/volumeresample.h"
#include "modules/base/processors/volume/volumesave.h"
#include "modules/base/processors/volume/volumecreate.h"
#include "modules/base/processors/volume/volumecubify.h"
#include "modules/base/processors/volume/volumehalfsample.h"
#include "modules/base/processors/volume/volumemirror.h"


///
/// volume readers/writers
///
#include "modules/base/io/analyzevolumereader.h"
#include "modules/base/io/brukervolumereader.h"
#include "modules/base/io/ecat7volumereader.h"
#include "modules/base/io/interfilevolumereader.h"
#include "modules/base/io/multivolumereader.h"
#include "modules/base/io/mhdvolumereader.h"
#include "modules/base/io/mhdvolumewriter.h"
#include "modules/base/io/nrrdvolumereader.h"
#include "modules/base/io/nrrdvolumewriter.h"
#include "modules/base/io/philipsusvolumereader.h"
#include "modules/base/io/quadhidacvolumereader.h"
#include "modules/base/io/synth2dreader.h"
#include "modules/base/io/rawvoxvolumereader.h"
#include "modules/base/io/tuvvolumereader.h"


namespace voreen {

const std::string BaseModule::loggerCat_("voreen.BaseModule");

BaseModule::BaseModule() : VoreenModule() {
    setName("Base");
    setXMLFileName("base/basemodule.xml");

    // data sources
    addProcessor(new GeometrySource());
    addProcessor(new ImageSequenceSource());
    addProcessor(new ImageSource());
    //addProcessor(new RawTextureSave());
    //addProcessor(new RawTextureSource());
    addProcessor(new TextSeriesSource);
    addProcessor(new TextSource());
    //addProcessor(new VolumeCollectionSource());
    //addProcessor(new VolumeSeriesSource());
    //addProcessor(new VolumeSource());

    // entry-exit points
    //addProcessor(new MeshEntryExitPoints());

    // geometry
    /*
    addProcessor(new BoundingBoxRenderer());
    addProcessor(new CameraPositionRenderer());
    addProcessor(new DepthPeelingProcessor());
    addProcessor(new EEPGeometryIntegrator());
    addProcessor(new GeometryBoundingBox);
    addProcessor(new GeometryProcessor());
    addProcessor(new GeometryRenderer());
    addProcessor(new GeometrySave());
    addProcessor(new IsosurfaceExtractor());
    addProcessor(new LightWidgetRenderer());
    addProcessor(new MeshClipping());
    addProcessor(new MeshClippingWidget());
    addProcessor(new MeshSlabClipping());
    addProcessor(new PlaneWidgetProcessor());
    addProcessor(new PointListRenderer());
    addProcessor(new PointSegmentListRenderer());
    addProcessor(new QuadricRenderer());
    addProcessor(new SlicePositionRenderer());
    */

    // image
    addProcessor(new Background());
    addProcessor(new BinaryImageProcessor());
    addProcessor(new Canny());
    addProcessor(new ColorDepth());
    addProcessor(new Compositor());
    addProcessor(new Convolution());
    addProcessor(new DepthDarkening());
    addProcessor(new EdgeDetect());
    addProcessor(new ExplosionCompositor());
    addProcessor(new Fade());
    addProcessor(new Gabor());
    addProcessor(new Gaussian());
    addProcessor(new Grayscale());
    addProcessor(new ImageMasking());
    addProcessor(new ImageMorphology());
    addProcessor(new ImageOverlay());
    addProcessor(new ImageThreshold());
    //addProcessor(new Labeling());
    addProcessor(new Mean());
    addProcessor(new Median());
    addProcessor(new NonMinMaxSuppression());
    addProcessor(new OrientationOverlay());
    addProcessor(new QuadView());
    addProcessor(new RegionOfInterest2D());
    addProcessor(new TargetToTexture);
    addProcessor(new TextureToTarget());
    addProcessor(new UnaryImageProcessor());
    addProcessor(new UnsharpMasking());

    // proxy geometry
    /*
    addProcessor(new CubeMeshProxyGeometry());
    addProcessor(new ExplosionProxyGeometry());
    addProcessor(new MultiPlanarProxyGeometry());
    addProcessor(new MultiVolumeProxyGeometry());
    */

    // render
    /*
    addProcessor(new CPURaycaster());
    addProcessor(new CurvatureRaycaster());
    addProcessor(new DynamicGLSLProcessor());
    addProcessor(new GLSLRaycaster());
    addProcessor(new HalfAngleSlicer());
    addProcessor(new IDRaycaster());
    addProcessor(new MultiplanarSliceRenderer());
    addProcessor(new MultiVolumeRaycaster());
    addProcessor(new SegmentationRaycaster());
    addProcessor(new SimpleRaycaster());
    addProcessor(new SingleVolumeRaycaster());
    addProcessor(new SingleVolumeSlicer());
    addProcessor(new SliceViewer());
    addProcessor(new RGBRaycaster());
    */

    // utility
    /*
    addProcessor(new BrickLoopImageCompositor());
    addProcessor(new BrickLoopInitiator());
    addProcessor(new BrickLoopVolumeCompositor());
    addProcessor(new ClockProcessor());
    addProcessor(new CoordinateTransformation());
    addProcessor(new DistanceMeasure());
    addProcessor(new IntensityMeasure());
    addProcessor(new ImageSequenceLoopInitiator());
    addProcessor(new ImageSequenceLoopFinalizer());
    addProcessor(new ImageSelector());
    addProcessor(new MetaDataExtractor());
    addProcessor(new MultiScale());
    addProcessor(new RenderLoopInitiator());
    addProcessor(new RenderLoopFinalizer());
    addProcessor(new RenderStore());
    addProcessor(new SegmentationValidation());
    addProcessor(new SingleScale());
    addProcessor(new TextOverlay());
    addProcessor(new VolumeCollectionModalityFilter());
    addProcessor(new VolumeInformation());
    addProcessor(new VolumePicking());
    addProcessor(new VolumeRegistration());
    addProcessor(new VolumeSelector());
    */

    // volume
    /*
    addProcessor(new VectorMagnitude());
    addProcessor(new VolumeCollectionSave());
    addProcessor(new VolumeCombine());
    addProcessor(new VolumeComposer());
    addProcessor(new VolumeCreate());
    addProcessor(new VolumeCrop());
    addProcessor(new VolumeCubify());
    addProcessor(new VolumeCurvature());
    addProcessor(new VolumeDecomposer());
    addProcessor(new VolumeDistanceTransform());
    addProcessor(new VolumeFiltering());
    addProcessor(new VolumeFormatConversion());
    addProcessor(new VolumeGradient());
    addProcessor(new VolumeHalfsample());
    addProcessor(new VolumeInversion());
    addProcessor(new VolumeMasking());
    addProcessor(new VolumeMirror());
    addProcessor(new VolumeMorphology());
    addProcessor(new VolumeNormalization);
    addProcessor(new VolumeResample());
    addProcessor(new VolumeSave());
    addProcessor(new VolumeScaling());
    addProcessor(new VolumeTransformation());
    addProcessor(new VolumeTranslation());
    */

    // volume readers/writers
    /*
    addVolumeReader(new AnalyzeVolumeReader());
    addVolumeReader(new BrukerVolumeReader());
    addVolumeReader(new ECAT7VolumeReader());
    addVolumeReader(new InterfileVolumeReader());
    addVolumeReader(new MultiVolumeReader(0));
    addVolumeReader(new MhdVolumeReader());
    addVolumeWriter(new MhdVolumeWriter());
    addVolumeReader(new NrrdVolumeReader());
    addVolumeWriter(new NrrdVolumeWriter());
    addVolumeReader(new PhilipsUSVolumeReader());
    addVolumeReader(new QuadHidacVolumeReader());
    addVolumeReader(new Synth2DReader());
    addVolumeReader(new RawVoxVolumeReader());
    addVolumeReader(new TUVVolumeReader());
    */

    // shader paths
    addShaderPath(getModulesPath("base/glsl"));

    // serialization factories
    addSerializerFactory(new RegionOfInterest2D::RegionOfInterestGeometryFactory());
}

void BaseModule::initialize() throw (tgt::Exception) {
    VoreenModule::initialize();
}

void BaseModule::deinitialize() throw (tgt::Exception) {
    VoreenModule::deinitialize();
}

} // namespace

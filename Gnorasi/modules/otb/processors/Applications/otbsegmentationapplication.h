/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef OTBSEGMENTATIONAPPLICATION_H
#define OTBSEGMENTATIONAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbimageport.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/floatproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"

// Writer
#include "otbImageFileWriter.h"

// Segmentation filters includes
#include "otbMeanShiftVectorImageFilter.h"
#include "otbMeanShiftSegmentationFilter.h"
#include "otbConnectedComponentMuParserFunctor.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbMaskMuParserFilter.h"
#include "otbVectorImageToAmplitudeImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "otbWatershedSegmentationFilter.h"
#include "otbMorphologicalProfilesSegmentationFilter.h"

// Large scale vectorization framework
#include "otbStreamingImageToOGRLayerSegmentationFilter.h"
#include "otbOGRDataSourceWrapper.h"

// Fusion filter
#include "otbOGRLayerStreamStitchingFilter.h"
#include "otbGeoInformationConversion.h"

// Utils
#include "itksys/SystemTools.hxx"

namespace voreen {

class OTBSegmentationApplication : public OTBImageFilterProcessor
{
public:
    OTBSegmentationApplication();

    virtual ~OTBSegmentationApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Segmentation Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;
    typedef otb::VectorImage<double, 2> VectorImageType;


    /** Images typedefs */
    typedef ImageType               LabelImageType;
    typedef ImageType               MaskImageType;

    typedef VectorImageType::SizeType size;

    // Embedded  Writer
    typedef otb::ImageFileWriter<ImageType> FileWriterType;
    FileWriterType::Pointer writer;

    // Segmentation filters typedefs
    // Edison mean-shift
    typedef otb::MeanShiftVectorImageFilter<VectorImageType,VectorImageType,LabelImageType> EdisonSegmentationFilterType;

    // Home made mean-shift
    typedef otb::MeanShiftSegmentationFilter<VectorImageType, LabelImageType, VectorImageType> MeanShiftSegmentationFilterType;

    // Simple connected components
    typedef otb::Functor::ConnectedComponentMuParserFunctor<VectorImageType::PixelType> FunctorType;

    typedef itk::ConnectedComponentFunctorImageFilter <VectorImageType, LabelImageType, FunctorType, MaskImageType> ConnectedComponentSegmentationFilterType;

    typedef itk::ScalarConnectedComponentImageFilter<LabelImageType, LabelImageType> LabeledConnectedComponentSegmentationFilterType;

    // Watershed
    typedef otb::VectorImageToAmplitudeImageFilter<VectorImageType,ImageType> AmplitudeFilterType;
    AmplitudeFilterType::Pointer amplitudeFilter;

    typedef itk::GradientMagnitudeImageFilter<ImageType,ImageType> GradientMagnitudeFilterType;
    GradientMagnitudeFilterType::Pointer gradientMagnitudeFilter;

    typedef otb::WatershedSegmentationFilter<ImageType,LabelImageType> WatershedSegmentationFilterType;

    // Geodesic morphology multiscale segmentation
    typedef otb::MorphologicalProfilesSegmentationFilter<ImageType,LabelImageType> MorphologicalProfilesSegmentationFilterType;

    // Vectorize filters
    // Edison mean-shift
    typedef otb::StreamingImageToOGRLayerSegmentationFilter<VectorImageType,EdisonSegmentationFilterType> EdisonStreamingVectorizedSegmentationOGRType;
    EdisonStreamingVectorizedSegmentationOGRType::Pointer edisonVectorizationFilter;

    // Home made mean-shift
    typedef otb::StreamingImageToOGRLayerSegmentationFilter<VectorImageType,MeanShiftSegmentationFilterType> MeanShiftVectorizedSegmentationOGRType;
    MeanShiftVectorizedSegmentationOGRType::Pointer meanShiftVectorizationFilter;

    // Connected components
    typedef otb::StreamingImageToOGRLayerSegmentationFilter<VectorImageType,ConnectedComponentSegmentationFilterType> ConnectedComponentStreamingVectorizedSegmentationOGRType;
    ConnectedComponentStreamingVectorizedSegmentationOGRType::Pointer ccVectorizationFilter;

    // Morphological profiles
    typedef otb::StreamingImageToOGRLayerSegmentationFilter<ImageType,MorphologicalProfilesSegmentationFilterType> MorphoVectorizedSegmentationOGRType;
    MorphoVectorizedSegmentationOGRType::Pointer morphoVectorizedSegmentation;

    typedef otb::OGRLayerStreamStitchingFilter<VectorImageType> FusionFilterType;
    FusionFilterType::Pointer fusionFilter;

    // Watershed
    typedef otb::StreamingImageToOGRLayerSegmentationFilter<ImageType,WatershedSegmentationFilterType> StreamingVectorizedWatershedFilterType;
    StreamingVectorizedWatershedFilterType::Pointer watershedVectorizedFilter;

protected:

    virtual void setDescriptions() {
        setDescription("Performs segmentation of an image, and output either a raster or a vector file. In vector mode, large input datasets are supported. In raster mode, output can be viewed using the outport.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual bool isReady() const;

    template<class TInputImage, class TSegmentationFilter>
    VectorImageType::SizeType  GenericApplySegmentation(otb::StreamingImageToOGRLayerSegmentationFilter
                                                        <TInputImage, TSegmentationFilter> * streamingVectorizedFilter, TInputImage * inputImage,
                                                        const otb::ogr::Layer& layer, const unsigned int outputNb);
    virtual void updateFilterSelection(); ///< Properties visibility configuration.
    virtual void updateModeSelection(); ///< Properties visibility configuration.
    virtual void writeImageToFile(ImageType *output); ///< In raster mode, writes labeled output to file.

private:

    OTBVectorImagePort inPort_;

    //FILTER
    StringOptionProperty filter_; ///< Select segmentation algorithm

    // MeanShift Parameters
    IntProperty meanshiftSpatialR_;
    FloatProperty meanshiftRangeR_;
    FloatProperty meanshiftThres_;
    IntProperty meanshiftMaxIter_;
    IntProperty meanshiftMinSize_;

    // EDISON Meanshift Parameters

    IntProperty edisonSpatialR_;
    FloatProperty edisonRangeR_;
    IntProperty edisonMinSize_;
    FloatProperty edisonScale_;

    //Connected component segmentation parameters
    StringProperty expr_;

    // Watershed
    FloatProperty watershedThreshold_;
    FloatProperty watershedLevel_;

    // GeoMorpho
    IntProperty mprofilesSize_;
    IntProperty mprofilesStart_;
    IntProperty mprofilesStep_;
    FloatProperty mprofilesSigma_;

    //MODE
    StringOptionProperty mode_; ///< Select processing mode

    //Vector mode
    StringOptionProperty vectorOutMode_;

    OTBImagePort vectorMaskInPort_;
    BoolProperty vectorNeighbor_;
    BoolProperty vectorStitch_;
    IntProperty vectorMinSize_;
    FloatProperty vectorSimplify_;
    StringProperty vectorLayerName_;
    StringProperty vectorFieldName_;
    IntProperty vectorTileSize_;
    IntProperty vectorStartLabel_;

    std::vector<std::string> options;
    StringProperty vectorOGROptions_;

    //Raster mode
    OTBImagePort outPort_;
    ImageType::Pointer imageOutput_;


    //Embedded Streaming Writer
    VectorImageType::Pointer vectorImageOutput_;
    FileDialogProperty imageFile_;  ///< Path of the saved image file.

    static const std::string loggerCat_; ///< Category used in logging
};


} // namespace

#endif // OTBSEGMENTATIONAPPLICATION_H

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

#include "otbsegmentationapplication.h"
#include "otbWrapperApplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBSegmentationApplication::loggerCat_("voreen.OTBSegmentationApplication");

OTBSegmentationApplication::OTBSegmentationApplication()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "IN Multiband Image", 0),
      vectorMaskInPort_(Port::INPORT, "IN Mask Image", 0),
      outPort_(Port::OUTPORT, "OUT Labeled Raster Image", 0),

      imageFile_("imageFile", "Set output filename", "Image/Database File", VoreenApplication::app()->getUserDataPath(), "All files (*.*)", FileDialogProperty::SAVE_FILE),

      filter_("selectFilter", "Segmentation algorithm"),

      meanshiftSpatialR_("meanshiftSpatialR_", "Spatial radius", 5, 0, 100),
      meanshiftRangeR_("meanshiftRangeR_", "Range radius", 15.0f, 0.0f, 100.0f),
      meanshiftThres_("meanshiftThres_", "Mode convergence threshold", 0.1f, 0.1f, 1.0f),
      meanshiftMaxIter_("meanshiftMaxIter_", "Maximum number of iterations", 100, 1, 100),
      meanshiftMinSize_("meanshiftMinSize_", "Minimum region size", 100, 0, 1000),

      edisonSpatialR_("edisonSpatialR_", "Spatial radius", 5, 0, 100),
      edisonRangeR_("edisonRangeR_", "Range radius", 15.0f, 0.0f, 100.0f),
      edisonMinSize_("edisonMinSize_", "Minimum region size", 100, 0, 1000),
      edisonScale_("edisonScale_", "Scale factor", 1, 0, 1),

      expr_("expr_", "Connected components condition", ""),

      watershedThreshold_("watershedThreshold_", "Depth Threshold", 0.01f, 0.0f, 1.0f),
      watershedLevel_("watershedLevel_", "Flood Level", 0.1f, 0.0f, 1.0f),

      mprofilesSize_("mprofilesSize_", "Profile Size", 5, 2, 10),
      mprofilesStart_("mprofilesStart_", "Initial radius", 1, 1, 10),
      mprofilesStep_("mprofilesStep_", "Radius step", 1, 1, 10),
      mprofilesSigma_("mprofilesSigma_", "Threshold of the final decision rule (sigma)", 1.0f, 0.0f, 10.0f),

      mode_("mode_", "Processing mode:"),

      vectorOutMode_("vectorOutMode_", "Writing mode for the output vector file:"),

      vectorNeighbor_("vectorNeighbor_", "8-neighbor connectivity", false),
      vectorStitch_("useAddColor_", "Stitch polygons", false),
      vectorMinSize_("vectorMinSize_", "Minimum object size", 1, 1, 10),
      vectorSimplify_("vectorSimplify_", "Simplify polygons", 0.1f, 0.1f, 1.0f),
      vectorLayerName_("vectorLayerName_", "Layer name", "layer"),
      vectorFieldName_("vectorFieldName_", "Geometry index field name", "DN"),
      vectorTileSize_("vectorTileSize_", "Tiles size", 1024, 0, 3000),
      vectorStartLabel_("vectorStartLabel_", "Starting geometry index", 1, 1, 3000),
      vectorOGROptions_("ogrOptions_", "OGR options for layer creation (KEY=VALUE)", "")

{

    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(vectorMaskInPort_);
    addPort(outPort_);

    addProperty(filter_);

    filter_.addOption("blank", "Select filter:");
    filter_.addOption("meanshift", "Mean-Shift");
    filter_.addOption("edison", "Edison mean-shift");
    filter_.addOption("cc", "Connected components");
    filter_.addOption("watershed", "Watershed");
    filter_.addOption("mprofiles", "Morphological profiles based segmentation");

    filter_.onChange(CallMemberAction<OTBSegmentationApplication>
                     (this,&OTBSegmentationApplication::updateFilterSelection));

    addProperty(meanshiftSpatialR_);
    addProperty(meanshiftRangeR_);
    addProperty(meanshiftThres_);
    addProperty(meanshiftMaxIter_);
    addProperty(meanshiftMinSize_);
    meanshiftSpatialR_.setVisible(false);
    meanshiftRangeR_.setVisible(false);
    meanshiftThres_.setVisible(false);
    meanshiftMaxIter_.setVisible(false);
    meanshiftMinSize_.setVisible(false);

    addProperty(edisonSpatialR_);
    addProperty(edisonRangeR_);
    addProperty(edisonMinSize_);
    addProperty(edisonScale_);
    edisonSpatialR_.setVisible(false);
    edisonRangeR_.setVisible(false);
    edisonMinSize_.setVisible(false);
    edisonScale_.setVisible(false);

    addProperty(expr_);
    expr_.setVisible(false);

    addProperty(watershedThreshold_);
    addProperty(watershedLevel_);
    watershedThreshold_.setVisible(false);
    watershedLevel_.setVisible(false);

    addProperty(mprofilesSize_);
    addProperty(mprofilesStart_);
    addProperty(mprofilesStep_);
    addProperty(mprofilesSigma_);
    mprofilesSize_.setVisible(false);
    mprofilesStart_.setVisible(false);
    mprofilesStep_.setVisible(false);
    mprofilesSigma_.setVisible(false);

    addProperty(mode_);

    mode_.addOption("blank", "Select mode:");
    mode_.addOption("vector", "Tile-based large-scale segmentation with vector output");
    mode_.addOption("raster", "Standard segmentation with labeled raster output");

    mode_.onChange(CallMemberAction<OTBSegmentationApplication>
                   (this,&OTBSegmentationApplication::updateModeSelection));

    addProperty(vectorOutMode_);
    vectorOutMode_.setVisible(false);

    vectorOutMode_.addOption("ulco", "Update output vector file, only allow to create new layers ");
    vectorOutMode_.addOption("ovw", "Overwrite output vector file if existing");
    vectorOutMode_.addOption("ulovw", "Update output vector file, overwrite existing layer");
    vectorOutMode_.addOption("ulu", "Update output vector file, update existing layer");

    addProperty(vectorNeighbor_);
    addProperty(vectorStitch_);
    addProperty(vectorMinSize_);
    addProperty(vectorSimplify_);
    addProperty(vectorLayerName_);
    addProperty(vectorFieldName_);
    vectorNeighbor_.setVisible(false);
    vectorStitch_.setVisible(false);
    vectorMinSize_.setVisible(false);
    vectorSimplify_.setVisible(false);
    vectorLayerName_.setVisible(false);
    vectorFieldName_.setVisible(false);

    addProperty(vectorTileSize_);
    addProperty(vectorStartLabel_);
    addProperty(vectorOGROptions_);
    vectorTileSize_.setVisible(false);
    vectorStartLabel_.setVisible(false);
    vectorOGROptions_.setVisible(false);

    addProperty(imageFile_);

}

Processor* OTBSegmentationApplication::create() const {
    return new OTBSegmentationApplication();
}

OTBSegmentationApplication::~OTBSegmentationApplication() {

}

void OTBSegmentationApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBSegmentationApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBSegmentationApplication::getProcessorInfo() const {
    return "Segmentation Application";
}

void OTBSegmentationApplication::updateFilterSelection() {

    if (filter_.get()=="blank") {
        meanshiftSpatialR_.setVisible(false);
        meanshiftRangeR_.setVisible(false);
        meanshiftThres_.setVisible(false);
        meanshiftMaxIter_.setVisible(false);
        meanshiftMinSize_.setVisible(false);

        edisonSpatialR_.setVisible(false);
        edisonRangeR_.setVisible(false);
        edisonMinSize_.setVisible(false);
        edisonScale_.setVisible(false);

        expr_.setVisible(false);

        watershedThreshold_.setVisible(false);
        watershedLevel_.setVisible(false);

        mprofilesSize_.setVisible(false);
        mprofilesStart_.setVisible(false);
        mprofilesStep_.setVisible(false);
        mprofilesSigma_.setVisible(false);

    } else if (filter_.get()=="meanshift") {
        meanshiftSpatialR_.setVisible(true);
        meanshiftRangeR_.setVisible(true);
        meanshiftThres_.setVisible(true);
        meanshiftMaxIter_.setVisible(true);
        meanshiftMinSize_.setVisible(true);

        edisonSpatialR_.setVisible(false);
        edisonRangeR_.setVisible(false);
        edisonMinSize_.setVisible(false);
        edisonScale_.setVisible(false);

        expr_.setVisible(false);

        watershedThreshold_.setVisible(false);
        watershedLevel_.setVisible(false);

        mprofilesSize_.setVisible(false);
        mprofilesStart_.setVisible(false);
        mprofilesStep_.setVisible(false);
        mprofilesSigma_.setVisible(false);


    } else if (filter_.get()=="edison") {
        meanshiftSpatialR_.setVisible(false);
        meanshiftRangeR_.setVisible(false);
        meanshiftThres_.setVisible(false);
        meanshiftMaxIter_.setVisible(false);
        meanshiftMinSize_.setVisible(false);

        edisonSpatialR_.setVisible(true);
        edisonRangeR_.setVisible(true);
        edisonMinSize_.setVisible(true);
        edisonScale_.setVisible(true);

        expr_.setVisible(false);

        watershedThreshold_.setVisible(false);
        watershedLevel_.setVisible(false);

        mprofilesSize_.setVisible(false);
        mprofilesStart_.setVisible(false);
        mprofilesStep_.setVisible(false);
        mprofilesSigma_.setVisible(false);

    } else if(filter_.get()=="cc") {
        meanshiftSpatialR_.setVisible(false);
        meanshiftRangeR_.setVisible(false);
        meanshiftThres_.setVisible(false);
        meanshiftMaxIter_.setVisible(false);
        meanshiftMinSize_.setVisible(false);

        edisonSpatialR_.setVisible(false);
        edisonRangeR_.setVisible(false);
        edisonMinSize_.setVisible(false);
        edisonScale_.setVisible(false);

        expr_.setVisible(true);

        watershedThreshold_.setVisible(false);
        watershedLevel_.setVisible(false);

        mprofilesSize_.setVisible(false);
        mprofilesStart_.setVisible(false);
        mprofilesStep_.setVisible(false);
        mprofilesSigma_.setVisible(false);

    } else if(filter_.get()=="watershed") {
        meanshiftSpatialR_.setVisible(false);
        meanshiftRangeR_.setVisible(false);
        meanshiftThres_.setVisible(false);
        meanshiftMaxIter_.setVisible(false);
        meanshiftMinSize_.setVisible(false);

        edisonSpatialR_.setVisible(false);
        edisonRangeR_.setVisible(false);
        edisonMinSize_.setVisible(false);
        edisonScale_.setVisible(false);

        expr_.setVisible(false);

        watershedThreshold_.setVisible(true);
        watershedLevel_.setVisible(true);

        mprofilesSize_.setVisible(false);
        mprofilesStart_.setVisible(false);
        mprofilesStep_.setVisible(false);
        mprofilesSigma_.setVisible(false);

    } else if(filter_.get()=="mprofiles") {
        meanshiftSpatialR_.setVisible(false);
        meanshiftRangeR_.setVisible(false);
        meanshiftThres_.setVisible(false);
        meanshiftMaxIter_.setVisible(false);
        meanshiftMinSize_.setVisible(false);

        edisonSpatialR_.setVisible(false);
        edisonRangeR_.setVisible(false);
        edisonMinSize_.setVisible(false);
        edisonScale_.setVisible(false);

        expr_.setVisible(false);

        watershedThreshold_.setVisible(false);
        watershedLevel_.setVisible(false);

        mprofilesSize_.setVisible(true);
        mprofilesStart_.setVisible(true);
        mprofilesStep_.setVisible(true);
        mprofilesSigma_.setVisible(true);
    }

}

void OTBSegmentationApplication::updateModeSelection() {

    if (mode_.get()=="blank" || mode_.get()=="raster") {
        vectorOutMode_.setVisible(false);
        vectorNeighbor_.setVisible(false);
        vectorStitch_.setVisible(false);
        vectorMinSize_.setVisible(false);
        vectorSimplify_.setVisible(false);
        vectorLayerName_.setVisible(false);
        vectorFieldName_.setVisible(false);
        vectorTileSize_.setVisible(false);
        vectorStartLabel_.setVisible(false);
        vectorOGROptions_.setVisible(false);

    } else if (mode_.get()=="vector") {
        vectorOutMode_.setVisible(true);
        vectorNeighbor_.setVisible(true);
        vectorStitch_.setVisible(true);
        vectorMinSize_.setVisible(true);
        vectorSimplify_.setVisible(true);
        vectorLayerName_.setVisible(true);
        vectorFieldName_.setVisible(true);
        vectorTileSize_.setVisible(true);
        vectorStartLabel_.setVisible(true);
        vectorOGROptions_.setVisible(true);

    }
}

template<class TInputImage, class TSegmentationFilter>
OTBSegmentationApplication::VectorImageType::SizeType
OTBSegmentationApplication::GenericApplySegmentation(otb::StreamingImageToOGRLayerSegmentationFilter<TInputImage,
                                                     TSegmentationFilter> * streamingVectorizedFilter, TInputImage * inputImage, const otb::ogr::Layer& layer, const unsigned int outputNb)
{
    typedef  TSegmentationFilter SegmentationFilterType;
    typedef  typename SegmentationFilterType::Pointer SegmentationFilterPointerType;
    typedef otb::StreamingImageToOGRLayerSegmentationFilter <TInputImage, SegmentationFilterType> StreamingVectorizedSegmentationOGRType;

    // Retrieve tile size parameter
    const unsigned int tileSize = vectorTileSize_.get();
    // Retrieve the 8-connected option
    bool use8connected = vectorNeighbor_.get();
    // Retrieve min object size parameter
    const unsigned int minSize = vectorMinSize_.get();
    //     Switch on segmentation mode
    const std::string segModeType = mode_.get();

    streamingVectorizedFilter->SetInput(inputImage);

    if (segModeType == "vector" && vectorMaskInPort_.isConnected())
    {
        streamingVectorizedFilter->SetInputMask(vectorMaskInPort_.getData());
        LINFO("Use a mask as input.");
    }
    streamingVectorizedFilter->SetOGRLayer(layer);

    if (tileSize != 0)
    {
        streamingVectorizedFilter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
    }
    else
    {
        streamingVectorizedFilter->GetStreamer()->SetAutomaticTiledStreaming();
    }

    if (use8connected)
    {
        LINFO("Use 8 connected neighborhood.");
    }
    streamingVectorizedFilter->SetUse8Connected(use8connected);

    if (minSize > 1)
    {
        LINFO("Object with size under "<< minSize <<" will be suppressed.");
        streamingVectorizedFilter->SetFilterSmallObject(true);
        streamingVectorizedFilter->SetMinimumObjectSize(minSize);
    }
    const std::string fieldName = vectorFieldName_.get();

    // Retrieve start label parameter
    const unsigned int startLabel = vectorStartLabel_.get();

    streamingVectorizedFilter->SetFieldName(fieldName);
    streamingVectorizedFilter->SetStartLabel(startLabel);

    if (vectorSimplify_.get() && segModeType == "vector")
    {
        streamingVectorizedFilter->SetSimplify(true);
        streamingVectorizedFilter->SetSimplificationTolerance(vectorSimplify_.get());
        LINFO("Simplify the geometry.");
    }
    else
    {
        streamingVectorizedFilter->SetSimplify(false);
    }

    if (segModeType == "vector")
    {
        LINFO("Large scale segmentation mode which output vector data");

        streamingVectorizedFilter->GetStreamer();

        streamingVectorizedFilter->Initialize(); //must be called !
        streamingVectorizedFilter->Update(); //must be called !
    }

    if (segModeType == "raster")
    {
        LINFO("Segmentation mode which output label image");

        streamingVectorizedFilter->GetSegmentationFilter()->SetInput(inputImage);
        streamingVectorizedFilter->GetSegmentationFilter()->Update();

        if (dynamic_cast<ImageType*>(streamingVectorizedFilter->GetSegmentationFilter()->GetOutputs().at(outputNb).GetPointer())) {
            imageOutput_=dynamic_cast<ImageType *>(streamingVectorizedFilter->GetSegmentationFilter()->GetOutputs().at(outputNb).GetPointer());
            writeImageToFile(imageOutput_);
        }
    }

    return streamingVectorizedFilter->GetStreamSize();
}

bool OTBSegmentationApplication::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected() && !vectorMaskInPort_.isConnected()) return false;

    if(inPort_.isConnected() && !vectorMaskInPort_.isConnected()) return true;

    if (mode_.get() == "raster") {
        if(!outPort_.isConnected()) return false; ///< In raster mode outport should be connected for pipeline usage
    }

    return true;
}


void OTBSegmentationApplication::writeImageToFile(ImageType *output) {

    writer = FileWriterType::New();
    writer->SetFileName(imageFile_.get());
    writer->SetInput(output);
    writer->WriteGeomFileOn();
    try
    {
        writer->Update();
    }
    catch (itk::ExceptionObject& err)
    {
        LWARNING("Output labeled image cannot be written to file!" << err);
        return;
    }

    outPort_.setData(output);
}

void OTBSegmentationApplication::process() {

    // Switch on segmentation mode
    const std::string segModeType = mode_.get();
    // Switch on segmentation filter case
    const std::string segType = filter_.get();

    otb::ogr::DataSource::Pointer ogrDS;
    otb::ogr::Layer layer(NULL, false);

    std::string projRef = inPort_.getData()->GetProjectionRef();

    OGRSpatialReference oSRS(projRef.c_str());

    if (segModeType == "vector")
    {

        // Retrieve output filename as well as layer names
        std::string dataSourceName = imageFile_.get();
        LINFO("Data source filename: " << dataSourceName);

        //projection ref conversion to ESRI need to be tested in case of .shp
        if ((dataSourceName.find(".shp") != string::npos) && (!projRef.empty()))
        {
            if (!(otb::GeoInformationConversion::IsESRIValidWKT(projRef)))
            {
                LERROR("Image projection reference "<< projRef);
                LERROR("Image spatial reference can't be converted to ESRI. Use another output format (kml,SQLite,...) to overcome .shp limitation");
            }
        }

        // Retrieve the output vector opening mode
        std::string outmode = vectorOutMode_.get();


        try
        {
            // Retrieve OGRoptions from string and pass them to vector
            std::string list = vectorOGROptions_.get();
            std::istringstream f(list);
            std::string s;

            LINFO("List of layer creation options that passed to OGR without validity checking:");
            while (std::getline(f, s, ' '))
                LINFO(s);
            options.push_back(s);
        }
        catch (int e)
        {
            LERROR("Error converting OGRoptions list");
            return;
        }

        try
        {
            // Create the DataSource in the appropriate mode
            if (outmode == "ovw")
            {
                // Create the datasource
                ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Overwrite);

                // and create the layer since we are in overwrite mode, the
                // datasource is blank
                layer = ogrDS->CreateLayer(vectorLayerName_.get(), &oSRS, wkbMultiPolygon, options);
                // And create the field
                OGRFieldDefn field(vectorFieldName_.get().c_str(), OFTInteger);
                layer.CreateField(field, true);
            }
            else
                if (outmode == "ulovw")
                {
                    // Create the datasource
                    ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerOverwrite);

                    // and create the layer since we are in overwrite mode, the
                    // datasource is blank
                    layer = ogrDS->CreateLayer(vectorLayerName_.get(), &oSRS, wkbMultiPolygon, options);
                    // And create the field
                    OGRFieldDefn field(vectorFieldName_.get().c_str(), OFTInteger);
                    layer.CreateField(field, true);
                }
                else
                    if (outmode == "ulu")
                    {
                        // Create the datasource
                        ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerUpdate);
                        // and create the layer since we are in overwrite mode, the
                        // datasource is blank
                        layer = ogrDS->CreateLayer(vectorLayerName_.get(), &oSRS, wkbMultiPolygon, options);

                        // And create the field if necessary
                        std::string fieldName = vectorFieldName_.get();
                        OGRFeatureDefn & ogrFeatureDfn = layer.GetLayerDefn();

                        if (-1 == ogrFeatureDfn.GetFieldIndex(fieldName.c_str()))
                        {
                            OGRFieldDefn field(fieldName.c_str(), OFTInteger);
                            layer.CreateField(field, true);
                        }

                    }
                    else
                        if (outmode == "ulco")
                        {
                            // Create the datasource
                            ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerCreateOnly);

                            // and create the layer since we are in overwrite mode, the
                            // datasource is blank
                            layer = ogrDS->CreateLayer(vectorLayerName_.get(), &oSRS, wkbMultiPolygon, options);
                            // And create the field
                            OGRFieldDefn field(vectorFieldName_.get().c_str(), OFTInteger);
                            layer.CreateField(field, true);
                        }
                        else
                        {
                            LERROR("outmode not handled yet: "<< outmode);
                        }
        }
        catch (int e)
        {
            LERROR("Error creating the DataSource in the appropriate mode!");
            return;
        }
    }

    // The actual stream size used
    VectorImageType::SizeType streamSize;

    try
    {
        if (segType == "cc")
        {

            LINFO("Use connected component segmentation.");
            ccVectorizationFilter = ConnectedComponentStreamingVectorizedSegmentationOGRType::New();

            if (vectorMaskInPort_.isConnected() && vectorMaskInPort_.hasData())
            {
                ccVectorizationFilter->GetSegmentationFilter()->SetMaskImage(vectorMaskInPort_.getData());
            }
            ccVectorizationFilter->GetSegmentationFilter()->GetFunctor().SetExpression(expr_.get());

            streamSize = GenericApplySegmentation<VectorImageType, ConnectedComponentSegmentationFilterType> (
                        ccVectorizationFilter,inPort_.getData(),
                        layer, 0);
        }
        else
            if (segType == "edison")
            {
                LINFO("Use Edison Mean-shift segmentation.");

                //segmentation parameters
                const unsigned int spatialRadius = edisonSpatialR_.get();
                const float rangeRadius = edisonRangeR_.get();
                const unsigned int minimumObjectSize = edisonMinSize_.get();
                const float scale = edisonScale_.get();

                edisonVectorizationFilter = EdisonStreamingVectorizedSegmentationOGRType::New();

                edisonVectorizationFilter->GetSegmentationFilter()->SetSpatialRadius(spatialRadius);
                edisonVectorizationFilter->GetSegmentationFilter()->SetRangeRadius(rangeRadius);
                edisonVectorizationFilter->GetSegmentationFilter()->SetMinimumRegionSize(minimumObjectSize);
                edisonVectorizationFilter->GetSegmentationFilter()->SetScale(scale);

                streamSize = GenericApplySegmentation<VectorImageType, EdisonSegmentationFilterType> (
                            edisonVectorizationFilter,inPort_.getData(),
                            layer, 2);
            }
            else
                if (segType == "meanshift")
                {
                    LINFO("Use threaded Mean-shift segmentation.");

                    meanShiftVectorizationFilter = MeanShiftVectorizedSegmentationOGRType::New();

                    //segmentation parameters
                    const unsigned int
                            spatialRadius = meanshiftSpatialR_.get();
                    const float rangeRadius = meanshiftRangeR_.get();
                    const unsigned int minimumObjectSize = meanshiftMinSize_.get();

                    const float threshold = meanshiftThres_.get();
                    const unsigned int maxIterNumber = meanshiftMaxIter_.get();

                    meanShiftVectorizationFilter->GetSegmentationFilter()->SetSpatialBandwidth(spatialRadius);
                    meanShiftVectorizationFilter->GetSegmentationFilter()->SetRangeBandwidth(rangeRadius);
                    meanShiftVectorizationFilter->GetSegmentationFilter()->SetMaxIterationNumber(maxIterNumber);
                    meanShiftVectorizationFilter->GetSegmentationFilter()->SetThreshold(threshold);
                    meanShiftVectorizationFilter->GetSegmentationFilter()->SetMinRegionSize(minimumObjectSize);

                    streamSize = GenericApplySegmentation<VectorImageType, MeanShiftSegmentationFilterType> (
                                meanShiftVectorizationFilter,inPort_.getData(),
                                layer, 0);
                }
                else
                    if (segType == "watershed")
                    {
                        LINFO("Using watershed segmentation.");

                        amplitudeFilter = AmplitudeFilterType::New();

                        amplitudeFilter->SetInput(inPort_.getData());

                        gradientMagnitudeFilter = GradientMagnitudeFilterType::New();
                        gradientMagnitudeFilter->SetInput(amplitudeFilter->GetOutput());

                        watershedVectorizedFilter = StreamingVectorizedWatershedFilterType::New();

                        watershedVectorizedFilter->GetSegmentationFilter()->SetThreshold(watershedThreshold_.get());
                        watershedVectorizedFilter->GetSegmentationFilter()->SetLevel(watershedLevel_.get());

                        streamSize = GenericApplySegmentation<ImageType, WatershedSegmentationFilterType> (
                                    watershedVectorizedFilter,
                                    gradientMagnitudeFilter->GetOutput(),
                                    layer, 0);
                    }

                    else
                        if (segType == "mprofiles")
                        {
                            LINFO("Using multiscale geodesic morphology segmentation.");

                            unsigned int profileSize = mprofilesSize_.get();
                            unsigned int initialValue = mprofilesStart_.get();
                            unsigned int step = mprofilesStep_.get();
                            double       sigma = mprofilesSize_.get();


                            AmplitudeFilterType::Pointer amplitudeFilter = AmplitudeFilterType::New();

                            amplitudeFilter->SetInput(inPort_.getData());

                            morphoVectorizedSegmentation = MorphoVectorizedSegmentationOGRType::New();
                            morphoVectorizedSegmentation->GetSegmentationFilter()->SetProfileStart(initialValue);
                            morphoVectorizedSegmentation->GetSegmentationFilter()->SetProfileSize(profileSize);
                            morphoVectorizedSegmentation->GetSegmentationFilter()->SetProfileStep(step);
                            morphoVectorizedSegmentation->GetSegmentationFilter()->SetSigma(sigma);

                            streamSize = GenericApplySegmentation<ImageType, MorphologicalProfilesSegmentationFilterType> (
                                        morphoVectorizedSegmentation,
                                        amplitudeFilter->GetOutput(),
                                        layer, 0);
                        }
                        else
                        {
                            LERROR("Non defined filtering method!");
                        }
    }
    catch (int e)
    {
        LERROR("Error while using " << filter_.get());
        return;
    }

    if (segModeType == "vector")
    {
        LINFO("Stream size: " << streamSize);
        try
        {
            ogrDS->SyncToDisk();
        }
        catch (int e)
        {
            LERROR("Error synchronizing to disk!");
            return;
        }

        try
        {
            // Stitching mode
            if (vectorStitch_.get())
            {
                LINFO("Segmentation done, stiching polygons ...");

                fusionFilter = FusionFilterType::New();
                fusionFilter->SetInput(inPort_.getData());
                fusionFilter->SetOGRLayer(layer);
                fusionFilter->SetStreamSize(streamSize);

                //             AddProcess(fusionFilter, "Stitching polygons");
                fusionFilter->GenerateData();

                std::string driverName(ogrDS->ogr().GetDriver()->GetName());
                if ( driverName.find("ESRI Shapefile") != std::string::npos)
                {
                    LINFO("REPACK the Shapefile ...");
                    //In Shapefile format, the name of the DaaSource is also the name of the Layer.
                    std::string shpLayerName = itksys::SystemTools::GetFilenameWithoutExtension(imageFile_.get());

                    std::string repack("REPACK ");
                    repack = repack + shpLayerName;
                    ogrDS->ExecuteSQL(repack, NULL, NULL);
                }
            }
        }
        catch (int e)
        {
            LERROR("Error while stitching polygons!");
            return;
        }
    }
    LINFO("Segmentation Application Executed Successfully!");

}

}   // namespace

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.             	*
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

#include "otbgenericroadextractionfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBGenericRoadExtractionFilterProcessor::loggerCat_("voreen.OTBGenericRoadExtractionFilterProcessor");

OTBGenericRoadExtractionFilterProcessor::OTBGenericRoadExtractionFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    alpha_("alpha_", "Alpha:",1.0f),
    amplitudeThreshold_("amplitudeThreshold_", "Amplitude Threshold: ",0.00005f),
    tolerance_("tolerance_", "Tolerance: ",1.0f),
    maxAngle_("maxAngle_", "Max Angle/Angular Threshold: ", 0.39269f),
    firstMeanDistanceThreshold_("firstMeanDistanceThreshold_", "First Mean Distance Threshold: ",1.0f, 0.0f, 10.0f),
    secondMeanDistanceThreshold_("secondMeanDistanceThreshold_", "Second Mean Distance Threshold: ",10.0f, 0.0f, 10.0f),
    distanceThreshold_("distanceThreshold_", "Distance Threshold: ",25.0f, 0.0f, 100.0f)

{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(enableSwitch_);
    addProperty(alpha_);
    addProperty(amplitudeThreshold_);
    addProperty(tolerance_);
    addProperty(maxAngle_);
    addProperty(firstMeanDistanceThreshold_);
    addProperty(secondMeanDistanceThreshold_);
    addProperty(distanceThreshold_);

    roadextractor = GenericRoadExtractionFilterType::New();
    drawingFilter = DrawPathFilterType::New();
}

void OTBGenericRoadExtractionFilterProcessor::update(){
    process();
}

Processor* OTBGenericRoadExtractionFilterProcessor::create() const {
  return new OTBGenericRoadExtractionFilterProcessor();
}

OTBGenericRoadExtractionFilterProcessor::~OTBGenericRoadExtractionFilterProcessor() {

}

void OTBGenericRoadExtractionFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBGenericRoadExtractionFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBGenericRoadExtractionFilterProcessor::getProcessorInfo() const {

    return "Performs a generic extraction of roads from an image";
}

void OTBGenericRoadExtractionFilterProcessor::process() {

    //Check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        LINFO("Generic Road Extraction Filter is disabled!");
        return;
    }

    try
    {
        roadextractor->SetInput(inPort_.getData());

        //Setting the GenericRoadExtraction properties
        roadextractor->SetAlpha(alpha_.get());
        roadextractor->SetAmplitudeThreshold(amplitudeThreshold_.get());
        roadextractor->SetTolerance(tolerance_.get());
        roadextractor->SetMaxAngle(maxAngle_.get());
        roadextractor->SetAngularThreshold(maxAngle_.get());
        roadextractor->SetFirstMeanDistanceThreshold(firstMeanDistanceThreshold_.get());
        roadextractor->SetSecondMeanDistanceThreshold(secondMeanDistanceThreshold_.get());
        roadextractor->SetDistanceThreshold(distanceThreshold_.get());

        //Setting up new image background
        ImageType::Pointer blackBackground = ImageType::New();
        blackBackground->SetRegions(inPort_.getData()->GetLargestPossibleRegion());
        blackBackground->Allocate();
        blackBackground->FillBuffer(0);

        //Drawing the path
        drawingFilter->SetInput(blackBackground);
        drawingFilter->SetInputPath(roadextractor->GetOutput());
        drawingFilter->UseInternalPathValueOn();
        drawingFilter->Update();

        outPort_.setData(drawingFilter->GetOutput());
        LINFO("Generic Road Extraction Filter Connected!");

    }
    catch (int e)
    {
    LERROR("Problem with Generic Road Extraction Filter!");
    return;
    }

}

} // namespace

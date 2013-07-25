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

#include "otbwatershedsegmentationfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBWatershedSegmentationFilterProcessor::loggerCat_("voreen.OTBWatershedSegmentationFilterProcessor");

OTBWatershedSegmentationFilterProcessor::OTBWatershedSegmentationFilterProcessor()
    :OTBImageFilterProcessor(),
      numberOfIterations_("numberOfIterations", "Iterations", 4, 1, 20),
      timeStep_("timeStep", "Time Step", 0.125f, 0.0f, 10.0f),
      conductance_("conductance", "Conductance", 2.0f, 0.0f, 100.0f),
      gradientMode_("gradientMode_", "Use Principle Components", true),
      waterShedLevel_("waterShedLevel", "Level", 0.05f, 0.0f, 1.0f),
      waterShedThreshold_("waterShedThreshold", "Threshold", 0.05f, 0.0f, 1.0f),
      vectorInPort_(Port::INPORT, "IN Multiband Image", 0),
      labelOutPort_(Port::OUTPORT, "Label Image port", 0),
      mapOutPort_(Port::OUTPORT, "Label Map port", 0),
      rgbOutPort_(Port::OUTPORT, "RGB Image port", 0),
      outPort_(Port::OUTPORT, "OTB Image port", 0)

{
//    addProperty(enableSwitch_);   //no vector outport available
    addProperty(numberOfIterations_);
    addProperty(timeStep_);
    addProperty(conductance_);
    addProperty(gradientMode_);
    addProperty(waterShedLevel_);
    addProperty(waterShedThreshold_);

    addPort(vectorInPort_);
    addPort(labelOutPort_);
    addPort(mapOutPort_);
//    addPort(rgbOutPort_);
    addPort(outPort_);

    diffusion = DiffusionFilterType::New();
    gradient = GradientMagnitudeFilterType::New();
    watershedFilter = WatershedFilterType::New();

    labelCaster = LabelImageCastFilterType::New();
    vectorCaster = VectorCastFilterType::New();
    otbCaster = ImageCastFilterType::New();

    labelMapFilter = LabelMapFilterType::New();
    colormapper = ColorMapFilterType::New();
}

Processor* OTBWatershedSegmentationFilterProcessor::create() const {
return new OTBWatershedSegmentationFilterProcessor();
}

OTBWatershedSegmentationFilterProcessor::~OTBWatershedSegmentationFilterProcessor() {

}

void OTBWatershedSegmentationFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBWatershedSegmentationFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBWatershedSegmentationFilterProcessor::getProcessorInfo() const {

  return "OTBWatershedSegmentationFilter Processor";
}

bool OTBWatershedSegmentationFilterProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!vectorInPort_.isConnected()) return false;

    if(!labelOutPort_.isConnected() && !mapOutPort_.isConnected() && !rgbOutPort_.isConnected() && !outPort_.isConnected()) return false;

    return true;
}

void OTBWatershedSegmentationFilterProcessor::process() {
    //check bypass switch
//    if (!enableSwitch_.get()){
//        bypass(&inPort_, &outPort_);
//        return;
//    }

    try
    {
        vectorCaster->SetInput(vectorInPort_.getData());
        diffusion->SetInput(vectorCaster->GetOutput());

        diffusion->SetNumberOfIterations(numberOfIterations_.get());
        diffusion->SetConductanceParameter(conductance_.get());
        diffusion->SetTimeStep(timeStep_.get());

        gradient->SetInput(diffusion->GetOutput());

        gradient->SetUsePrincipleComponents(gradientMode_.get());

        watershedFilter->SetInput(gradient->GetOutput());

        watershedFilter->SetLevel(waterShedLevel_.get());
        watershedFilter->SetThreshold(waterShedThreshold_.get());
        watershedFilter->Update();

        //*RGBcolormap Output*
        colormapper->SetInput(watershedFilter->GetOutput());
        /// TODO: RGBImagePort needed
//        rgbOutPort_.setData(colormapper->GetOutput());

        //LabelImage Output
        labelCaster->SetInput(watershedFilter->GetOutput());
        labelOutPort_.setData(labelCaster->GetOutput());

        //LabelMap Output
        labelMapFilter->SetInput(watershedFilter->GetOutput());
        labelMapFilter->SetBackgroundValue(itk::NumericTraits<unsigned long>::min());
        labelMapFilter->Update();

        //OTBImage Output
        otbCaster->SetInput(watershedFilter->GetOutput());
        outPort_.setData(otbCaster->GetOutput());

        mapOutPort_.setData(labelMapFilter->GetOutput());

    }
    catch (int e)
    {
        LERROR("Problem with Watershed Segmentation Filter!");
    return;
    }
}

} //namespace



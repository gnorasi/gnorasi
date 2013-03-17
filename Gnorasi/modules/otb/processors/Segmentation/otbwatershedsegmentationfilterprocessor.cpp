/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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
    waterShedLevel_("waterShedLevel", "Level", 0.005f, 0.0f, 1.0f),
    waterShedThreshold_("waterShedThreshold", "Threshold", 0.05f, 0.0f, 1.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(waterShedLevel_);
    addProperty(waterShedThreshold_);

    addPort(inPort_);
    addPort(outPort_);

    waterShedFilter = WatershedFilterType::New();
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

void OTBWatershedSegmentationFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        /*
         * There are two parameters. Level controls watershed depth, and Threshold controls the lower thresholding of the input.
         * Both parameters are set as a percentage (0.0 - 1.0) of the maximum depth in the input image.
         */
        waterShedFilter->SetLevel(waterShedLevel_.get());
        waterShedFilter->SetThreshold(waterShedThreshold_.get());

        waterShedFilter->SetInput(inPort_.getData());

        waterShedFilter->Update();

        typedef itk::ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType> RGBFilterType;
          RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();

        colormapImageFilter->SetInput(waterShedFilter->GetOutput());
        colormapImageFilter->SetColormap( RGBFilterType::Jet );
        colormapImageFilter->Update();

        //outPort_.setData(colormapImageFilter->GetOutput());

    }
    catch (int e)
    {
        LERROR("Problem with Watershed Segmentation Filter!");
    return;
    }
}

} //namespace



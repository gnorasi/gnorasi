/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) ****** - All rights reserved.                                  *
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

#include "otbhistogrammatchingimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBHistogramMatchingImageFilterProcessor::loggerCat_("voreen.OTBHistogramMatchingImageFilterProcessor");

OTBHistogramMatchingImageFilterProcessor::OTBHistogramMatchingImageFilterProcessor()
    :OTBImageFilterProcessor(),
    numberOfHistogramLevels_("numberOfHistogramLevels", "Number of Histogram Levels:", 1, 1, 100),
    numberOfMatchPoints_("numberOfMatchPoints", "Number Of Match Points:", 10, 1, 100),
    thresholdAtMeanIntensity_("thesholdAtMeanIntensity", "Theshold at Mean Intensity:", true),
    sourcePort_(Port::INPORT, "OTBImage.source",0),
    referencePort_(Port::INPORT, "OTBImage.reference",0),
    outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    addProperty(numberOfHistogramLevels_);
    addProperty(numberOfMatchPoints_);
    addProperty(thresholdAtMeanIntensity_);
    addPort(sourcePort_);
    addPort(referencePort_);
    addPort(outPort_);

      filter = FilterType::New();
}

Processor* OTBHistogramMatchingImageFilterProcessor::create() const {
    return new OTBHistogramMatchingImageFilterProcessor();
}

OTBHistogramMatchingImageFilterProcessor::~OTBHistogramMatchingImageFilterProcessor() {

}

void OTBHistogramMatchingImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBHistogramMatchingImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBHistogramMatchingImageFilterProcessor::getProcessorInfo() const {
    return "Histogram Matching Image Filter";
}

void OTBHistogramMatchingImageFilterProcessor::process() {


    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&sourcePort_, &outPort_);
        return;
    }


    try
    {
        filter->SetSourceImage(sourcePort_.getData());
        filter->SetReferenceImage(referencePort_.getData());
        filter->SetNumberOfHistogramLevels(numberOfHistogramLevels_.get());
        filter->SetNumberOfMatchPoints(numberOfMatchPoints_.get());
        filter->SetThresholdAtMeanIntensity(thresholdAtMeanIntensity_.get());
        filter->Update();
        outPort_.setData(filter->GetOutput());
        LINFO("Histogram Matching Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Histogram Matching Image Filter");
        return;
    }
}

}   // namespace

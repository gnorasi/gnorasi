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

#include "otbzerocrossingbasededgedetectionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::loggerCat_("voreen.OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor");

OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor()
    :OTBImageFilterProcessor(),
      variance_("variance", "Variance:", 2.0f, 0.0f, 10000.0f),
      maxError_("maxError", "Maximum Error:", 0.01f, 0.01f, 0.99f),
      backgroundValue_("backgroundValue", "Background Label value:", 0.0f, 0.0f, 10000.0f),
      foregroundValue_("foregroundValue", "Foreground Label value:", 1.0f, 0.0f, 10000.0f),
      inPort_(Port::INPORT, "IN OTB Image port", 0),
      outPort_(Port::OUTPORT, "OUT Image port", 0)

{
      addProperty(enableSwitch_);
      addProperty(variance_);
      addProperty(maxError_);
      addProperty(backgroundValue_);
      addProperty(foregroundValue_);
      addPort(inPort_);
      addPort(outPort_);

      filter = ZeroCrossingType::New();
}

Processor* OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::create() const {
    return new OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor();
}

OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::~OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor() {

}

void OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::getProcessorInfo() const {
    return "Zero Crossing Based Edge Detection Image Filter";
}

void OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    ZeroCrossingType::ArrayType var;
    ZeroCrossingType::ArrayType error;
    ImageType::PixelType bValue;
    ImageType::PixelType fValue;

    var.Fill(variance_.get());
    error.Fill(maxError_.get());
    bValue = backgroundValue_.get();
    fValue = foregroundValue_.get();

    try
    {
        filter->SetInput(inPort_.getData());
        filter->SetVariance(var);
        filter->SetMaximumError(error);
        filter->SetBackgroundValue(bValue);
        filter->SetForegroundValue(fValue);
        outPort_.setData(filter->GetOutput());
        LINFO("Zero Crossing Based Edge Detection Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Zero Crossing Based Edge Detection Image Filter");
        return;
    }

}

}   // namespace

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

#include "otbvcaimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBVCAImageFilterProcessor::loggerCat_("voreen.OTBVCAImageFilterProcessor");

OTBVCAImageFilterProcessor::OTBVCAImageFilterProcessor()
    :OTBImageFilterProcessor(),
      estimateNumberOfEndmembers_("estimateNumberOfEndmembers_", "Estimate Number of Endmembers:", 16, 0,100),
      inPort_(Port::INPORT, "IN MultiBand Image", 0),
      outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)
{
      addProperty(enableSwitch_);
      addProperty(estimateNumberOfEndmembers_);

      addPort(inPort_);
      addPort(outPort_);

      filter = VCAFilterType::New();
      rescaler = VectorRescalerFilterType::New();
}

Processor* OTBVCAImageFilterProcessor::create() const {
    return new OTBVCAImageFilterProcessor();
}

OTBVCAImageFilterProcessor::~OTBVCAImageFilterProcessor() {

}

void OTBVCAImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBVCAImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBVCAImageFilterProcessor::getProcessorInfo() const {
    return "VCA Image Filter";
}

void OTBVCAImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    try
    {
        rescaler->SetInput(inPort_.getData());

        VectorImageType::PixelType minPixel, maxPixel;
        minPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
        maxPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
        minPixel.Fill(0.);
        maxPixel.Fill(1.);

        rescaler->SetOutputMinimum(minPixel);
        rescaler->SetOutputMaximum(maxPixel);

        filter->SetInput(rescaler->GetOutput());
        filter->SetNumberOfEndmembers(estimateNumberOfEndmembers_.get());

        filter->Update();

        outPort_.setData(filter->GetOutput());

        LINFO("VCA Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in VCA Image Filter");
        return;
    }

}

void OTBVCAImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace


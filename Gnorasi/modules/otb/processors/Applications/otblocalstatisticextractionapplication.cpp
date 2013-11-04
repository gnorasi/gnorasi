/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#include "otblocalstatisticextractionapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLocalStatisticExtractionApplication::loggerCat_("voreen.OTBLocalStatisticExtractionApplication");

OTBLocalStatisticExtractionApplication::OTBLocalStatisticExtractionApplication()
    :OTBImageFilterProcessor(),
      channel_("The selected channel index", "Selected Channel", 1),
      radius_("The computational window radius.", "Neighborhood radius", 3, 1, 10),
      inPort_(Port::INPORT, "IN OTB Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)

{
      addProperty(enableSwitch_);
      addProperty(channel_);
      addProperty(radius_);
      addPort(inPort_);
      addPort(outPort_);

    extractorFilter = ExtractorFilterType::New();
    filter = FilterType::New();

}

Processor* OTBLocalStatisticExtractionApplication::create() const {
    return new OTBLocalStatisticExtractionApplication();
}

OTBLocalStatisticExtractionApplication::~OTBLocalStatisticExtractionApplication() {

}

void OTBLocalStatisticExtractionApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBLocalStatisticExtractionApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBLocalStatisticExtractionApplication::getProcessorInfo() const {
    return "Local Statistic Extraction Application";
}

void OTBLocalStatisticExtractionApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        extractorFilter->SetInput(inPort_.getData());
        extractorFilter->SetStartX(inPort_.getData()->GetLargestPossibleRegion().GetIndex(0));
        extractorFilter->SetStartY(inPort_.getData()->GetLargestPossibleRegion().GetIndex(1));
        extractorFilter->SetSizeX(inPort_.getData()->GetLargestPossibleRegion().GetSize(0));
        extractorFilter->SetSizeY(inPort_.getData()->GetLargestPossibleRegion().GetSize(1));
        extractorFilter->SetChannel(channel_.get());
        extractorFilter->UpdateOutputInformation();

        filter->SetInput(extractorFilter->GetOutput());
        filter->SetRadius(radius_.get());
        filter->UpdateOutputInformation();

        outPort_.setData(filter->GetOutput());

        LINFO("Local Statistic Extraction Application Connected");

    }
    catch (int e)
    {
        LERROR("Error in Local Statistic Extraction Application");
        return;
    }

}

void OTBLocalStatisticExtractionApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace

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

#include "otbimagetosiftkeypointsetfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

#include "otbsiftfastimagefilterprocessor.h"


namespace voreen {
const std::string OTBSiftFastImageFilterProcessor::loggerCat_("voreen.OTBSiftFastImageFilterProcessor");

OTBSiftFastImageFilterProcessor::OTBSiftFastImageFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Image Input", 0),
    outPort_(Port::OUTPORT, "Point Set Output", 0),
    m_scalesProperty("siftScales", "Scales Property", 3, 3, 100)
{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(m_scalesProperty);

    //*FIXME: Instantiate triggers reference error*
    filter = ImageToFastSIFTKeyPointSetFilter::New();
}

Processor* OTBSiftFastImageFilterProcessor::create() const {
  return new OTBSiftFastImageFilterProcessor();
}

OTBSiftFastImageFilterProcessor::~OTBSiftFastImageFilterProcessor() {

}

void OTBSiftFastImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBSiftFastImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBSiftFastImageFilterProcessor::getProcessorInfo() const {

    return "SIFT Fast Image Filter";
}

void OTBSiftFastImageFilterProcessor::process() {

    try
    {
//        filter->SetInput(inPort_.getData());
//        filter->SetScalesNumber(m_scalesProperty.get());
//        filter->Update();
//        outPort_.setData(filter->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with SIFT Fast Image Filter!");
    return;
    }

}

} // namespace

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
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

#include "otbsomimageclassificationfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBSOMImageClassificationFilterProcessor::loggerCat_("voreen.OTBSOMImageClassificationFilterProcessor");

OTBSOMImageClassificationFilterProcessor::OTBSOMImageClassificationFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    SOMMapPort_(Port::INPORT, "SOM Map Port", 0),
    outPort_(Port::OUTPORT, "Image Output", 0)
{
    addPort(inPort_);
    addPort(SOMMapPort_);
    addPort(outPort_);

    filter = ClassificationFilterType::New();
    labelToPortCaster = LabelToPortCastingFilterType::New();
}

Processor* OTBSOMImageClassificationFilterProcessor::create() const {
  return new OTBSOMImageClassificationFilterProcessor();
}

OTBSOMImageClassificationFilterProcessor::~OTBSOMImageClassificationFilterProcessor() {

}

void OTBSOMImageClassificationFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBSOMImageClassificationFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBSOMImageClassificationFilterProcessor::getProcessorInfo() const {

    return "SOM Classification Filter";
}

void OTBSOMImageClassificationFilterProcessor::process() {

    try
    {
        filter->SetInput(inPort_.getData());
        filter->SetMap(SOMMapPort_.getData());
        labelToPortCaster->SetInput(filter->GetOutput());
        outPort_.setData(labelToPortCaster->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with SOM Classification Filter!");
    return;
    }

}

} // namespace

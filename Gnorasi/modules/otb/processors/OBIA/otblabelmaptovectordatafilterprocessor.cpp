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

#include "otblabelmaptovectordatafilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLabelMapToVectorDataFilterProcessor::loggerCat_("voreen.OTBLabelMapToVectorDataFilterProcessor");

OTBLabelMapToVectorDataFilterProcessor::OTBLabelMapToVectorDataFilterProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "Label Map Inport",0),
      outPort_(Port::OUTPORT, "Vector Data Outport",0)
{
    addPort(inPort_);
    addPort(outPort_);

    filter = LabelMapToVectorDataFilterType::New();
}

Processor* OTBLabelMapToVectorDataFilterProcessor::create() const {
    return new OTBLabelMapToVectorDataFilterProcessor();
}

OTBLabelMapToVectorDataFilterProcessor::~OTBLabelMapToVectorDataFilterProcessor() {

}

void OTBLabelMapToVectorDataFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBLabelMapToVectorDataFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBLabelMapToVectorDataFilterProcessor::getProcessorInfo() const {
    return "Label Map to Vector Data Filter Processor";
}

void OTBLabelMapToVectorDataFilterProcessor::process() {

    try
    {
        filter->SetInput(inPort_.getData());
        filter->Update();
        outPort_.setData(filter->GetOutput());

        LINFO("Label Map to Vector Data Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Label Map to Vector Data Filter!");
        return;
    }
}

}   // namespace


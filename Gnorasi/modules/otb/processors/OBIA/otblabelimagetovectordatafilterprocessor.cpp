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

#include "otblabelimagetovectordatafilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLabelImageToVectorDataFilterProcessor::loggerCat_("voreen.OTBLabelImageToVectorDataFilterProcessor");

OTBLabelImageToVectorDataFilterProcessor::OTBLabelImageToVectorDataFilterProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "Label Image Inport",0),
      outPort_(Port::OUTPORT, "Vector Data Outport",0)
{
    addPort(inPort_);
    addPort(outPort_);

    filter = VectorizationFilterType::New();
}

Processor* OTBLabelImageToVectorDataFilterProcessor::create() const {
    return new OTBLabelImageToVectorDataFilterProcessor();
}

OTBLabelImageToVectorDataFilterProcessor::~OTBLabelImageToVectorDataFilterProcessor() {

}

void OTBLabelImageToVectorDataFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBLabelImageToVectorDataFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBLabelImageToVectorDataFilterProcessor::getProcessorInfo() const {
    return "Label Image to Vector Data Filter Processor";
}

void OTBLabelImageToVectorDataFilterProcessor::process() {

    try
    {
        filter->SetInput(inPort_.getData());
        filter->Update();
        outPort_.setData(filter->GetOutput());

        LINFO("Label Image to Vector Data Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Label Image to Vector Data Filter!");
        return;
    }
}

}   // namespace


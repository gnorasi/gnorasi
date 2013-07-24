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

#include "otblabelmaptobinaryimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLabelMapToBinaryImageFilterProcessor::loggerCat_("voreen.OTBLabelMapToBinaryImageFilterProcessor");

OTBLabelMapToBinaryImageFilterProcessor::OTBLabelMapToBinaryImageFilterProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "ObjectMap inport",0),
      outPort_(Port::OUTPORT, "OTBImage outport",0)
{
    addPort(inPort_);
    addPort(outPort_);

    filter = LabelMapToBinaryImageFilter::New();
    caster = MaskCasterType::New();
}

Processor* OTBLabelMapToBinaryImageFilterProcessor::create() const {
    return new OTBLabelMapToBinaryImageFilterProcessor();
}

OTBLabelMapToBinaryImageFilterProcessor::~OTBLabelMapToBinaryImageFilterProcessor() {

}

void OTBLabelMapToBinaryImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBLabelMapToBinaryImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBLabelMapToBinaryImageFilterProcessor::getProcessorInfo() const {
    return "Label Map To Binary Image Filter";
}

void OTBLabelMapToBinaryImageFilterProcessor::process() {

    try
    {
        filter->SetInput(inPort_.getData());
        caster->SetInput(filter->GetOutput());
        caster->Update();
        outPort_.setData(caster->GetOutput());

        LINFO("Label Map To Binary Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Label Map To Binary Image Filter");
        return;
    }

}

}   // namespace



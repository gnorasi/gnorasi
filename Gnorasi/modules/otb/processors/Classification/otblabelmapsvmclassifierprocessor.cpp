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

#include "otblabelmapsvmclassifierprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBLabelMapSVMClassifierProcessor::loggerCat_("voreen.OTBLabelMapSVMClassifierProcessor");

OTBLabelMapSVMClassifierProcessor::OTBLabelMapSVMClassifierProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Label Map Input", 0),
//    SVMModelPort_(Port::INPORT, "SVM Model Port", 0),
    outPort_(Port::OUTPORT, "Image Output", 0)
{
    addPort(inPort_);
//    addPort(SVMModelPort_);
    addPort(outPort_);


    //***FIXME: TRIGGERS LINKING ERROR***
    filter = SVMClassifierType::New();
}

Processor* OTBLabelMapSVMClassifierProcessor::create() const {
  return new OTBLabelMapSVMClassifierProcessor();
}

OTBLabelMapSVMClassifierProcessor::~OTBLabelMapSVMClassifierProcessor() {

}

void OTBLabelMapSVMClassifierProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBLabelMapSVMClassifierProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBLabelMapSVMClassifierProcessor::getProcessorInfo() const {

    return "SVM Classification Filter";
}

void OTBLabelMapSVMClassifierProcessor::process() {

    try
    {
//        filter->SetInput(inPort_.getData());
//        filter->SetModel(model);

//        labelToPortCaster->SetInput(filter->GetOutput());
//        labelToPortCaster->Update();
//        outPort_.setData(labelToPortCaster->GetOutput());
    }
    catch (int e)
    {
    LERROR("Problem with SVM Classification Filter!");
    return;
    }

}

} // namespace

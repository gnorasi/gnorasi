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

#include "otbdotproductimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBDotProductImageFilterProcessor::loggerCat_("voreen.OTBDotProductImageFilterProcessor");

OTBDotProductImageFilterProcessor::OTBDotProductImageFilterProcessor()
    :OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "IN MultiBand Image", 0),
    outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)
{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
}

Processor* OTBDotProductImageFilterProcessor::create() const {
    return new OTBDotProductImageFilterProcessor();
}

OTBDotProductImageFilterProcessor::~OTBDotProductImageFilterProcessor() {

}

void OTBDotProductImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBDotProductImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBDotProductImageFilterProcessor::getProcessorInfo() const {
    return "Dot Product Image Filter Processor";
}

void OTBDotProductImageFilterProcessor::process() {

    LINFO("Dot Product Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
   }

    try
    {
        VectorImageType *img = inPort_.getData();
        filter->SetInput(img);

        //test
        //filter->Update();
        //end of test

        outPort_.setData(filter->GetOutput());
        LINFO("Dot Product Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Dot Product Image Filter");
        return;
    }

}

void OTBDotProductImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}// namespace



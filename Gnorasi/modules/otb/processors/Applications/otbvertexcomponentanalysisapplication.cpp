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

#include "otbvertexcomponentanalysisapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBVertexComponentAnalysisApplication::loggerCat_("voreen.OTBVertexComponentAnalysisApplication");

OTBVertexComponentAnalysisApplication::OTBVertexComponentAnalysisApplication()
    :OTBImageFilterProcessor(),
      numEnd_("numEnd_", "Number of endmembers", 5, 1, 100),
      inPort_(Port::INPORT, "IN Vector Image", 0),
      outPort_(Port::OUTPORT, "OUT Vector Image", 0)
{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);
    addProperty(numEnd_);

    vca = VCAFilterType::New();

}

Processor* OTBVertexComponentAnalysisApplication::create() const {
    return new OTBVertexComponentAnalysisApplication();
}

OTBVertexComponentAnalysisApplication::~OTBVertexComponentAnalysisApplication() {

}

void OTBVertexComponentAnalysisApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBVertexComponentAnalysisApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBVertexComponentAnalysisApplication::getProcessorInfo() const {
    return "Vertex Component Analysis Application";
}

void OTBVertexComponentAnalysisApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        vca->SetInput(inPort_.getData());
        vca->SetNumberOfEndmembers(numEnd_.get());
        outPort_.setData(vca->GetOutput());

        LINFO("Vertex Component Analysis Application Connected");
    }
    catch (int e)
    {
        LERROR("Error in Vertex Component Analysis Application");
        return;
    }

}

void OTBVertexComponentAnalysisApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
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

#include "otbcbamichangedetectorprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBCBAMIChangeDetectorProcessor::loggerCat_("voreen.OTBCBAMIChangeDetectorProcessor");

OTBCBAMIChangeDetectorProcessor::OTBCBAMIChangeDetectorProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input 1", 0),
    inPort1_(Port::INPORT, "Vector Image Input 2", 0),
    outPort_(Port::OUTPORT, "Vector Image Output", 0),
    m_radiusProperty("radius", "Radius",2)
{
    addPort(inPort_);
    addPort(inPort1_);
    addPort(outPort_);

    addProperty(m_radiusProperty);

    cbamidetector = CBAMINDetectorType::New();

}

void OTBCBAMIChangeDetectorProcessor::update(){
    process();
}

Processor* OTBCBAMIChangeDetectorProcessor::create() const {
  return new OTBCBAMIChangeDetectorProcessor();
}

OTBCBAMIChangeDetectorProcessor::~OTBCBAMIChangeDetectorProcessor() {

}

void OTBCBAMIChangeDetectorProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBCBAMIChangeDetectorProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBCBAMIChangeDetectorProcessor::getProcessorInfo() const {
    
    return "Implements neighborhood-wise the computation of the cumulant-based approximation to mutual information";
}

void OTBCBAMIChangeDetectorProcessor::process() {
  
    try
    {
        InputImageType *img = inPort_.getData();
        InputImageType *img1 = inPort1_.getData();
        if(!img || !img1) {
	    LERROR("One or more input images missing.");
            return;
	}


        cbamidetector->SetInput1(img);
        cbamidetector->SetInput2(img1);
        cbamidetector->SetRadius(m_radiusProperty.get());

        outPort_.setData(cbamidetector->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with cbami detector!");
	return;
    }
    
}

} // namespace

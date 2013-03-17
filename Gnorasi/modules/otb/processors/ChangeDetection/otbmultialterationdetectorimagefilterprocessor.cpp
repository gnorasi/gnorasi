/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
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

#include "otbmultialterationdetectorimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBMultiAlterationDetectorImageFilterProcessor::loggerCat_("voreen.OTBMultiAlterationDetectorImageFilterProcessor");

OTBMultiAlterationDetectorImageFilterProcessor::OTBMultiAlterationDetectorImageFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input 1", 0),
    inPort1_(Port::INPORT, "Vector Image Input 2", 0),
    outPort_(Port::OUTPORT, "Vector Image Output", 0)
{
    addPort(inPort_);
    addPort(inPort1_);
    addPort(outPort_);

    madfilter = MADFilterType::New();
}

void OTBMultiAlterationDetectorImageFilterProcessor::update(){
    process();
}

Processor* OTBMultiAlterationDetectorImageFilterProcessor::create() const {
  return new OTBMultiAlterationDetectorImageFilterProcessor();
}

OTBMultiAlterationDetectorImageFilterProcessor::~OTBMultiAlterationDetectorImageFilterProcessor() {

}

void OTBMultiAlterationDetectorImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBMultiAlterationDetectorImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBMultiAlterationDetectorImageFilterProcessor::getProcessorInfo() const {
    
    return "Implements the Multivariate Alteration Detector";
}

void OTBMultiAlterationDetectorImageFilterProcessor::process() {
  
    try
    {
        InputImageType *img = inPort_.getData();
        InputImageType *img1 = inPort1_.getData();
        if(!img || !img1) {
	    LERROR("One or more input images missing.");
            return;
	}

        madfilter->SetInput1(img);
        madfilter->SetInput(img1);

        madfilter->Update();

        outPort_.setData(madfilter->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with Multivariate Alteration Detector!");
	return;
    }
    
}

} // namespace

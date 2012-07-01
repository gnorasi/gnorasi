/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

#include "otbgrayscaleclosingfilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBGrayscaleClosingFilterProcessor::loggerCat_("voreen.OTBGrayscaleClosingFilterProcessor");

OTBGrayscaleClosingFilterProcessor::OTBGrayscaleClosingFilterProcessor()
    : OTBImageFilterProcessor(),
    radius_("radius", "Structuring Element Radius", 1),
    inPort_(Port::INPORT, "Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0)
{
    addProperty(enableSwitch_);
    addProperty(radius_);
    addPort(inPort_);
    addPort(outPort_);
    
    dilatefilter = DilateFilterType::New();
    erodefilter = ErodeFilterType::New();
    structuringElement.SetRadius(radius_.get());
    byterescaler = ByteRescalerFilterType::New();
    doublerescaler = DoubleRescalerFilterType::New();
}

Processor* OTBGrayscaleClosingFilterProcessor::create() const {
  return new OTBGrayscaleClosingFilterProcessor(); 
}

OTBGrayscaleClosingFilterProcessor::~OTBGrayscaleClosingFilterProcessor() {

}

void OTBGrayscaleClosingFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBGrayscaleClosingFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBGrayscaleClosingFilterProcessor::getProcessorInfo() const {
    
    return "Grayscale Closing Filtering Processor";
}

void OTBGrayscaleClosingFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    structuringElement.SetRadius(radius_.get());
    structuringElement.CreateStructuringElement();
    dilatefilter->SetKernel(structuringElement);
    erodefilter->SetKernel(structuringElement);
    
    try
    {
	byterescaler->SetOutputMinimum(0);
	byterescaler->SetOutputMaximum(255);
        byterescaler->SetInput(inPort_.getData());
	
	dilatefilter->SetInput(byterescaler->GetOutput());
	erodefilter->SetInput(dilatefilter->GetOutput());
	
	doublerescaler->SetOutputMinimum(0);
	doublerescaler->SetOutputMaximum(255);
	doublerescaler->SetInput(erodefilter->GetOutput());
	
	outPort_.setData(doublerescaler->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Grayscale Closing Image Filter!");
	return;
    }
    
}


} // namespace

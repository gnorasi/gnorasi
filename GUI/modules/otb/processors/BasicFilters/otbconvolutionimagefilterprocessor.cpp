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

#include "otbconvolutionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBConvolutionImageFilterProcessor::loggerCat_("voreen.OTBConvolutionImageFilterProcessor");

OTBConvolutionImageFilterProcessor::OTBConvolutionImageFilterProcessor()
    : OTBImageFilterProcessor(),
    filterSize_("filterSize", "Filter Radius", 1),
    kernel_("kernel", "Filter Kernel in CSV", "1;1;1;1;1;1;1;1;1"),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(filterSize_);
    addProperty(kernel_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

OTBConvolutionImageFilterProcessor::~OTBConvolutionImageFilterProcessor() {

}

void OTBConvolutionImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBConvolutionImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBConvolutionImageFilterProcessor::getProcessorInfo() const {
    
    return "Base Filtering Processor";
}

void OTBConvolutionImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    
    int rad = filterSize_.get();
    int filternum = (2*rad+1)*(2*rad+1);
    ArrayType array(filternum);
    std::string line = kernel_.get();
    int itemnum = 0;
    std::istringstream linestream(line);
    std::string item;
    try
    {
	while (std::getline (linestream, item, ';'))
	{
	    double temp = (double)atof(item.c_str());
	    //LINFO("Testing kernel element: " << temp);
	    if(itemnum<filternum){
		array.SetElement(itemnum,temp);
	    }
	    itemnum++;
	}
    }
    catch (int e)
    {
	LERROR("Problem converting kernel element to number!");
	return;
    }
    
    if(filternum!=itemnum){
	LWARNING("Kernel size does not match provided elements");
	return;
    }else{
	LINFO("Kernel setup is OK");
    }
    
    filter->SetFilter(array);
    filter->SetInput(inPort_.getData());
    outPort_.setData(filter->GetOutput());
    
}


} // namespace

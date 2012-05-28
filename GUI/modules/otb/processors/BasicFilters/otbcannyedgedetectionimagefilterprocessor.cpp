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

#include "otbcannyedgedetectionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBCannyEdgeDetectionImageFilterProcessor::loggerCat_("voreen.OTBConvolutionImageFilterProcessor");

OTBCannyEdgeDetectionImageFilterProcessor::OTBCannyEdgeDetectionImageFilterProcessor()
    : OTBImageFilterProcessor(),
    variance_("variance", "Variance", 2.0f, 0.0f, 10000.0f),
    upper_("upper", "Upper Threshold", 10.0f, 0.0f, 10000.0f),
    lower_("lower", "Lower Threshold", 0.0f, 0.0f, 10000.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(variance_);
    addProperty(upper_);
    addProperty(lower_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

OTBCannyEdgeDetectionImageFilterProcessor::~OTBCannyEdgeDetectionImageFilterProcessor() {

}

void OTBCannyEdgeDetectionImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBCannyEdgeDetectionImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBCannyEdgeDetectionImageFilterProcessor::getProcessorInfo() const {
    
    return "Canny Edge Detection Image Filtering Processor";
}

void OTBCannyEdgeDetectionImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    try
    {
	filter->SetVariance( variance_.get() );
	filter->SetUpperThreshold( upper_.get() );
	filter->SetLowerThreshold( lower_.get() );
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
	
	LINFO("Canny Edge Detection Sucesfull!");
    }
    catch (int e)
    {
	LERROR("Error in Canny Edge Detection");
	return;
    }

    
}


} // namespace

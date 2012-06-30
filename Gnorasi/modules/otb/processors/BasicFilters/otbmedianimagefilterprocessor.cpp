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

#include "otbmedianimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBMedianImageFilterProcessor::loggerCat_("voreen.OTBMedianImageFilterProcessor");

OTBMedianImageFilterProcessor::OTBMedianImageFilterProcessor()
    : OTBImageFilterProcessor(),
    filterSize_("filterSize", "Filter Radius", 1),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(filterSize_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

OTBMedianImageFilterProcessor::~OTBMedianImageFilterProcessor() {

}

void OTBMedianImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBMedianImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBMedianImageFilterProcessor::getProcessorInfo() const {
    
    return "Median Filtering Processor";
}

void OTBMedianImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    
    int rad = filterSize_.get();
    ImageType::SizeType indexRadius;

    indexRadius[0] = rad; // radius along x
    indexRadius[1] = rad; // radius along y

    filter->SetRadius( indexRadius );
    
    try
    {
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Median Image Filter!");
	return;
    }
    
}


} // namespace

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

#include "otbvectorimagebandmathprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBVectorImageBandMathProcessor::loggerCat_("voreen.OTBVectorImageBandMathProcessor");

OTBVectorImageBandMathProcessor::OTBVectorImageBandMathProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    expression_("expr", "Math Expression", "(b1-b2)")
{
    addPort(inPort_);
    addPort(outPort_);
    expression_.onChange(CallMemberAction<OTBVectorImageBandMathProcessor>(this, 
					&OTBVectorImageBandMathProcessor::setExpresion));
    addProperty(expression_);
    filter = FilterType::New();
    imageList = VectorImageToImageListType::New();
}

OTBVectorImageBandMathProcessor::~OTBVectorImageBandMathProcessor() {

}

void OTBVectorImageBandMathProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBVectorImageBandMathProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBVectorImageBandMathProcessor::getProcessorInfo() const {
    
    return "Calculate Math Expression for Vector Image Bands";
}

void OTBVectorImageBandMathProcessor::process() {
  
    try
    {
	imageList->SetInput(inPort_.getData());
	imageList->UpdateOutputInformation();
      
	const unsigned int nbBands = inPort_.getData()->GetNumberOfComponentsPerPixel();
	
	for(unsigned int j = 0; j < nbBands; ++j)
	{
	    filter->SetNthInput(j, imageList->GetOutput()->GetNthElement(j));
	}
	
	filter->SetExpression(expression_.get());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Band Math calculation!");
	return;
    }
    
}

void OTBVectorImageBandMathProcessor::setExpresion() {
    
    filter->SetExpression(expression_.get());
}

} // namespace

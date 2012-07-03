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

#include "otbtwoimagebandmathprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBTwoVectorImagesBandMathProcessor::loggerCat_("voreen.OTBTwoVectorImagesBandMathProcessor");

OTBTwoVectorImagesBandMathProcessor::OTBTwoVectorImagesBandMathProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Vector Image Input1", 0),
    inPort2_(Port::INPORT, "Vector Image Input2", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    expression_("expr", "Math Expression", "(b1-b2)")
{
    addPort(inPort_);
    addPort(inPort2_);
    addPort(outPort_);
    expression_.onChange(CallMemberAction<OTBTwoVectorImagesBandMathProcessor>(this, 
					&OTBTwoVectorImagesBandMathProcessor::setExpresion));
    addProperty(expression_);
    filter = FilterType::New();
    imageList = VectorImageToImageListType::New();
    imageList2 = VectorImageToImageListType::New();
}

Processor* OTBTwoVectorImagesBandMathProcessor::create() const {
  return new OTBTwoVectorImagesBandMathProcessor(); 
}

OTBTwoVectorImagesBandMathProcessor::~OTBTwoVectorImagesBandMathProcessor() {

}

void OTBTwoVectorImagesBandMathProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBTwoVectorImagesBandMathProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBTwoVectorImagesBandMathProcessor::getProcessorInfo() const {
    
    return "Calculate Math Expression for Two Vector Images";
}

void OTBTwoVectorImagesBandMathProcessor::process() {
  
    try
    {
	imageList->SetInput(inPort_.getData());
	imageList->UpdateOutputInformation();
	imageList2->SetInput(inPort2_.getData());
	imageList2->UpdateOutputInformation();
      
	const unsigned int nbBands1 = inPort_.getData()->GetNumberOfComponentsPerPixel();
	
	for(unsigned int j = 0; j < nbBands1; ++j)
	{
	    filter->SetNthInput(j, imageList->GetOutput()->GetNthElement(j));
	}
	
	const unsigned int nbBands2 = inPort2_.getData()->GetNumberOfComponentsPerPixel();
	int k = nbBands1;

	for(unsigned int j = 0; j < nbBands2; ++j)
	{
	    filter->SetNthInput(k, imageList2->GetOutput()->GetNthElement(j));
	    k++;
	}
	
	LINFO("Math Expression can have bands from b1 to b" << k );
	
	filter->SetExpression(expression_.get());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Two Image Band Math calculation!");
	return;
    }
    
}

void OTBTwoVectorImagesBandMathProcessor::setExpresion() {
    
    filter->SetExpression(expression_.get());
}

} // namespace

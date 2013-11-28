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

#include "otblabelimagetolabelmapprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBLabelImageToLabelMapProcessor::loggerCat_("voreen.OTBLabelImageToLabelMapProcessor");

OTBLabelImageToLabelMapProcessor::OTBLabelImageToLabelMapProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Label Image", 0),
    outPort_(Port::OUTPORT, "Object Map", 0)
{
    addPort(inPort_);
    addPort(outPort_);
    
    labelMapFilter = LabelMapFilterType::New();
}

Processor* OTBLabelImageToLabelMapProcessor::create() const {
  return new OTBLabelImageToLabelMapProcessor(); 
}

OTBLabelImageToLabelMapProcessor::~OTBLabelImageToLabelMapProcessor() {

}

void OTBLabelImageToLabelMapProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBLabelImageToLabelMapProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBLabelImageToLabelMapProcessor::getProcessorInfo() const {
    
    return "Transformation of Label Image to Object Map";
}

void OTBLabelImageToLabelMapProcessor::process() {
    
    try
    {
	labelMapFilter->SetInput(inPort_.getData());
	//labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());
        labelMapFilter->SetBackgroundValue(-1);
        labelMapFilter->Update();
	outPort_.setData(labelMapFilter->GetOutput());
	LINFO("Label Image to Objects Processed");
    }
    catch (int e)
    {
	LERROR("Problem with Label Image to Object Map process!");
	return;
    }

}

/*bool OTBLabelImageToLabelMapProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected() && !outPort_.isConnected()) return false;
    
    return true;
}*/


} // namespace

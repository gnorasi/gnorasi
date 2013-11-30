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

#include "otblabelimagetoclassimageprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLabelImageToClassImageProcessor::loggerCat_("voreen.OTBLabelImageToClassImageProcessor");
  
OTBLabelImageToClassImageProcessor::OTBLabelImageToClassImageProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Label Image Input", 0),
    inTextPort_(Port::INPORT, "Input Classification Text", 0),
    outPort_(Port::OUTPORT, "Classified Image Output", 0),
    updateButton_("updateButton", "Update")
{
    // register ports and properties
    addPort(inPort_);
    addPort(inTextPort_);
    addPort(outPort_);
    updateButton_.onChange(CallMemberAction<OTBLabelImageToClassImageProcessor>(this, &OTBLabelImageToClassImageProcessor::update));
    addProperty(updateButton_);
    
    //OTB initialization
    inLabelImg = NULL;
    outLabelImg = LabeledImageType::New();
}

Processor* OTBLabelImageToClassImageProcessor::create() const {
    return new OTBLabelImageToClassImageProcessor();
}


bool OTBLabelImageToClassImageProcessor::isReady() const {
    return (inPort_.isConnected() && inTextPort_.isConnected());
}

std::string OTBLabelImageToClassImageProcessor::getProcessorInfo() const {
    return "Saves Classification as a Label Image";
}

void OTBLabelImageToClassImageProcessor::process() {

}

void OTBLabelImageToClassImageProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
}

void OTBLabelImageToClassImageProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBLabelImageToClassImageProcessor::update() {
    
    if(this->isReady())
    {
	try
	{
	    //Connect with data inport
	    inLabelImg = inPort_.getData();
            
            std::map<unsigned long, unsigned short> classIndex;
	    
            ////////////////////////////////////////////////
	    //Text port code
            ////////////////////////////////////////////////
            std::stringstream ss(inTextPort_.getData());
	    
	    //For each input text line (CSV)
	    std::string line;
	    std::getline(ss,line);//throw first line
	    while(std::getline(ss,line))
	    {
		std::stringstream  lineStream(line);
		std::string        cell;
		int i = 1;
		unsigned long id;
		unsigned short classid;
		//debug info
		//LINFO(line);
		while(std::getline(lineStream,cell,';'))
		{
		    std::stringstream tmp(cell);
		    if (i == 1) tmp >> id;
                    if (i == 2) {
                        tmp >> classid;                
                        classIndex[id]=classid;
                        //classIndex.insert(std::make_pair(id, classid));
                    }
		    i++;
		}
	    }
	
            ////////////////////////////////////////////////
            //Image port code
            ////////////////////////////////////////////////            
            inLabelImg->UpdateOutputInformation();
	    inLabelImg->UpdateOutputData();
	    outLabelImg->SetRegions(inLabelImg->GetRequestedRegion());
	    outLabelImg->CopyInformation(inLabelImg);
	    outLabelImg->Allocate();
	    
            ConstIteratorType inputIt(inLabelImg, inLabelImg->GetRequestedRegion()); 
            IteratorType outputIt(outLabelImg, inLabelImg->GetRequestedRegion());
	    
            inputIt.SetDirection(0); 
            outputIt.SetDirection(0);
	    
            for (inputIt.GoToBegin(),  outputIt.GoToBegin(); !inputIt.IsAtEnd(); 
                outputIt.NextLine(),  inputIt.NextLine()) 
            { 
                inputIt.GoToBeginOfLine();
		outputIt.GoToBeginOfLine();
		while (!inputIt.IsAtEndOfLine()) 
                { 
                    //TODO: Check if I can connect to color type from global
                    unsigned long curval;
                    unsigned short classval;
                    curval = inputIt.Get();
                    classval = classIndex[curval];
                    outputIt.Set(classval);
                    ++inputIt; 
                    ++outputIt;
                } 
            }            
            
            outPort_.setData(outLabelImg);
	    
	}
	catch(itk::ExceptionObject& err)
	{
	    LWARNING("ExceptionObject caught !");
	    return;
	}
    }else if(!this->isReady()){
	LWARNING("Input Label Image or Input Classification Text Data is not connected");
	return;
    }

    LINFO("Classified Image created succesfully!");
    
}


} // namespace

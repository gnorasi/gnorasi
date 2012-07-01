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

#include "otblabelimagereaderprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBLabelImageReaderProcessor::loggerCat_("voreen.OTBLabelImageReaderProcessor");

OTBLabelImageReaderProcessor::OTBLabelImageReaderProcessor()
    : Processor(),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0),
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getUserDataPath()),
    clearImage_("clearButton", "Clear Image")
{
    // register ports and properties
    addPort(outPort_);
    imageFile_.onChange(CallMemberAction<OTBLabelImageReaderProcessor>(this, &OTBLabelImageReaderProcessor::loadImage));
    clearImage_.onChange(CallMemberAction<OTBLabelImageReaderProcessor>(this, &OTBLabelImageReaderProcessor::clearImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    
    //OTB stuff
    reader = ReaderType::New();
}

OTBLabelImageReaderProcessor::~OTBLabelImageReaderProcessor() {

}

Processor* OTBLabelImageReaderProcessor::create() const {
    return new OTBLabelImageReaderProcessor();
}

void OTBLabelImageReaderProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    setOutPortData();
    hasImage = false;
}

void OTBLabelImageReaderProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");
    pDataOut_ = 0;
    clearImage();

    Processor::deinitialize();
}

bool OTBLabelImageReaderProcessor::isReady() const {
    return true;
}

bool OTBLabelImageReaderProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void OTBLabelImageReaderProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
      pDataOut_ = reader->GetOutput();
      outPort_.setData(pDataOut_);
    //}
}

std::string OTBLabelImageReaderProcessor::getProcessorInfo() const {
    return "Open Image Data to Perform Processing";
}

void OTBLabelImageReaderProcessor::readData() {

    OTBLabelImageReaderProcessor::updateView();
    
}

void OTBLabelImageReaderProcessor::updateView() {
    
}

void OTBLabelImageReaderProcessor::process() {
    
}

void OTBLabelImageReaderProcessor::loadImage() {

    // necessary since the passed string reference might be changed during clearImage/invalidate,
    std::string filename = imageFile_.get();

    // check for empty filename
    
    if (filename.empty())
        return;

    // load image as texture
    hasImage = true;
    /*texture_ = TexMgr.load(filename, tgt::Texture::LINEAR,
        false, false, true, false);*/
    reader->SetFileName(filename.c_str());
    reader->GenerateOutputInformation();
    
    if (!hasImage) {
        LWARNING("Failed to load image: " << filename);
        imageFile_.set("");
    }

    setOutPortData();
    LINFO("Label Image Loaded.");
    //invalidate();
}

void OTBLabelImageReaderProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

OTBLabelImageReaderProcessor::ImagePointer OTBLabelImageReaderProcessor::getImage() {
    return pDataOut_;
}


} // namespace


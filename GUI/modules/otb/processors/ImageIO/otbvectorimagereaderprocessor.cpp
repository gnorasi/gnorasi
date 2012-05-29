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

#include "otbvectorimagereaderprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBVectorImageReaderProcessor::loggerCat_("voreen.OTBVectorImageReaderProcessor");

OTBVectorImageReaderProcessor::OTBVectorImageReaderProcessor()
    : Processor(),
    outPort_(Port::OUTPORT, "OTBMultibandImage.outport", 0),
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getDataPath()),
    clearImage_("clearButton", "Clear Image")
{
    // register ports and properties
    addPort(outPort_);
    imageFile_.onChange(CallMemberAction<OTBVectorImageReaderProcessor>(this, &OTBVectorImageReaderProcessor::loadImage));
    clearImage_.onChange(CallMemberAction<OTBVectorImageReaderProcessor>(this, &OTBVectorImageReaderProcessor::clearImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    
    //OTB stuff
    reader = ReaderType::New();
}

OTBVectorImageReaderProcessor::~OTBVectorImageReaderProcessor() {

}

Processor* OTBVectorImageReaderProcessor::create() const {
    return new OTBVectorImageReaderProcessor();
}

void OTBVectorImageReaderProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    setOutPortData();
    hasImage = false;
}

void OTBVectorImageReaderProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");
    pDataOut_ = 0;
    clearImage();

    Processor::deinitialize();
}

bool OTBVectorImageReaderProcessor::isReady() const {
    return true;
}

bool OTBVectorImageReaderProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void OTBVectorImageReaderProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
      pDataOut_ = reader->GetOutput();
      outPort_.setData(pDataOut_);
    //}
}

std::string OTBVectorImageReaderProcessor::getProcessorInfo() const {
    return "Open Image Data to Perform Processing";
}

void OTBVectorImageReaderProcessor::readData() {

    OTBVectorImageReaderProcessor::updateView();
    
}

void OTBVectorImageReaderProcessor::updateView() {
    
}

void OTBVectorImageReaderProcessor::process() {
    
}

void OTBVectorImageReaderProcessor::loadImage() {

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
    
    if (!hasImage) {
        LWARNING("Failed to load image: " << filename);
        imageFile_.set("");
    }

    setOutPortData();
    //invalidate();
}

void OTBVectorImageReaderProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

OTBVectorImageReaderProcessor::ImagePointer OTBVectorImageReaderProcessor::getImage() {
    return pDataOut_;
}


} // namespace


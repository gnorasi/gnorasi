/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#include "otbsommapreaderprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBSOMMapReaderProcessor::loggerCat_("voreen.OTBSOMMapReaderProcessor");

OTBSOMMapReaderProcessor::OTBSOMMapReaderProcessor()
    : Processor(),
    SOMMapOutport_(Port::OUTPORT, "SOMMap port", 0),
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getUserDataPath()),
    clearImage_("clearButton", "Clear Image")
{
    // register ports and properties
    addPort(SOMMapOutport_);
    addProperty(imageFile_);
    addProperty(clearImage_);

    imageFile_.onChange(CallMemberAction<OTBSOMMapReaderProcessor>(this, &OTBSOMMapReaderProcessor::loadImage));
    clearImage_.onChange(CallMemberAction<OTBSOMMapReaderProcessor>(this, &OTBSOMMapReaderProcessor::clearImage));

    //OTB stuff
    reader = ReaderType::New();
}

OTBSOMMapReaderProcessor::~OTBSOMMapReaderProcessor() {

}

Processor* OTBSOMMapReaderProcessor::create() const {
    return new OTBSOMMapReaderProcessor();
}

void OTBSOMMapReaderProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    setSOMMapPortData();
    hasImage = false;
}

void OTBSOMMapReaderProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");
    pDataOut_ = 0;
    clearImage();

    Processor::deinitialize();
}

bool OTBSOMMapReaderProcessor::isReady() const {

    if (!isInitialized()) return false;

    if(!SOMMapOutport_.isConnected()) return false;

    return true;
}

bool OTBSOMMapReaderProcessor::isEndProcessor() const {
    return (!SOMMapOutport_.isConnected());
}

void OTBSOMMapReaderProcessor::setSOMMapPortData(){

      pDataOut_ = reader->GetOutput();
      SOMMapOutport_.setData(pDataOut_);

}

std::string OTBSOMMapReaderProcessor::getProcessorInfo() const {
    return "Open Image Data to Perform Processing";
}

void OTBSOMMapReaderProcessor::readData() {

    OTBSOMMapReaderProcessor::updateView();

}

void OTBSOMMapReaderProcessor::updateView() {

}

void OTBSOMMapReaderProcessor::process() {

}

void OTBSOMMapReaderProcessor::loadImage() {

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

    setSOMMapPortData();
    //invalidate();
}

void OTBSOMMapReaderProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

OTBSOMMapReaderProcessor::MapTypePointer OTBSOMMapReaderProcessor::getMap() {
    return pDataOut_;
}


} // namespace


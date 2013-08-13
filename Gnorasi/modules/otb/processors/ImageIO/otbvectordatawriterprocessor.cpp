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

#include "otbvectordatawriterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBVectorDataWriterProcessor::loggerCat_("voreen.OTBVectorDataWriterProcessor");

OTBVectorDataWriterProcessor::OTBVectorDataWriterProcessor()
    : Processor(),
      inPort_(Port::INPORT, "Vector Data Input", 0),
      vectorDataFile_("vectorDataFile", "Output Vector Data", "Vector Data File", VoreenApplication::app()->getUserDataPath(), "Vector Data File (*.SHP)", FileDialogProperty::SAVE_FILE),
      clearVectorData_("clearButton", "Clear Vector Data"),
      saveVectorDataButton_("saveButton", "Save Vector Data")
{
    addPort(inPort_);

    addProperty(vectorDataFile_);
    addProperty(clearVectorData_);
    addProperty(saveVectorDataButton_);

    clearVectorData_.onChange(CallMemberAction<OTBVectorDataWriterProcessor>(this, &OTBVectorDataWriterProcessor::clearVectorData));
    saveVectorDataButton_.onChange(CallMemberAction<OTBVectorDataWriterProcessor>(this, &OTBVectorDataWriterProcessor::saveVectorData));

    writer = VectorDataWriterType::New();
}

Processor* OTBVectorDataWriterProcessor::create() const {
    return new OTBVectorDataWriterProcessor();
}

bool OTBVectorDataWriterProcessor::isEndProcessor() const {
    return true;
}

bool OTBVectorDataWriterProcessor::isReady() const {
    return (inPort_.isConnected());
}

std::string OTBVectorDataWriterProcessor::getProcessorInfo() const {
    return "Saves Vector Data in various formats";
}

void OTBVectorDataWriterProcessor::process() {

}

void OTBVectorDataWriterProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
    hasVectorData = false;
}

void OTBVectorDataWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBVectorDataWriterProcessor::saveVectorData() {

    std::string filename = vectorDataFile_.get();
    if (!filename.empty()) {
        hasVectorData = true;
    }

    if(this->isReady() && hasVectorData) {
        writer->SetFileName(filename.c_str());
        writer->SetInput(inPort_.getData());
        try {
            writer->Update();
        } catch (itk::ExceptionObject& err) {
            LWARNING("ExceptionObject caught ! " << err);
            return;
        }
    } else if(!this->isReady()) {
        LWARNING("Writer inport not connected");
        return;
    } else if(!hasVectorData) {
        LWARNING("Vector Data file name not set correctly");
        return;
    }

    LINFO("'" << filename << "'" << " written succesfully!");
}

void OTBVectorDataWriterProcessor::clearVectorData() {

    if (hasVectorData) {
        hasVectorData = false;
    }

    vectorDataFile_.set("");
    LINFO("Vector Data have been cleared successfully!");

}

} // namespace

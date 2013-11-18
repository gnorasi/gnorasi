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

#include "otbvectordatareaderprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBVectorDataReaderProcessor::loggerCat_("voreen.OTBVectorDataReaderProcessor");

OTBVectorDataReaderProcessor::OTBVectorDataReaderProcessor()
    : Processor(),
      outPort_(Port::OUTPORT, "Vector Data Output", 0),
      VectorDataFile_("VectorDataFile_", "Input File", "Input File Name", VoreenApplication::app()->getUserDataPath(), "Vector Data File (*.SHP)"),
      clearFile_("clearButton", "Clear File")
{
    addPort(outPort_);

    addProperty(VectorDataFile_);
    addProperty(clearFile_);

    clearFile_.onChange(CallMemberAction<OTBVectorDataReaderProcessor>(this, &OTBVectorDataReaderProcessor::clearVectorData));
    VectorDataFile_.onChange(CallMemberAction<OTBVectorDataReaderProcessor>(this, &OTBVectorDataReaderProcessor::loadVectorData));

    hasFileName = false;
    reader = VectorDataFileReaderType::New();
}

Processor* OTBVectorDataReaderProcessor::create() const {
    return new OTBVectorDataReaderProcessor();
}

bool OTBVectorDataReaderProcessor::isReady() const {
    return (outPort_.isConnected());
}

std::string OTBVectorDataReaderProcessor::getProcessorInfo() const {
    return "Loads Vector Data files";
}

void OTBVectorDataReaderProcessor::process() {

}

void OTBVectorDataReaderProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void OTBVectorDataReaderProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBVectorDataReaderProcessor::loadVectorData() {

    std::string filename = VectorDataFile_.get();
    if (!filename.empty())
    {
        hasFileName = true;
    }

    if(hasFileName)
    {
        //Read vector data from file
        reader->SetFileName(filename);
        reader->Update();

        //Pass them to vector data outport
        outPort_.setData(reader->GetOutput());
        outPort_.setDataPath(filename);

        if (!outPort_.hasData()) {
            LERROR("No Vector Data passed!");
        }

    }else if(!hasFileName){
        LWARNING("Vector Data File Name Not Set");
        return;
    }
    LINFO("Vector Data loaded successfully!");
}

void OTBVectorDataReaderProcessor::clearVectorData() {

    if (hasFileName) {
        hasFileName = false;
        VectorDataFile_.set("");
        outPort_.setDataPath("");
    }
}

} // namespace

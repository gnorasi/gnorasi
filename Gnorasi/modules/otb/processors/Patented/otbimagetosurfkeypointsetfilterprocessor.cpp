/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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

#include "otbimagetosurfkeypointsetfilterprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageToSURFKeyPointSetFilter.h"

namespace voreen {
const std::string OTBImageToSURFKeyPointSetFilterProcessor::loggerCat_("voreen.OTBImageToSURFKeyPointSetFilterProcessor");

OTBImageToSURFKeyPointSetFilterProcessor::OTBImageToSURFKeyPointSetFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Image Input", 0),
   // outPort_(Port::OUTPORT, "Point Set Output", 0),
    m_octavesProperty("surfOctaves", "Octaves Property", 3),
    m_scalesProperty("surfScales", "Scales Property", 3,3,100)
{
    addPort(inPort_);
    //addPort(outPort_);

    addProperty(m_octavesProperty);
    addProperty(m_scalesProperty);
}

void OTBImageToSURFKeyPointSetFilterProcessor::update(){
    process();
}

Processor* OTBImageToSURFKeyPointSetFilterProcessor::create() const {
  return new OTBImageToSURFKeyPointSetFilterProcessor();
}

OTBImageToSURFKeyPointSetFilterProcessor::~OTBImageToSURFKeyPointSetFilterProcessor() {

}

void OTBImageToSURFKeyPointSetFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBImageToSURFKeyPointSetFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBImageToSURFKeyPointSetFilterProcessor::getProcessorInfo() const {

    return "Implements the SURF key point detector";
}

void OTBImageToSURFKeyPointSetFilterProcessor::process() {

    try
    {
        ImageType* imgtype = (ImageType*)inPort_.getData();
        if(!imgtype)
            return;

        typedef otb::ImageToSURFKeyPointSetFilter<ImageType,PointSetType>   ImageToSURFKeyPointSetFilterType;

        ImageToSURFKeyPointSetFilterType::Pointer filter = ImageToSURFKeyPointSetFilterType::New();

        filter->SetInput(imgtype);
        filter->SetOctavesNumber(m_octavesProperty.get());
        filter->SetScalesNumber(m_scalesProperty.get());

        filter->Update();

        //outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
        LERROR("Problem with SURF Feature Detector Extractor!");
        return;
    }}

} // namespace

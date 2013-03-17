/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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
 
#include "otbimagetosiftkeypointsetfilterprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbKeyPointDensityImageFilter.h"

namespace voreen {
const std::string OTBImageToSIFTKeyPointSetFilterProcessor::loggerCat_("voreen.OTBImageToSIFTKeyPointSetFilterProcessor");

OTBImageToSIFTKeyPointSetFilterProcessor::OTBImageToSIFTKeyPointSetFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Image Input", 0),
    outPort_(Port::OUTPORT, "Point Set Output", 0),
    m_octavesProperty("siftOctaves", "Octaves Property", 3),
    m_scalesProperty("siftScales", "Scales Property", 3, 3, 100),
    m_ratioProperty("siftRatio", "Ratio Property", 1.0f, 0.0f, 100.0f),
    m_thresholdProperty("siftThreshold", "Threshold Property",0.0f, 0.0f, 100.0f),
    m_neighborhoodProperty("neighborhood", "KeyPoint Density Neighborhood Property", 0, 0, 1000),
    m_updateProperty("update","Update")
{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(m_octavesProperty);
    addProperty(m_scalesProperty);
    addProperty(m_ratioProperty);
    addProperty(m_thresholdProperty);
    addProperty(m_neighborhoodProperty);
    addProperty(m_updateProperty);

    m_updateProperty.onChange(CallMemberAction<OTBImageToSIFTKeyPointSetFilterProcessor>(this, &OTBImageToSIFTKeyPointSetFilterProcessor::update));
    
//    filter = ImageToSIFTKeyPointSetFilterType::New();
//    densfilter = DensFilterType::New();
}

void OTBImageToSIFTKeyPointSetFilterProcessor::update(){
    process();
}

Processor* OTBImageToSIFTKeyPointSetFilterProcessor::create() const {
  return new OTBImageToSIFTKeyPointSetFilterProcessor();
}

OTBImageToSIFTKeyPointSetFilterProcessor::~OTBImageToSIFTKeyPointSetFilterProcessor() {

}

void OTBImageToSIFTKeyPointSetFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBImageToSIFTKeyPointSetFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBImageToSIFTKeyPointSetFilterProcessor::getProcessorInfo() const {
    
    return "Implements the SIFT key point detector";
}

void OTBImageToSIFTKeyPointSetFilterProcessor::process() {
  
    try
    {
        ImageType* imgtype = (ImageType*)inPort_.getData();
        if(!imgtype)
            return;

        typedef otb::ImageToSIFTKeyPointSetFilter<ImageType,PointSetType>   ImageToSIFTKeyPointSetFilterType;
       // typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, ImageToSIFTKeyPointSetFilterType> DensFilterType;

        ImageToSIFTKeyPointSetFilterType::Pointer filter = ImageToSIFTKeyPointSetFilterType::New();

        filter->SetInput(imgtype);
        filter->SetOctavesNumber(m_octavesProperty.get());
        filter->SetScalesNumber(m_scalesProperty.get());
        filter->SetDoGThreshold(m_thresholdProperty.get());
        filter->SetEdgeThreshold(m_ratioProperty.get());

        filter->Update();

        outPort_.setData(filter->GetOutput());

        //densfilter->SetDetector(filter);
        //densfilter->SetNeighborhoodRadius(m_neighborhoodProperty.get());
        //densfilter->SetInput(imgtype);

        //densfilter->Update();

       // outPort_.setData(densfilter->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with SIFT key point detector!");
	return;
    }
    
}

} // namespace

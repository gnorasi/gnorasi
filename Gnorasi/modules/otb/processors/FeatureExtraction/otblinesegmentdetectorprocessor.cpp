/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#include "otblinesegmentdetectorprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBLineSegmentDetectorProcessor::loggerCat_("voreen.OTBLineSegmentDetectorProcessor");

OTBLineSegmentDetectorProcessor::OTBLineSegmentDetectorProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Image Input", 0),
    outPort_(Port::OUTPORT, "Vector Data Output", 0)
    /*,
    m_thresholdProperty("threshold", "Threshold Property", 0.1),
    m_minimumRegionSizeProperty("minregion", "Minimum Region Property",0),
    m_precisionProperty("precision","Precision Property",0.1),
    m_directionAllowedProperty("directionAllowed", "Direction Allowed Property")*/
{
    addPort(inPort_);
    addPort(outPort_);

//    addProperty(m_thresholdProperty);
//    addProperty(m_minimumRegionSizeProperty);
//    addProperty(m_precisionProperty);
//    addProperty(m_directionAllowedProperty);

    filter = LSDFilterType::New();

}

void OTBLineSegmentDetectorProcessor::update(){
    process();
}

Processor* OTBLineSegmentDetectorProcessor::create() const {
  return new OTBLineSegmentDetectorProcessor();
}

OTBLineSegmentDetectorProcessor::~OTBLineSegmentDetectorProcessor() {

}

void OTBLineSegmentDetectorProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBLineSegmentDetectorProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBLineSegmentDetectorProcessor::getProcessorInfo() const {
    
    return "Implements a Line Segment Detector with false detection control using the a contrario method";
}

void OTBLineSegmentDetectorProcessor::process() {
  
    try
    {

        ImageType *img = inPort_.getData();
        if(!img)
            return;

        filter->SetInput(inPort_.getData());

        filter->Update();

        //outPort_.setData(filter->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with Line Segment Detector!");
	return;
    }
    
}

} // namespace

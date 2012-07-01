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

#include "otbmeanshiftimagesegmentationprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBMeanShiftSegmentationProcessor::loggerCat_("voreen.OTBMeanShiftSegmentationProcessor");

OTBMeanShiftSegmentationProcessor::OTBMeanShiftSegmentationProcessor()
    : OTBImageFilterProcessor(),
    spatialRadius_("spatialRadius", "Spatial Radius", 1),
    rangeRadius_("rangeRadius", "Range Radius", 30.0f, 0.0f, 10000.0f),
    minRegionSize_("minRegionSize", "Min Region Size", 20),
    scale_("scale", "Scale", 1.0f, 0.0f, 10000.0f),
    inPort_(Port::INPORT, "MultiBand Image", 0),
    outPort_(Port::OUTPORT, "Filtered MultiBand Image", 0),
    outPort2_(Port::OUTPORT, "Clustered MultiBand Image", 0),
    outPort3_(Port::OUTPORT, "Labeled Image", 0),
    outPort4_(Port::OUTPORT, "Boundary Image", 0),
    outPort5_(Port::OUTPORT, "Initial MultiBand Image", 0)
{
    addProperty(enableSwitch_);
    addProperty(spatialRadius_);
    addProperty(rangeRadius_);
    addProperty(minRegionSize_);
    addProperty(scale_);
    addPort(inPort_);
    addPort(outPort_);
    addPort(outPort2_);
    addPort(outPort3_);
    addPort(outPort4_);
    addPort(outPort5_);
    
    filter = FilterType::New();
}

Processor* OTBMeanShiftSegmentationProcessor::create() const {
  return new OTBMeanShiftSegmentationProcessor(); 
}

OTBMeanShiftSegmentationProcessor::~OTBMeanShiftSegmentationProcessor() {

}

void OTBMeanShiftSegmentationProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBMeanShiftSegmentationProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBMeanShiftSegmentationProcessor::getProcessorInfo() const {
    
    return "Mean Shift Image Segmentation Processor";
}

bool OTBMeanShiftSegmentationProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected()) return false;
    
    if(!outPort_.isConnected() && !outPort2_.isConnected() && !outPort3_.isConnected() &&
        !outPort4_.isConnected() && !outPort5_.isConnected()) return false;

    return true;
}

void OTBMeanShiftSegmentationProcessor::process() {
    
    //LINFO("Mean Shift Segmentation Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
	return;
    }
    
    try
    {
	filter->SetSpatialRadius(spatialRadius_.get());
	filter->SetRangeRadius(rangeRadius_.get());
	filter->SetMinimumRegionSize(minRegionSize_.get());
	filter->SetScale(scale_.get());
	filter->SetNumberOfThreads(1);
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
	outPort2_.setData(filter->GetClusteredOutput());
	outPort3_.setData(filter->GetLabeledClusteredOutput());
	outPort4_.setData(filter->GetClusterBoundariesOutput());
	outPort5_.setData(inPort_.getData());
	LINFO("Mean Shift Segmentation Connected!");
    }
    catch (int e)
    {
	LERROR("Error in Mean Shift Segmentation");
	return;
    }

    
}

void OTBMeanShiftSegmentationProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {
   
    outport->setData(inport->getData());
}


} // namespace

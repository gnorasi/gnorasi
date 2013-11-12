/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.               *
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

#include "otbmultichannelextractroiprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBMultiChannelExtractROIProcessor::loggerCat_("voreen.OTBMultiChannelExtractROIProcessor");

OTBMultiChannelExtractROIProcessor::OTBMultiChannelExtractROIProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "Vector Image Output", 0),
    m_useSingleChannelProperty("useSingleChannel", "Use Single Channel"),
    m_singleChannelProperty("singleChannel", "Single Channel",2),
    m_useMultipleChannelProperty("multipleChannel", "Use Multiple Channels"),
    m_channelFromProperty("channelFrom", "Channel From",0,0,10),
    m_channelEndProperty("channelEnd", "Channel To",0,0,10),
    useSpatialSubsetting_("useSpatialSubsetting_", "Use Spatial Subsetting:", false),
    startX_("startX_", "Start Point X:",0,0,3000),
    startY_("startY_", "Start Point Y:",0,0,3000),
    sizeX_("sizeX_", "Set Extract Area Size X:",100,1,3000),
    sizeY_("sizeY_", "Set Extract Area Size Y:",100,1,3000)

{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);

    addProperty(m_useSingleChannelProperty);
    addProperty(m_singleChannelProperty);
    addProperty(m_useMultipleChannelProperty);
    addProperty(m_channelFromProperty);
    addProperty(m_channelEndProperty);
    addProperty(useSpatialSubsetting_);
    addProperty(startX_);
    addProperty(startY_);
    addProperty(sizeX_);
    addProperty(sizeY_);

    m_singleChannelProperty.setVisible(false);
    m_channelFromProperty.setVisible(false);
    m_channelEndProperty.setVisible(false);
    startX_.setVisible(false);
    startY_.setVisible(false);
    sizeX_.setVisible(false);
    sizeY_.setVisible(false);

    m_useSingleChannelProperty.onChange(CallMemberAction<OTBMultiChannelExtractROIProcessor>(this,
                                            &OTBMultiChannelExtractROIProcessor::updateUseSingleChannel));
    m_useMultipleChannelProperty.onChange(CallMemberAction<OTBMultiChannelExtractROIProcessor>(this,
                                            &OTBMultiChannelExtractROIProcessor::updateUseMultipleChannel));
    useSpatialSubsetting_.onChange(CallMemberAction<OTBMultiChannelExtractROIProcessor>(this,
                                            &OTBMultiChannelExtractROIProcessor::updateUseSpatialSubsetting));

    multichannelextractor = MultiChannelExtractROIType::New();
}

void OTBMultiChannelExtractROIProcessor::updateUseSingleChannel(){
    bool val = m_useSingleChannelProperty.get();

    m_singleChannelProperty.setVisible(val);

    if(m_useMultipleChannelProperty.get())
        m_useMultipleChannelProperty.set(!val);
}

void OTBMultiChannelExtractROIProcessor::updateUseMultipleChannel(){
    bool val = m_useMultipleChannelProperty.get();

    m_channelEndProperty.setVisible(val);
    m_channelFromProperty.setVisible(val);

    if(m_useSingleChannelProperty.get())
        m_useSingleChannelProperty.set(!val);
}

void OTBMultiChannelExtractROIProcessor::updateUseSpatialSubsetting(){
        startX_.setVisible(useSpatialSubsetting_.get());
        startY_.setVisible(useSpatialSubsetting_.get());
        sizeX_.setVisible(useSpatialSubsetting_.get());
        sizeY_.setVisible(useSpatialSubsetting_.get());
}

void OTBMultiChannelExtractROIProcessor::update(){
    process();
}

Processor* OTBMultiChannelExtractROIProcessor::create() const {
  return new OTBMultiChannelExtractROIProcessor();
}

OTBMultiChannelExtractROIProcessor::~OTBMultiChannelExtractROIProcessor() {

}

void OTBMultiChannelExtractROIProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBMultiChannelExtractROIProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBMultiChannelExtractROIProcessor::getProcessorInfo() const {
    
    return "Extract a spatial or spectral subset of a multi-channel image";
}

void OTBMultiChannelExtractROIProcessor::process() {
  
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        LINFO("Multi Channel Extract ROI filter disabled!");
        return;
    }

    try
    {
        multichannelextractor->SetInput(inPort_.getData());

        if(m_useSingleChannelProperty.get()) {
            multichannelextractor->SetChannel(m_singleChannelProperty.get());
            LINFO("Single channel extraction enabled! [" << m_singleChannelProperty.get() << "]");
        }

        if(m_useMultipleChannelProperty.get()){
            multichannelextractor->SetFirstChannel(m_channelFromProperty.get());
            multichannelextractor->SetLastChannel(m_channelEndProperty.get());
            LINFO("Multi-channel extraction enabled! [" << m_channelFromProperty.get() <<
                  ".." << m_channelEndProperty.get() << "]");
        }

        //Spatial Subsetting functionality added
        if(useSpatialSubsetting_.get()) {
            multichannelextractor->SetStartX(startX_.get());
            multichannelextractor->SetStartY(startY_.get());
            multichannelextractor->SetSizeX(sizeX_.get());
            multichannelextractor->SetSizeY(sizeY_.get());
            LINFO("Spatial Subsetting enabled!");
       }

//        multichannelextractor->UpdateOutputInformation();
//        multichannelextractor->UpdateLargestPossibleRegion();

        multichannelextractor->Update();
        outPort_.setData(multichannelextractor->GetOutput());

        if(useSpatialSubsetting_.get()) {
            LINFO("Extraction region size: " << multichannelextractor->GetExtractionRegion().GetSize());
            LINFO("Extraction region index: " << multichannelextractor->GetExtractionRegion().GetIndex());
        }
        LINFO("Multi Channel Extract ROI filter connected!");
    }
    catch (int e)
    {
    LERROR("Problem with Multi Channel Extract ROI!");
	return;
    }
    
}
void OTBMultiChannelExtractROIProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

} // namespace

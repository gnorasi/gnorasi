/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.				*
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
    inPort_(Port::INPORT, "Vector Image Input 1", 0),
    outPort_(Port::OUTPORT, "Vector Image Output", 0),
    m_useSingleChannelProperty("useSingleChannel", "Use Single Channel Property"),
    m_singleChannelProperty("singleChannel", "Single Channel Property",2),
    m_useMultipleChannelProperty("multipleChannel", "Use Multiple Channels Property"),
    m_channelFromProperty("channelFrom", "Channel From Proprety",0,0,20),
    m_channelEndProperty("channelEnd", "Channel End Proprety",2,0,20)

{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(m_useSingleChannelProperty);
    addProperty(m_singleChannelProperty);
    addProperty(m_useMultipleChannelProperty);
    addProperty(m_channelFromProperty);
    addProperty(m_channelEndProperty);

    m_singleChannelProperty.setVisible(false);
    m_channelFromProperty.setVisible(false);
    m_channelEndProperty.setVisible(false);

    m_useSingleChannelProperty.onChange(CallMemberAction<OTBMultiChannelExtractROIProcessor>(this,
                                            &OTBMultiChannelExtractROIProcessor::updateUseSingleChannel));
    m_useMultipleChannelProperty.onChange(CallMemberAction<OTBMultiChannelExtractROIProcessor>(this,
                                            &OTBMultiChannelExtractROIProcessor::updateUseMultipleChannel));

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
  
    try
    {
        InputImageType *img = inPort_.getData();
        if(!img)
            return;

        MultiChannelExtractROIType::Pointer multichannelextractor = MultiChannelExtractROIType::New();

        multichannelextractor->SetInput(img);

        if(m_useSingleChannelProperty.get())
            multichannelextractor->SetChannel(m_singleChannelProperty.get());

        if(m_useMultipleChannelProperty.get()){
            multichannelextractor->SetFirstChannel(m_channelFromProperty.get());
            multichannelextractor->SetLastChannel(m_channelEndProperty.get());
        }

        multichannelextractor->Update();

        outPort_.setData(multichannelextractor->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with cbami detector!");
	return;
    }
    
}

} // namespace

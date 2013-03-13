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

#include "otblaindviprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBLAINDVIProcessor::loggerCat_("voreen.OTBLAINDVIProcessor");

OTBLAINDVIProcessor::OTBLAINDVIProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "NDVI Image Output", 0),
    redBand_("red", "Red Band Index", 1,1,3),
    nirBand_("nir", "NIR Band Index", 4),
    m_ndviSoilProperty("soil", "NDVI Soil Property", 0.0,0.0, 256.0),
    m_NdviInfProperty("inf", "NDVI Inf Property", 0.0,0.0, 256.0),
    m_ExtinctionCoefficientProperty("Coeffictient", "Coefficient Extinction Property", 0.0,0.0, 256.0)
{
    addPort(inPort_);
    addPort(outPort_);
    redBand_.onChange(CallMemberAction<OTBLAINDVIProcessor>(this,
                    &OTBLAINDVIProcessor::setRedBandIndex));
    addProperty(redBand_);
    nirBand_.onChange(CallMemberAction<OTBLAINDVIProcessor>(this,
                    &OTBLAINDVIProcessor::setNirBandIndex));
    addProperty(nirBand_);
    addProperty(m_ndviSoilProperty);
    addProperty(m_NdviInfProperty);
    addProperty(m_ExtinctionCoefficientProperty);

    m_NdviInfProperty.onChange(CallMemberAction<OTBLAINDVIProcessor>(this,
                                            &OTBLAINDVIProcessor::updateInfProperty));
    m_ExtinctionCoefficientProperty.onChange(CallMemberAction<OTBLAINDVIProcessor>(this,
                                            &OTBLAINDVIProcessor::updateCoefficientProperty));
    m_ndviSoilProperty.onChange(CallMemberAction<OTBLAINDVIProcessor>(this,
                                            &OTBLAINDVIProcessor::updateSoilProperty));
    
    filter = LAINDVIImageFilterType::New();
}

Processor* OTBLAINDVIProcessor::create() const {
  return new OTBLAINDVIProcessor();
}

OTBLAINDVIProcessor::~OTBLAINDVIProcessor() {

}

void OTBLAINDVIProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBLAINDVIProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBLAINDVIProcessor::getProcessorInfo() const {
    
    return "Calculate LAI from NDVI for Vector Image";
}

void OTBLAINDVIProcessor::process() {
  
    try
    {
	filter->SetRedIndex(redBand_.get());
    filter->SetNIRIndex(nirBand_.get());
    filter->GetFunctor().SetExtinctionCoefficient(m_ExtinctionCoefficientProperty.get());
    filter->GetFunctor().SetNdviInf(m_NdviInfProperty.get());
    filter->GetFunctor().SetNdviSoil(m_ndviSoilProperty.get());
	filter->SetInput(inPort_.getData());
    filter->Update();
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
    LERROR("Problem with LAI from NDVI calculation!");
	return;
    }
    
}

void OTBLAINDVIProcessor::setRedBandIndex() {
    filter->SetRedIndex(redBand_.get());
}

void OTBLAINDVIProcessor::setNirBandIndex() {
    filter->SetNIRIndex(nirBand_.get());
}


void OTBLAINDVIProcessor::updateCoefficientProperty(){
    double val = m_ExtinctionCoefficientProperty.get();

    filter->GetFunctor().SetExtinctionCoefficient(val);
}


void OTBLAINDVIProcessor::updateInfProperty(){
    double val = m_NdviInfProperty.get();

    filter->GetFunctor().SetNdviInf(val);
}

void OTBLAINDVIProcessor::updateSoilProperty(){
    double val = m_ndviSoilProperty.get();

    filter->GetFunctor().SetNdviSoil(val);
}

} // namespace

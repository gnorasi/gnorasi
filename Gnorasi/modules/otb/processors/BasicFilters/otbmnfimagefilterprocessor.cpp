/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#include "otbmnfimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"
namespace voreen {

const std::string OTBMNFImageFilterProcessor::loggerCat_("voreen.OTBMNFImageFilterProcessor");

OTBMNFImageFilterProcessor::OTBMNFImageFilterProcessor()
    :OTBImageFilterProcessor()
//      normalization_("setNormalization", "Normalization:", false),
//      numberOfPrincipalComponentsReq_("numberOfPrincipalComponentsReq","Number of Principal Components:", 1,1,3),
//      radius_("noiseFilterRadius", "Noise Filter Radius:", 1),
//      inPort_(Port::INPORT, "IN MultiBand Image", 0),
//      outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)

{
//      addProperty(enableSwitch_);
//      addProperty(numberOfPrincipalComponentsReq_);
//      addProperty(normalization_);
//      addProperty(radius_);
//      addPort(inPort_);
//      addPort(outPort_);

//      filter = FilterType::New();
}

Processor* OTBMNFImageFilterProcessor::create() const {
    return new OTBMNFImageFilterProcessor();
}

OTBMNFImageFilterProcessor::~OTBMNFImageFilterProcessor() {

}

void OTBMNFImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBMNFImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBMNFImageFilterProcessor::getProcessorInfo() const {
    return "MNF Image Filter";
}

void OTBMNFImageFilterProcessor::process() {

    //check bypass switch
//    if (!enableSwitch_.get()) {
//        bypass(&inPort_, &outPort_);
//        return;
//    }

    try
    {
//        filter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsReq_.get());
//        NoiseFilterType::RadiusType radius;
//        radius[0] = radius_.get();
//        radius[1] = radius_.get();
//        filter->GetNoiseImageFilter()->SetRadius(radius);
//        filter->SetNormalization(normalization_.get());
//        filter->SetInput(inPort_.getData());
//        outPort_.setData(filter->GetOutput());
//        LINFO("MNF Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in MNF Image Filter");
        return;
    }

}

//void OTBMNFImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

//    outport->setData(inport->getData());
//}

}   // namespace

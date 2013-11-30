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

#include "otblmvmpansharpeningfusionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLmvmPanSharpeningFusionImageFilterProcessor::loggerCat_("voreen.OTBLmvmPanSharpeningFusionImageFilterProcessor");

OTBLmvmPanSharpeningFusionImageFilterProcessor::OTBLmvmPanSharpeningFusionImageFilterProcessor()
    :OTBImageFilterProcessor(),
    filterRadius_("filterRadius_", "Filter Radius", 3),
    inPort_(Port::INPORT, "OTBImage.Inport",0),
    vectorInPort_(Port::INPORT, "IN Multiband Image", 0),
    vectorOutPort_(Port::OUTPORT, "OUT Multiband image", 0)
{
    addProperty(enableSwitch_);
    addProperty(filterRadius_);

    addPort(inPort_);
    addPort(vectorInPort_);
    addPort(vectorOutPort_);
}

Processor* OTBLmvmPanSharpeningFusionImageFilterProcessor::create() const {
return new OTBLmvmPanSharpeningFusionImageFilterProcessor();
}

OTBLmvmPanSharpeningFusionImageFilterProcessor::~OTBLmvmPanSharpeningFusionImageFilterProcessor() {

}

void OTBLmvmPanSharpeningFusionImageFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBLmvmPanSharpeningFusionImageFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBLmvmPanSharpeningFusionImageFilterProcessor::getProcessorInfo() const {

  return "LMVM Pan Sharpening Fusion Image Filter";
}

void OTBLmvmPanSharpeningFusionImageFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
//        bypass(&inPort_, &outPort_);
//        return;
    }

    try
    {

        fusion->SetPanInput(inPort_.getData());
        fusion->SetXsInput(vectorInPort_.getData());

        int rad = filterRadius_.get();
        ImageType::SizeType indexRadius;

//        indexRadius.Fill(rad);
        indexRadius[0] = rad; // radius along x
        indexRadius[1] = rad; // radius along y

        fusion->SetRadius( indexRadius );
        fusion->Update();

        caster->SetInput(fusion->GetOutput());
        vectorOutPort_.setData(caster->GetOutput());

    }
    catch (int e)
    {
        LERROR("LMVM Pan Sharpening Fusion Image Filter");
    return;
    }
}

} //namespace



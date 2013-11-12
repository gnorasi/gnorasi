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

#include "otbharrisimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBHarrisImageFilterProcessor::loggerCat_("voreen.OTBHarrisImageFilterProcessor");

OTBHarrisImageFilterProcessor::OTBHarrisImageFilterProcessor()
    :OTBImageFilterProcessor(),
      sigmaD_("sigmaD_", "Sigma D: ", 1.5f, 0.0f, 10.0f),
      sigmaI_("sigmaI_", "Sigma I: ", 2.0f, 0.0f, 10.0f),
      alpha_("alpha_", "Alpha: ",  0.1f, 0.0f, 1.0f),
      inPort_(Port::INPORT, "OTBImage.Inport",0),
      outPort_(Port::OUTPORT, "OTBImage.Outport",0)
{
    addProperty(enableSwitch_);
    addProperty(sigmaD_);
    addProperty(sigmaI_);
    addProperty(alpha_);
    addPort(inPort_);
    addPort(outPort_);

    filter = DerivativeImageFilterType::New();
    rescaler = RescaleImageFilterType::New();
    charToPortCaster = charToPortCastingFilterType::New();
}

Processor* OTBHarrisImageFilterProcessor::create() const {
    return new OTBHarrisImageFilterProcessor();
}

OTBHarrisImageFilterProcessor::~OTBHarrisImageFilterProcessor() {

}

void OTBHarrisImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBHarrisImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBHarrisImageFilterProcessor::getProcessorInfo() const {
    return "Harris Image Filter Processor";
}

void OTBHarrisImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Harris Image Filter Enabled!");

    try
    {
        filter->SetInput(inPort_.getData());

        filter->SetSigmaD(sigmaD_.get());
        filter->SetSigmaI(sigmaI_.get());
        filter->SetAlpha(alpha_.get());
//        filter->Update();

        rescaler->SetInput(filter->GetOutput());

        //Note the use of the \doxygen{NumericTraits} class which
        // allows to define a number of type-related constant in a generic
        // way. The use of traits is a fundamental characteristic of generic
        // programming

        rescaler->SetOutputMinimum(itk::NumericTraits<CharPixelType>::min());
        rescaler->SetOutputMaximum(itk::NumericTraits<CharPixelType>::max());
        rescaler->Update();

        charToPortCaster->SetInput(rescaler->GetOutput());
        outPort_.setData(charToPortCaster->GetOutput());

        LINFO("Harris Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Harris Image Filter!");
        return;
    }
}

}   // namespace


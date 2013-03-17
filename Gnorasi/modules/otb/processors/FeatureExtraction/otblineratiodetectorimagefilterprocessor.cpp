/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
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
 
#include "otblineratiodetectorimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "itkNumericTraits.h"

namespace voreen {

const std::string OTBLineRatioDetectorImageFilterProcessor::loggerCat_("voreen.OTBLineRatioDetectorImageFilterProcessor");

OTBLineRatioDetectorImageFilterProcessor::OTBLineRatioDetectorImageFilterProcessor()
    :OTBImageFilterProcessor(),
    lengthLine_("lengthLine", "lengthLine Property", 100, 0, 4096),
    widthLine_("widthLine", "widthLine Property", 100, 0, 4096),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(lengthLine_);
    addProperty(widthLine_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    //rescaler_Filter = RescalerFilterType::New();
}

Processor* OTBLineRatioDetectorImageFilterProcessor::create() const {
return new OTBLineRatioDetectorImageFilterProcessor();
}

OTBLineRatioDetectorImageFilterProcessor::~OTBLineRatioDetectorImageFilterProcessor() {

}

void OTBLineRatioDetectorImageFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBLineRatioDetectorImageFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBLineRatioDetectorImageFilterProcessor::getProcessorInfo() const {

return "OTBLineRatioDetectorImageFilter Processor";
}

void OTBLineRatioDetectorImageFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        //rescaler_Filter->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
        //rescaler_Filter->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

        filter->SetInput(inPort_.getData());

        //rescaler_Filter->SetInput(filter->GetOutput());

        //outPort_.setData(rescaler_Filter->GetOutput());
        outPort_.setData(filter->GetOutput());//outPort_.setData(rescaler_Filter->GetOutput());
        outPort_.setData(filter->GetOutput());

        filter->SetLengthLine(lengthLine_.get());
        filter->SetWidthLine(widthLine_.get());

        filter->Update();


    }
    catch (int e)
    {
        LERROR("Problem with LineRatioDetectorImage Filter!");
    return;
    }
}

} //namespace


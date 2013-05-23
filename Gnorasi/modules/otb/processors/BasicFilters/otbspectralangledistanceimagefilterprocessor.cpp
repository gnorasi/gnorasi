/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.             	*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#include "otbspectralangledistanceimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBSpectralAngleDistanceImageFilterProcessor::loggerCat_("voreen.OTBSpectralAngleDistanceImageFilterProcessor");

OTBSpectralAngleDistanceImageFilterProcessor::OTBSpectralAngleDistanceImageFilterProcessor()
    :OTBImageFilterProcessor(),
      size_("size","Pixel Type Size:", 1,1,4),
      refPixel0_("refPixel0","Reference Pixel #1:", 1,1,1000),
      refPixel1_("refPixel1","Reference Pixel #2:", 1,1,1000),
      refPixel2_("refPixel2","Reference Pixel #3:", 1,1,1000),
      refPixel3_("refPixel3","Reference Pixel #4:", 1,1,1000),
      inPort_(Port::INPORT, "IN MultiBand Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)

{
      addProperty(enableSwitch_);
      addProperty(size_);
      addProperty(refPixel0_);
      addProperty(refPixel1_);
      addProperty(refPixel2_);
      addProperty(refPixel3_);
      addPort(inPort_);
      addPort(outPort_);

      refPixel1_.setVisible(false);
      refPixel2_.setVisible(false);
      refPixel3_.setVisible(false);

      size_.onChange(CallMemberAction<OTBSpectralAngleDistanceImageFilterProcessor>(this,&OTBSpectralAngleDistanceImageFilterProcessor::updateSpectralAngleDistance));
      filter = FilterType::New();

}

Processor* OTBSpectralAngleDistanceImageFilterProcessor::create() const {
    return new OTBSpectralAngleDistanceImageFilterProcessor();
}

OTBSpectralAngleDistanceImageFilterProcessor::~OTBSpectralAngleDistanceImageFilterProcessor() {

}

void OTBSpectralAngleDistanceImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBSpectralAngleDistanceImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBSpectralAngleDistanceImageFilterProcessor::getProcessorInfo() const {
    return "Spectral Angle Distance Image Filter";
}

void OTBSpectralAngleDistanceImageFilterProcessor::updateSpectralAngleDistance() {

    if (size_.get()==1)
    {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(false);
        refPixel2_.setVisible(false);
        refPixel3_.setVisible(false);
    } else if (size_.get()==2)
    {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(false);
        refPixel3_.setVisible(false);
    } else if (size_.get()==3)
    {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(false);
    } else if (size_.get()==4)
    {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
    }
}

void OTBSpectralAngleDistanceImageFilterProcessor::process() {

    //check bypass switch
//    if (!enableSwitch_.get()) {
//        bypass(&inPort_, &outPort_);
//        return;
//    }

    try
    {
        MultiSpectralImageType::PixelType pixelRef;

        if (size_.get()==1)
        {
            pixelRef.SetSize(1);
            pixelRef[0] = refPixel0_.get();

        } else if (size_.get()==2)
        {
            pixelRef.SetSize(2);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
        } else if (size_.get()==3)
        {
            pixelRef.SetSize(3);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();

        } else if (size_.get()==4)
        {
            pixelRef.SetSize(4);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            pixelRef[3] = refPixel3_.get();
        }

        filter->SetReferencePixel(pixelRef);
        filter->SetInput(inPort_.getData());
        outPort_.setData(filter->GetOutput());

        LINFO("Spectral Angle Distance Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Spectral Angle Distance Image Filter");
        return;
    }

}

//void OTBSpectralAngleDistanceImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBImagePort *outport) {

//    outport->setData(inport->getData());
//}

}   // namespace

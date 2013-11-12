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
      refPixel0_("refPixel0","Reference Pixel #1:", 1,1,3000),
      refPixel1_("refPixel1","Reference Pixel #2:", 1,1,3000),
      refPixel2_("refPixel2","Reference Pixel #3:", 1,1,3000),
      refPixel3_("refPixel3","Reference Pixel #4:", 1,1,3000),
      refPixel4_("refPixel4","Reference Pixel #5:", 1,1,3000),
      refPixel5_("refPixel5","Reference Pixel #6:", 1,1,3000),
      refPixel6_("refPixel6","Reference Pixel #7:", 1,1,3000),
      refPixel7_("refPixel7","Reference Pixel #8:", 1,1,3000),
      inPort_(Port::INPORT, "IN MultiBand Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)

{
    addProperty(refPixel0_);
    addProperty(refPixel1_);
    addProperty(refPixel2_);
    addProperty(refPixel3_);
    addProperty(refPixel4_);
    addProperty(refPixel5_);
    addProperty(refPixel6_);
    addProperty(refPixel7_);
    addPort(inPort_);
    addPort(outPort_);

    refPixel0_.setVisible(false);
    refPixel1_.setVisible(false);
    refPixel2_.setVisible(false);
    refPixel3_.setVisible(false);
    refPixel4_.setVisible(false);
    refPixel5_.setVisible(false);
    refPixel6_.setVisible(false);
    refPixel7_.setVisible(false);

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

void OTBSpectralAngleDistanceImageFilterProcessor::updateBands(int bands) {

    //Display the corresponding number of properties
    //depending on input image's spectral bands.
    switch (bands) {
    case 1: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(false);
        refPixel2_.setVisible(false);
        refPixel3_.setVisible(false);
        refPixel4_.setVisible(false);
        refPixel5_.setVisible(false);
        refPixel6_.setVisible(false);
        refPixel7_.setVisible(false);
        break;
    }
    case 2: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(false);
        refPixel3_.setVisible(false);
        refPixel4_.setVisible(false);
        refPixel5_.setVisible(false);
        refPixel6_.setVisible(false);
        refPixel7_.setVisible(false);
        break;
    }
    case 3: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(false);
        refPixel4_.setVisible(false);
        refPixel5_.setVisible(false);
        refPixel6_.setVisible(false);
        refPixel7_.setVisible(false);
        break;
    }
    case 4: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
        refPixel4_.setVisible(false);
        refPixel5_.setVisible(false);
        refPixel6_.setVisible(false);
        refPixel7_.setVisible(false);
        break;
    }
    case 5: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
        refPixel4_.setVisible(true);
        refPixel5_.setVisible(false);
        refPixel6_.setVisible(false);
        refPixel7_.setVisible(false);
        break;
    }
    case 6: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
        refPixel4_.setVisible(true);
        refPixel5_.setVisible(true);
        refPixel6_.setVisible(false);
        refPixel7_.setVisible(false);
        break;
    }
    case 7: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
        refPixel4_.setVisible(true);
        refPixel5_.setVisible(true);
        refPixel6_.setVisible(true);
        refPixel7_.setVisible(false);
        break;
    }
    case 8: {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
        refPixel4_.setVisible(true);
        refPixel5_.setVisible(true);
        refPixel6_.setVisible(true);
        refPixel7_.setVisible(true);
        break;
    }
    }
    LINFO("Pixel size has been updated!");
}

void OTBSpectralAngleDistanceImageFilterProcessor::process() {

    try
    {
        //Detect the number of spectral bands the input image has.
        nbBands = inPort_.getData()->GetNumberOfComponentsPerPixel();
        LINFO("Number of Bands detected: " << nbBands);
        updateBands(nbBands);

        MultiSpectralImageType::PixelType pixelRef;

        //Pass the parameters to filter
        //depending on input image's spectral bands.
        switch (nbBands) {
        case 1: {
            pixelRef.SetSize(1);
            pixelRef[0] = refPixel0_.get();
            break;
        }
        case 2: {
            pixelRef.SetSize(2);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            break;
        }
        case 3: {
            pixelRef.SetSize(3);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            break;
        }
        case 4: {
            pixelRef.SetSize(4);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            pixelRef[3] = refPixel3_.get();
            break;
        }
        case 5: {
            pixelRef.SetSize(5);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            pixelRef[3] = refPixel3_.get();
            pixelRef[4] = refPixel4_.get();
            break;
        }
        case 6: {
            pixelRef.SetSize(6);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            pixelRef[3] = refPixel3_.get();
            pixelRef[4] = refPixel4_.get();
            pixelRef[5] = refPixel5_.get();
            break;
        }
        case 7: {
            pixelRef.SetSize(7);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            pixelRef[3] = refPixel3_.get();
            pixelRef[4] = refPixel4_.get();
            pixelRef[5] = refPixel5_.get();
            pixelRef[6] = refPixel6_.get();
            break;
        }
        case 8: {
            pixelRef.SetSize(8);
            pixelRef[0] = refPixel0_.get();
            pixelRef[1] = refPixel1_.get();
            pixelRef[2] = refPixel2_.get();
            pixelRef[3] = refPixel3_.get();
            pixelRef[4] = refPixel4_.get();
            pixelRef[5] = refPixel5_.get();
            pixelRef[6] = refPixel6_.get();
            pixelRef[7] = refPixel7_.get();
            break;
        }
        }

        filter->SetInput(inPort_.getData());
        filter->SetReferencePixel(pixelRef);
        filter->UpdateLargestPossibleRegion();
        filter->Update();
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

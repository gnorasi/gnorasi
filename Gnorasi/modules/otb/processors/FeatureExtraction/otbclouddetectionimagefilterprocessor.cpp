/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#include "otbclouddetectionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBCloudDetectionImageFilterProcessor::loggerCat_("voreen.OTBCloudDetectionImageFilterProcessor");

OTBCloudDetectionImageFilterProcessor::OTBCloudDetectionImageFilterProcessor()
    :OTBImageFilterProcessor(),
      gaussianVarianceValue_("gaussianVarianceValue", "Gaussian Variance Value:", 1.0f, 0.0f, 1024.0f),
      maxThreshold_("max", "Maximum Threshold:", 1.0f, 0.0f, 1.0f),
      minThreshold_("min", "Minimum Threshold:", 0.0f, 0.0f, 1.0f),
      refPixel0_("refPixel0","Reference Pixel #1:", 1,1,3000),
      refPixel1_("refPixel1","Reference Pixel #2:", 1,1,3000),
      refPixel2_("refPixel2","Reference Pixel #3:", 1,1,3000),
      refPixel3_("refPixel3","Reference Pixel #4:", 1,1,3000),
      refPixel4_("refPixel4","Reference Pixel #5:", 1,1,3000),
      refPixel5_("refPixel5","Reference Pixel #6:", 1,1,3000),
      refPixel6_("refPixel6","Reference Pixel #7:", 1,1,3000),
      refPixel7_("refPixel7","Reference Pixel #8:", 1,1,3000),
      inPort_(Port::INPORT, "IN Vector Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)
{
    addProperty(gaussianVarianceValue_);
    addProperty(maxThreshold_);
    addProperty(minThreshold_);
    addProperty(refPixel0_);
    addProperty(refPixel1_);
    addProperty(refPixel2_);
    addProperty(refPixel3_);
    addProperty(refPixel4_);
    addProperty(refPixel5_);
    addProperty(refPixel6_);
    addProperty(refPixel7_);

    refPixel0_.setVisible(false);
    refPixel1_.setVisible(false);
    refPixel2_.setVisible(false);
    refPixel3_.setVisible(false);
    refPixel4_.setVisible(false);
    refPixel5_.setVisible(false);
    refPixel6_.setVisible(false);
    refPixel7_.setVisible(false);

    addPort(inPort_);
    addPort(outPort_);

    filter = CloudDetectionFilterType::New();
}

Processor* OTBCloudDetectionImageFilterProcessor::create() const {
    return new OTBCloudDetectionImageFilterProcessor();
}

OTBCloudDetectionImageFilterProcessor::~OTBCloudDetectionImageFilterProcessor() {

}

void OTBCloudDetectionImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBCloudDetectionImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBCloudDetectionImageFilterProcessor::getProcessorInfo() const {
    return "Cloud Detection Image Filter Processor";
}

void OTBCloudDetectionImageFilterProcessor::updateBands(int bands) {

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

void OTBCloudDetectionImageFilterProcessor::process() {

    try
    {
        //REFERENCE PIXELS PROPERTY

        // *CLASSIC METHOD
        //obtain number of spectral bands manually from user by analyzing a stringProperty
        //*it may trigger an error if wrong number of arguments are given*

//        std::string text = referencePixelProperty.get();
//        std::vector<std::string> strings;
//        std::istringstream f(text);
//        std::string s;
//        while (std::getline(f, s, ';')) {
//            std::cout << s << std::endl;
//            strings.push_back(s);
//        }

//        for(int i  = 0; i < strings.size(); i++){
//            std::string cs = strings.at(i);
//            const char *c = cs.c_str();
//            referencePixel.SetElement(i, ::atof(c));
//        }

        // *DYNAMIC METHOD
        //get number of spectral bands from image after the processor is connected
        //and display the equivalent number of intPropertiesto get the reference pixel values

        //Detect the number of spectral bands the input image has.
        nbBands = inPort_.getData()->GetNumberOfComponentsPerPixel();
        LINFO("Number of Bands detected: " << nbBands);
        updateBands(nbBands);

        VectorImageType::PixelType pixelRef;

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

        //SET FILTER PARAMETERS
        filter->SetInput(inPort_.getData());

        filter->SetReferencePixel(pixelRef);
        filter->SetVariance(gaussianVarianceValue_.get());
        filter->SetMinThreshold(minThreshold_.get());
        filter->SetMaxThreshold(maxThreshold_.get());

        filter->UpdateLargestPossibleRegion();
        filter->Update();

        outPort_.setData(filter->GetOutput());
        LINFO("Cloud Detection Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Cloud Detection Image Filter");
        return;
    }

}

}   // namespace



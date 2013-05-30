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
      //referencePixelProperty("referencePixel", "Reference Pixel Property"),
      refPixel0_("refPixel0","Reference Pixel Band 1:", 1,1,3000),
      refPixel1_("refPixel1","Reference Pixel Band 2:", 1,1,3000),
      refPixel2_("refPixel2","Reference Pixel Band 3:", 1,1,3000),
      refPixel3_("refPixel3","Reference Pixel Band 4:", 1,1,3000),

      inPort_(Port::INPORT, "IN Vector Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)
{
    addProperty(enableSwitch_);
    addProperty(gaussianVarianceValue_);
    addProperty(maxThreshold_);
    addProperty(minThreshold_);
    //addProperty(referencePixelProperty);
    addProperty(refPixel0_);
    addProperty(refPixel1_);
    addProperty(refPixel2_);
    addProperty(refPixel3_);

    refPixel0_.setVisible(false);
    refPixel1_.setVisible(false);
    refPixel2_.setVisible(false);
    refPixel3_.setVisible(false);

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

void OTBCloudDetectionImageFilterProcessor::updatePixelSize() {

    //get the number of spectral band directly from the image
    //and set the visibility of intProperties according to this
    size = inPort_.getData()->GetNumberOfComponentsPerPixel();

    if (size==1) {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(false);
        refPixel2_.setVisible(false);
        refPixel3_.setVisible(false);
    } else if (size==2) {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(false);
        refPixel3_.setVisible(false);
    } else if (size==3) {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(false);
    } else if (size==4) {
        refPixel0_.setVisible(true);
        refPixel1_.setVisible(true);
        refPixel2_.setVisible(true);
        refPixel3_.setVisible(true);
    }
}

void OTBCloudDetectionImageFilterProcessor::process() {

    //check bypass switch

//    if (!enableSwitch_.get()) {
//        bypass(&inPort_, &outPort_);
//        return;
//    }

    try
    {
        //REFERENCE PIXELS PROPERTY

        VectorPixelType referencePixel;

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

        updatePixelSize();
        referencePixel.SetSize(size);
        referencePixel.Fill(0.);

        if (size==1) {
            referencePixel[0] = refPixel0_.get();
        } else if (size==2) {
            referencePixel[0] = refPixel0_.get();
            referencePixel[1] = refPixel1_.get();
        } else if (size==3) {
            referencePixel[0] = refPixel0_.get();
            referencePixel[1] = refPixel1_.get();
            referencePixel[2] = refPixel2_.get();
        } else if (size==4) {
            referencePixel[0] = refPixel0_.get();
            referencePixel[1] = refPixel1_.get();
            referencePixel[2] = refPixel2_.get();
            referencePixel[3] = refPixel3_.get();
        }

        //SET FILTER PARAMETERS

        filter->SetReferencePixel(referencePixel);
        filter->SetVariance(gaussianVarianceValue_.get());
        filter->SetMinThreshold(minThreshold_.get());
        filter->SetMaxThreshold(maxThreshold_.get());
        filter->SetInput(inPort_.getData());
        //filter->Update();
        outPort_.setData(filter->GetOutput());
        LINFO("Cloud Detection Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Cloud Detection Image Filter");
        return;
    }

}

void OTBCloudDetectionImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBImagePort *outport) {

    //TODO: bypass needs to convert from VectorImage to OTBImage
    //outport->setData(inport->getData());
}

}   // namespace



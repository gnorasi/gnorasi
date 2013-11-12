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

#include "otbshapedetectionlevelsetimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBShapeDetectionLevelSetImageFilterProcessor::loggerCat_("voreen.OTBShapeDetectionLevelSetImageFilterProcessor");

OTBShapeDetectionLevelSetImageFilterProcessor::OTBShapeDetectionLevelSetImageFilterProcessor()
    :OTBImageFilterProcessor(),
      seedX_("seedX", "Seed Point X: ", 0, 0, 2000),
      seedY_("seedY", "Seed Point Y: ", 0, 0, 2000),
      initialDistance_("initialDistance", "Initial Distance: ", 5.0f, 0.0f, 1000.0f),
      sigma_("sigma", "Sigma: ", 1.0f, 0.0f, 100.0f),
      sigmoidAlpha_("sigmoidAlpha", "Sigmoid Alpha:", 5.f, -100.0f, 255.0f),
      sigmoidBeta_("sigmoidBeta", "Sigmoid Beta:", 10.0f, -100.0f, 255.0f),
      curvatureScaling_("curvatureScaling_", "Curvature Scaling: ", 1.0, 0.0f, 100.0f),
      propagationScaling_("propagationScaling", "Propagation Scaling: ", 0.05, 0.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0),
      testOutPort1_(Port::OUTPORT, "OTBImage.Smoothing",0),
      testOutPort2_(Port::OUTPORT, "OTBImage.GradientMagnitude",0),
      testOutPort3_(Port::OUTPORT, "OTBImage.Sigmoid",0),
      testOutPort4_(Port::OUTPORT, "OTBImage.FastMarching",0)
{
    addProperty(enableSwitch_);
    addProperty(seedX_);
    addProperty(seedY_);
    addProperty(initialDistance_);
    addProperty(sigma_);
    addProperty(sigmoidAlpha_);
    addProperty(sigmoidBeta_);
    addProperty(propagationScaling_);
    addProperty(curvatureScaling_);

    addPort(inPort_);
    addPort(testOutPort1_);
    addPort(testOutPort2_);
    addPort(testOutPort3_);
    addPort(testOutPort4_);
    addPort(outPort_);

    thresholder = ThresholdingFilterType::New();
    smoothing = SmoothingFilterType::New();
    gradientMagnitude = GradientFilterType::New();
    sigmoid = SigmoidFilterType::New();
    fastMarching = FastMarchingFilterType::New();
    seeds = NodeContainer::New();
    shapeDetection = ShapeDetectionFilterType::New();

    portToFloatCaster = PortToFloatCastingFilterType::New();
    floatToPortCaster = FloatToPortCastingFilterType::New();
    floatToPortCaster1 = FloatToPortCastingFilterType::New();
    charToPortCaster = CharToPortCastingFilterType::New();
}

Processor* OTBShapeDetectionLevelSetImageFilterProcessor::create() const {
    return new OTBShapeDetectionLevelSetImageFilterProcessor();
}

OTBShapeDetectionLevelSetImageFilterProcessor::~OTBShapeDetectionLevelSetImageFilterProcessor() {

}

void OTBShapeDetectionLevelSetImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBShapeDetectionLevelSetImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBShapeDetectionLevelSetImageFilterProcessor::getProcessorInfo() const {
    return "Otsu Multiple Threshold Image Segmentation Processor";
}

void OTBShapeDetectionLevelSetImageFilterProcessor::process() {

    //LINFO("Shape Detection Level Set Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        //Curvature Anisotropic Diffusion
        smoothing->SetInput(inPort_.getData());
        smoothing->SetTimeStep(0.125);
        smoothing->SetNumberOfIterations(5);
        smoothing->SetConductanceParameter(9.0);
//        smoothing->Update();

        //testOutport1.Smoothing
        testOutPort1_.setData(smoothing->GetOutput());

        //Gradient Magnitude
        gradientMagnitude->SetInput(smoothing->GetOutput());
        double sigma = sigma_.get();
        gradientMagnitude->SetSigma(sigma);
//        gradientMagnitude->Update();

        //testOutport2.GradientMagnitude
        testOutPort2_.setData(gradientMagnitude->GetOutput());


        //Sigmoid
        portToFloatCaster->SetInput(gradientMagnitude->GetOutput());
        sigmoid->SetInput(portToFloatCaster->GetOutput());
        sigmoid->SetOutputMinimum(0.0);
        sigmoid->SetOutputMaximum(255.0);
        double alpha = sigmoidAlpha_.get();
        double beta = sigmoidBeta_.get();
        sigmoid->SetAlpha(alpha);
        sigmoid->SetBeta(beta);
//        sigmoid->Update();

        //testOutport3.Sigmoid
        floatToPortCaster->SetInput(sigmoid->GetOutput());
        testOutPort3_.setData(floatToPortCaster->GetOutput());


        //Fast Marching
        seedPosition[0] = seedX_.get();
        seedPosition[1] = seedY_.get();
        double initialDistance = initialDistance_.get();
        NodeType node;
        double seedValue = -initialDistance;
        node.SetValue(seedValue);
        node.SetIndex(seedPosition);
        seeds->Initialize();
        seeds->InsertElement(0,node);

        fastMarching->SetTrialPoints(seeds);
        fastMarching->SetSpeedConstant(1.0);
        fastMarching->SetOutputSize(inPort_.getData()->GetBufferedRegion().GetSize());
        fastMarching->Update();

        //testOutport4.FastMarching
        testOutPort4_.setData(fastMarching->GetOutput());

        //Shape Detection
        double propagationScaling = propagationScaling_.get();
        double curvatureScaling = curvatureScaling_.get();
        shapeDetection->SetPropagationScaling(propagationScaling);
        shapeDetection->SetCurvatureScaling(curvatureScaling);
        shapeDetection->SetMaximumRMSError(0.02);
        shapeDetection->SetNumberOfIterations(800);

        shapeDetection->SetInput(fastMarching->GetOutput());
        floatToPortCaster1->SetInput(sigmoid->GetOutput());
        shapeDetection->SetFeatureImage(floatToPortCaster1->GetOutput());

        //Thresholder
        thresholder->SetInput(shapeDetection->GetOutput());
        thresholder->SetLowerThreshold(-1000.0);
        thresholder->SetUpperThreshold(0.0);
        thresholder->SetOutsideValue(0);
        thresholder->SetInsideValue(255);
        thresholder->Update();

        //Outport.Filter
        charToPortCaster->SetInput(thresholder->GetOutput());
        outPort_.setData(charToPortCaster->GetOutput());
        LINFO("Shape Detection Level Set Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Shape Detection Level Set Image Filter");
        return;
    }

}

}

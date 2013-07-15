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

#include "otbgeodesicactivecontourlevelsetimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBGeodesicActiveContourLevelSetImageFilterProcessor::loggerCat_("voreen.OTBGeodesicActiveContourLevelSetImageFilterProcessor");

OTBGeodesicActiveContourLevelSetImageFilterProcessor::OTBGeodesicActiveContourLevelSetImageFilterProcessor()
    :OTBImageFilterProcessor(),
      seedX_("seedX", "Seed Point X: ", 0, 0, 2000),
      seedY_("seedY", "Seed Point Y: ", 0, 0, 2000),
      initialDistance_("initialDistance", "Initial Distance: ", 5.0f, 0.0f, 1000.0f),
      sigma_("sigma", "Sigma: ", 1.0f, 0.0f, 100.0f),
      sigmoidAlpha_("sigmoidAlpha", "Sigmoid Alpha:", 5.f, -100.0f, 255.0f),
      sigmoidBeta_("sigmoidBeta", "Sigmoid Beta:", 10.0f, -100.0f, 255.0f),
      propagationScaling_("propagationScaling", "Propagation Scaling: ", 10.0, 0.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.Inport",0),
      outPort_(Port::OUTPORT, "OTBImage.Outport",0),
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
    addPort(inPort_);
    addPort(testOutPort1_);
    addPort(testOutPort2_);
    addPort(testOutPort3_);
    addPort(testOutPort4_);
    addPort(outPort_);

    smoothing = SmoothingFilterType::New();
    gradientMagnitude = GradientFilterType::New();
    sigmoid = SigmoidFilterType::New();
    fastMarching = FastMarchingFilterType::New();
    seeds = NodeContainer::New();
    geodesicActiveContour = GeodesicActiveContourFilterType::New();
    thresholder = ThresholdingFilterType::New();

    portToCharCaster = PortToCharCastingFilterType::New();
    charToInternalCaster = CharToInternalCastingFilterType::New();
    charToPortCaster = CharToPortCastingFilterType::New();
    internToPortCaster1 = InternalToPortCastingFilterType::New();
}

Processor* OTBGeodesicActiveContourLevelSetImageFilterProcessor::create() const {
    return new OTBGeodesicActiveContourLevelSetImageFilterProcessor();
}

OTBGeodesicActiveContourLevelSetImageFilterProcessor::~OTBGeodesicActiveContourLevelSetImageFilterProcessor() {

}

void OTBGeodesicActiveContourLevelSetImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBGeodesicActiveContourLevelSetImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBGeodesicActiveContourLevelSetImageFilterProcessor::getProcessorInfo() const {
    return "Geodesic Active Contour Level Set Image Filter Processor";
}

void OTBGeodesicActiveContourLevelSetImageFilterProcessor::process() {

    //LINFO("Geodesic Active Contour Level SetImage Filter Enabled!");
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

        //testOutport1.Smoothing
        testOutPort1_.setData(smoothing->GetOutput());

        //Gradient Magnitude
        gradientMagnitude->SetInput(smoothing->GetOutput());
        double sigma = sigma_.get();
        gradientMagnitude->SetSigma(sigma);

        //testOutport2.GradientMagnitude
        testOutPort2_.setData(gradientMagnitude->GetOutput());

        //Sigmoid
        portToCharCaster->SetInput(gradientMagnitude->GetOutput());
        sigmoid->SetInput(portToCharCaster->GetOutput());
        sigmoid->SetOutputMinimum(0);
        sigmoid->SetOutputMaximum(255);
        double alpha = sigmoidAlpha_.get();
        double beta = sigmoidBeta_.get();
        sigmoid->SetAlpha(alpha);
        sigmoid->SetBeta(beta);
        sigmoid->Update();
        charToInternalCaster->SetInput(sigmoid->GetOutput());

        //testOutport3.Sigmoid
        charToPortCaster->SetInput(sigmoid->GetOutput());
        testOutPort3_.setData(charToPortCaster->GetOutput());

        //Geodesic Active Contour
        geodesicActiveContour->SetFeatureImage(charToInternalCaster->GetOutput());
        double propagationScaling = propagationScaling_.get();
        geodesicActiveContour->SetPropagationScaling(propagationScaling);
        geodesicActiveContour->SetCurvatureScaling(1.0);
        geodesicActiveContour->SetAdvectionScaling(1.0);
        geodesicActiveContour->SetMaximumRMSError(0.02);
        geodesicActiveContour->SetNumberOfIterations(800);

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

        //testOutport3.FastMarching
        testOutPort4_.setData(fastMarching->GetOutput());

        //Fast Matching -> Geodesic Active Contour
        geodesicActiveContour->SetInput(fastMarching->GetOutput());
        geodesicActiveContour->Update();

        //Thresholder
        internToPortCaster1->SetInput(geodesicActiveContour->GetOutput());
        thresholder->SetInput(internToPortCaster1->GetOutput());
        thresholder->SetLowerThreshold(-1000.0);
        thresholder->SetUpperThreshold(0.0);
        thresholder->SetOutsideValue(0);
        thresholder->SetInsideValue(255);
        thresholder->Update();

        //outPort
        outPort_.setData(thresholder->GetOutput());

        LINFO("Geodesic Active Contour Level SetImage Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Geodesic Active Contour Level SetImage Filter!");
        return;
    }
}

}   // namespace


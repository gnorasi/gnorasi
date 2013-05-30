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

#include "otbperbandvectorimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBPerBandVectorImageFilterProcessor::loggerCat_("voreen.OTBPerBandVectorImageFilterProcessor");

OTBPerBandVectorImageFilterProcessor::OTBPerBandVectorImageFilterProcessor()
    :OTBImageFilterProcessor(),
      selectFilter_("selectFilter", "Select filter to apply:"),
      inPort_(Port::INPORT, "IN MultiBand Image", 0),
      outPort_(Port::OUTPORT, "OUT MultiBand Image", 0),

      // CANNY EDGE DETECTION IMAGE FILTER PROPERTIES

      variance_("variance", "Variance", 2.0f, 0.0f, 10000.0f),
      upper_("upper", "Upper Threshold", 10.0f, 0.0f, 10000.0f),
      lower_("lower", "Lower Threshold", 0.0f, 0.0f, 10000.0f),

      // GRADIENT ANISOTROPIC DIFFUSION IMAGE FILTER PROPERTIES

      numberOfIterations_("numberOfIterations", "Iterations", 5),
      timeStep_("timeStep", "Time Step", 0.125f, 0.0f, 10.0f),
      conductance_("conductance", "Conductance", 3.0f, 0.0f, 100.0f),

      // MEAN IMAGE FILTER PROPERTIES

      filterSize_("filterSize", "Filter Radius", 1)

{
      selectFilter_.addOption("canny", "Canny Edge Detection Image Filter");
      selectFilter_.addOption("gradient", "Gradient Anisotropic Diffusion Image Filter");
      selectFilter_.addOption("mean", "Mean Image Filter");

      selectFilter_.onChange(CallMemberAction<OTBPerBandVectorImageFilterProcessor>
                             (this,&OTBPerBandVectorImageFilterProcessor::updateFilterSelection));

      addProperty(enableSwitch_);
      addProperty(selectFilter_);
      addPort(inPort_);
      addPort(outPort_);

      // GRADIENT ANISOTROPIC DIFFUSION IMAGE FILTER PROPERTIES CONFIG

      addProperty(variance_);
      addProperty(upper_);
      addProperty(lower_);
      variance_.setVisible(false);
      upper_.setVisible(false);
      lower_.setVisible(false);

      gradient = GradientAnisotropicDiffusionFilterType::New();
      gradientAnisotropicPerBand = GradientAnisotropicPerBandFilterType::New();

      // CANNY EDGE DETECTION IMAGE FILTER PROPERTIES CONFIG

      addProperty(numberOfIterations_);
      addProperty(timeStep_);
      addProperty(conductance_);
      numberOfIterations_.setVisible(false);
      timeStep_.setVisible(false);
      conductance_.setVisible(false);

      canny = CannyEdgeDetectionFilterType::New();
      cannyPerBand = CannyPerBandFilterType::New();

      // MEAN IMAGE FILTER PROPERTIES CONFIG

      addProperty(filterSize_);
      filterSize_.setVisible(false);

      mean = MeanFilterType::New();
      meanPerBand = MeanPerBandFilterType::New();
}

Processor* OTBPerBandVectorImageFilterProcessor::create() const {
    return new OTBPerBandVectorImageFilterProcessor();
}

OTBPerBandVectorImageFilterProcessor::~OTBPerBandVectorImageFilterProcessor() {

}

void OTBPerBandVectorImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    updateFilterSelection();    //to force update first option properties
}

void OTBPerBandVectorImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBPerBandVectorImageFilterProcessor::getProcessorInfo() const {
    return "Per Band Vector Image Filter";
}

void OTBPerBandVectorImageFilterProcessor::updateFilterSelection() {

    if (selectFilter_.get()=="canny") {
        variance_.setVisible(true);
        upper_.setVisible(true);
        lower_.setVisible(true);
        numberOfIterations_.setVisible(false);
        timeStep_.setVisible(false);
        conductance_.setVisible(false);
        filterSize_.setVisible(false);
    } else if (selectFilter_.get()=="gradient") {
        variance_.setVisible(false);
        upper_.setVisible(false);
        lower_.setVisible(false);
        numberOfIterations_.setVisible(true);
        timeStep_.setVisible(true);
        conductance_.setVisible(true);
        filterSize_.setVisible(false);
    } else if(selectFilter_.get()=="mean") {
        variance_.setVisible(false);
        upper_.setVisible(false);
        lower_.setVisible(false);
        numberOfIterations_.setVisible(false);
        timeStep_.setVisible(false);
        conductance_.setVisible(false);
        filterSize_.setVisible(true);
    }

}

void OTBPerBandVectorImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    try
    {
        if (selectFilter_.get()=="canny") {
            canny->SetVariance( variance_.get() );
            canny->SetUpperThreshold( upper_.get() );
            canny->SetLowerThreshold( lower_.get() );

            cannyPerBand->SetFilter(canny);
            cannyPerBand->SetInput(inPort_.getData());
            cannyPerBand->Update();
            outPort_.setData(cannyPerBand->GetOutput());
            LINFO("Canny Edge Detection Per Band Image Filter connected successfully!");

        } else if (selectFilter_.get()=="gradient") {
            gradient->SetNumberOfIterations(numberOfIterations_.get());
            gradient->SetTimeStep(timeStep_.get());
            gradient->SetConductanceParameter(conductance_.get());

            gradientAnisotropicPerBand->SetFilter(gradient);
            gradientAnisotropicPerBand->SetInput(inPort_.getData());
            gradientAnisotropicPerBand->Update();
            outPort_.setData(gradientAnisotropicPerBand->GetOutput());
            LINFO("Gradient Anisotropic Diffusion Per Band Image Filter connected successfully!");

        } else if(selectFilter_.get()=="mean") {
            //Property validation
            int rad = filterSize_.get();
            ImageType::SizeType indexRadius;
            indexRadius[0] = rad; // radius along x
            indexRadius[1] = rad; // radius along y

            mean->SetRadius(indexRadius);

            meanPerBand->SetFilter(mean);
            meanPerBand->SetInput(inPort_.getData());
            meanPerBand->Update();
            outPort_.setData(meanPerBand->GetOutput());
            LINFO("Mean Per Band Image Filter connected successfully!");
        }

    }
    catch (int e)
    {
        LERROR("Error in Per Band Vector Image Filter");
        return;
    }

}

void OTBPerBandVectorImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace


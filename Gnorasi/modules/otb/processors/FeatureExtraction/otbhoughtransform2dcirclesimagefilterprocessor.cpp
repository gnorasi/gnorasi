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

#include "otbhoughtransform2dcirclesimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBHoughTransform2DCirclesImageFilterProcessor::loggerCat_("voreen.OTBHoughTransform2DCirclesImageFilterProcessor");

OTBHoughTransform2DCirclesImageFilterProcessor::OTBHoughTransform2DCirclesImageFilterProcessor()
    :OTBImageFilterProcessor(),
      numberOfCircles_("numberOfCircles", "Number of Circles:", 1, 1, 100),
      minRadius_("minRadius", "Radius Min:", 0.0f, 0.0f, 1000.0f),
      maxRadius_("maxRadius", "Radius Max:", 10.0f, 0.0f, 1000.0f),
      sweepAngle_("sweepAngle", "Sweep Angle:", 0.0f, 0.0f, 360.0f),
      sigmaGradient_("sigmaGradient", "Sigma Gradient:", 1, 1, 100),
      variance_("variance", "Variance:", 5.0f, 1.0f, 100.0f),
      radiusToRemove_("radiusToRemove", "Radius to remove:", 10.0f, 1.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    addProperty(numberOfCircles_);
    addProperty(minRadius_);
    addProperty(maxRadius_);
    addProperty(sweepAngle_);
    addProperty(sigmaGradient_);
    addProperty(variance_);
    addProperty(radiusToRemove_);
    addPort(inPort_);
    addPort(outPort_);

    houghFilter = HoughTransformFilterType::New();
    localOutputImage = OutputImageType::New();
    localInputImage = CharImageType::New();
    portToCharCaster = PortToCharCastingFilterType::New();
}

Processor* OTBHoughTransform2DCirclesImageFilterProcessor::create() const {
    return new OTBHoughTransform2DCirclesImageFilterProcessor();
}

OTBHoughTransform2DCirclesImageFilterProcessor::~OTBHoughTransform2DCirclesImageFilterProcessor() {

}

void OTBHoughTransform2DCirclesImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBHoughTransform2DCirclesImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBHoughTransform2DCirclesImageFilterProcessor::getProcessorInfo() const {
    return "Hough Transform 2D Circles Image Filter Processor";
}

void OTBHoughTransform2DCirclesImageFilterProcessor::process() {

    //LINFO("Hough Transform 2D Circles Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        ///< Cast inPort to Input

        portToCharCaster->SetInput(inPort_.getData());
        localInputImage=portToCharCaster->GetOutput();

        ///< Parameters (arguments 3 to 5)

        houghFilter->SetNumberOfCircles(numberOfCircles_.get());
        houghFilter->SetMinimumRadius(minRadius_.get());
        houghFilter->SetMaximumRadius(maxRadius_.get());

        ///< Optional Parameters (arguments 6 to 9)

        houghFilter->SetThreshold(0);
        houghFilter->SetSweepAngle(sweepAngle_.get());
        houghFilter->SetSigmaGradient(sigmaGradient_.get());
        houghFilter->SetVariance(variance_.get());
        houghFilter->SetDiscRadiusRatio(radiusToRemove_.get());

        houghFilter->SetInput(inPort_.getData());
//        houghFilter->SetInput(localInputImage);

        houghFilter->Update();

//        localAccumulator = houghFilter->GetOutput();

        ///< Allocate an image to draw the resulting circles as binary objects

        circles = houghFilter->GetCircles(numberOfCircles_.get());

        region.SetSize(localInputImage->GetLargestPossibleRegion().GetSize());
        region.SetIndex(localInputImage->GetLargestPossibleRegion().GetIndex());
        localOutputImage->SetRegions( region );
        localOutputImage->SetOrigin(localInputImage->GetOrigin());
        localOutputImage->SetSpacing(localInputImage->GetSpacing());
        localOutputImage->Allocate();
        localOutputImage->FillBuffer(0);

        ///< Iterate through the list of circles and draw

        itCircles = circles.begin();

        while (itCircles != circles.end() )
        {

            ///< Draw white pixels in the output image to represent each circle

            for (double angle = 0; angle <= 2*vnl_math::pi; angle += vnl_math::pi/60.0 )
            {
                localIndex[0] =
                        (long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[0]
                                   +(*itCircles)->GetRadius()[0]*vcl_cos(angle));
                localIndex[1] =
                        (long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[1]
                                   +(*itCircles)->GetRadius()[0]*vcl_cos(angle));
                outputRegion = localOutputImage->GetLargestPossibleRegion();

                if (outputRegion.IsInside(localIndex))
                {
                    localOutputImage->SetPixel(localIndex, 255);
                }
            }
            itCircles++;
        }

        ///< Cast Output to outPort

        charToPortCaster->SetInput(localOutputImage);
        outPort_.setData(charToPortCaster->GetOutput());

        LINFO("Hough Transform 2D Circles Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Hough Transform 2D Circles Image Filter!");
        return;
    }

}

}   // namespace



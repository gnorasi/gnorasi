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

#include "otbhoughtransform2dlinesimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBHoughTransform2DLinesImageFilterProcessor::loggerCat_("voreen.OTBHoughTransform2DLinesImageFilterProcessor");

OTBHoughTransform2DLinesImageFilterProcessor::OTBHoughTransform2DLinesImageFilterProcessor()
    :OTBImageFilterProcessor(),
      numberOfLines_("numberOfLines", "Number of Lines:", 4, 1, 100),
      discRadius_("maxRadius", "Disc Radius to remove:", 10.0f, 1.0f, 100.0f),
      variance_("variance", "Variance:", 5.0f, 1.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    addProperty(numberOfLines_);
    addProperty(discRadius_);
    addProperty(variance_);
    addPort(inPort_);
    addPort(outPort_);

    houghFilter = HoughTransformFilterType::New();
    caster = CastingFilterType::New();
    portToCharCaster = PortToCharCastingFilterType::New();
    charToPortCaster = CharToPortCastingFilterType::New();
    gradFilter = GradientFilterType::New();
    threshFilter = ThresholdFilterType::New();
    accToDoubleCaster = AccToDoubleCastingFilterType::New();

    localOutputImage = OutputImageType::New();
    localInputImage = CharImageType::New();
}

Processor* OTBHoughTransform2DLinesImageFilterProcessor::create() const {
    return new OTBHoughTransform2DLinesImageFilterProcessor();
}

OTBHoughTransform2DLinesImageFilterProcessor::~OTBHoughTransform2DLinesImageFilterProcessor() {

}

void OTBHoughTransform2DLinesImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBHoughTransform2DLinesImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBHoughTransform2DLinesImageFilterProcessor::getProcessorInfo() const {
    return "Hough Transform 2D Lines Image Filter Processor";
}

void OTBHoughTransform2DLinesImageFilterProcessor::process() {

    //LINFO("Hough Transform 2D Lines Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        portToCharCaster->SetInput(inPort_.getData());
        localInputImage = portToCharCaster->GetOutput();

        caster->SetInput(localInputImage);
        gradFilter->SetInput(caster->GetOutput());
        gradFilter->Update();

        threshFilter->SetInput(gradFilter->GetOutput());
        threshFilter->SetOutsideValue(0);

        unsigned char threshBelow = 0;
        unsigned char threshAbove = 255;

        threshFilter->ThresholdOutside(threshBelow,threshAbove);
        threshFilter->Update();

        AccumulatorImageType::Pointer test = threshFilter->GetOutput();
        accToDoubleCaster->SetInput(test);
        houghFilter->SetInput(accToDoubleCaster->GetOutput());

        ///< parameters go here.

        houghFilter->SetNumberOfLines(numberOfLines_.get());
        houghFilter->SetVariance(variance_.get());
        houghFilter->SetDiscRadius(discRadius_.get());

        houghFilter->Update();

//        localAccumulator=houghFilter->GetOutput();

        lines = houghFilter->GetLines(numberOfLines_.get());

        region.SetSize(localInputImage->GetLargestPossibleRegion().GetSize());
        region.SetIndex(localInputImage->GetLargestPossibleRegion().GetIndex());
        localOutputImage->SetRegions( region );
        localOutputImage->SetOrigin(localInputImage->GetOrigin());
        localOutputImage->SetSpacing(localInputImage->GetSpacing());
        localOutputImage->Allocate();
        localOutputImage->FillBuffer(0);

        linesIterator itLines = lines.begin();

        while (itLines != lines.end() )
        {

            PointListType pointsList = (*itLines)->GetPoints();
            PointListType::const_iterator itPoints = pointsList.begin();

            double u[2];
            u[0] = (*itPoints).GetPosition()[0];
            u[1] = (*itPoints).GetPosition()[1];
            itPoints++;

            double v[2];
            v[0] = u[0]-(*itPoints).GetPosition()[0];
            v[1] = u[1]-(*itPoints).GetPosition()[1];

            double norm = vcl_sqrt(v[0]*v[0]+v[1]*v[1]);
            v[0] /= norm;
            v[1] /= norm;

            itk::Size<2> size = localOutputImage->GetLargestPossibleRegion().GetSize();
            float diag = vcl_sqrt((float)(size[0]*size[0] + size[1]*size[1]));

            for(int i=static_cast<int>(-diag); i<static_cast<int>(diag); i++)
            {
                localIndex[0] = (long int)(u[0]+i*v[0]);
                localIndex[1] = (long int)(u[1]+i*v[1]);

                outputRegion=localOutputImage->GetLargestPossibleRegion();

                if(outputRegion.IsInside(localIndex))
                {
                    localOutputImage->SetPixel(localIndex, 255);
                }
            }
            itLines++;
        }

        charToPortCaster->SetInput(localOutputImage);
        outPort_.setData(charToPortCaster->GetOutput());
        LINFO("Hough Transform 2D Lines Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Hough Transform 2D Lines Image Filter!");
        return;
    }

}

}   // namespace


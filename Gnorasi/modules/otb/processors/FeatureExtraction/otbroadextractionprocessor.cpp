/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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

#include "otbroadextractionprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBRoadExtractionProcessor::loggerCat_("voreen.OTBRoadExtractionProcessor");

OTBRoadExtractionProcessor::OTBRoadExtractionProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    m_alphaProperty("alpha", "Alpha Property",0.0),
    m_amplitudeProperty("amplitude", "Amplitude",0.0),
    m_angularThresholdProperty("angularThreshold", "Angular Threshold Property",0.0),
    m_distanceThresholdProperty("distanceThreshold", "Distance Threshold Property",0.0),
    m_fistMeanDistanceThresholdProperty("firstMeanDistanceThreshold", "First Mean Threshold Property",0.0),
    m_referencePixelProperty("referencePixel", "Reference Pixel Property"),
    m_resolutionProperty("resolution", "Resolution Property"),
    m_sendondMeanDistanceThresholdProperty("second", "Second Mean Distance Threshold Property",0.0),
    m_toleranceProperty("tolerance", "Tolerance Property",0.0),
    m_maxAngleProperty("maxAngle", "Max Angle Property", 0.0)
{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(m_alphaProperty);
    addProperty(m_amplitudeProperty);
    addProperty(m_angularThresholdProperty);
    addProperty(m_distanceThresholdProperty);
    addProperty(m_fistMeanDistanceThresholdProperty);
    addProperty(m_referencePixelProperty);
    addProperty(m_resolutionProperty);
    addProperty(m_sendondMeanDistanceThresholdProperty);
    addProperty(m_toleranceProperty);
    addProperty(m_maxAngleProperty);

    roadextractor = RoadExtractionFilterType::New();
    drawingFilter = DrawPathFilterType::New();

    drawingFilter->UseInternalPathValueOn();

}

void OTBRoadExtractionProcessor::update(){
    process();
}

Processor* OTBRoadExtractionProcessor::create() const {
  return new OTBRoadExtractionProcessor();
}

OTBRoadExtractionProcessor::~OTBRoadExtractionProcessor() {

}

void OTBRoadExtractionProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBRoadExtractionProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBRoadExtractionProcessor::getProcessorInfo() const {
    
    return "Performs the extraction of roads from an image";
}

void OTBRoadExtractionProcessor::process() {
  
    try
    {

        VectorImageType *img = inPort_.getData();
        if(!img)
            return;

        roadextractor->SetInput(img);

        std::string text = m_referencePixelProperty.get();
        std::vector<std::string> strings;
        std::istringstream f(text);
        std::string s;
        while (std::getline(f, s, ';')) {
            std::cout << s << std::endl;
            strings.push_back(s);
        }

        VectorImageType::PixelType ReferencePixel;
        ReferencePixel.SetSize(4);
        for(int i  = 0; i < strings.size(); i++){
            std::string cs = strings.at(i);
            const char *c = cs.c_str();
            ReferencePixel.SetElement(i, ::atof(c));
        }
        roadextractor->SetReferencePixel(ReferencePixel);
        roadextractor->SetAlpha(m_alphaProperty.get());
        roadextractor->SetAmplitudeThreshold(m_amplitudeProperty.get());
        roadextractor->SetTolerance(m_toleranceProperty.get());
        roadextractor->SetMaxAngle(m_maxAngleProperty.get());
        roadextractor->SetAngularThreshold(m_angularThresholdProperty.get());
        roadextractor->SetFirstMeanDistanceThreshold(m_fistMeanDistanceThresholdProperty.get());
        roadextractor->SetSecondMeanDistanceThreshold(m_sendondMeanDistanceThresholdProperty.get());
        roadextractor->SetDistanceThreshold(m_distanceThresholdProperty.get());

        ImageType::Pointer blackBackground = ImageType::New();
        blackBackground->SetRegions(img->GetLargestPossibleRegion());
        blackBackground->Allocate();
        blackBackground->FillBuffer(0);

        drawingFilter->SetInput(blackBackground);
        drawingFilter->SetInputPath(roadextractor->GetOutput());

        drawingFilter->Update();

        outPort_.setData(drawingFilter->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with road extraction!");
	return;
    }
    
}

} // namespace

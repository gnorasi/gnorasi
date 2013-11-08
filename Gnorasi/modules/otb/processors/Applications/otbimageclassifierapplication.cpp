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

#include "otbimageclassifierapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBImageClassifierApplication::loggerCat_("voreen.OTBImageClassifierApplication");

OTBImageClassifierApplication::OTBImageClassifierApplication()
    :OTBImageFilterProcessor(),
      enableMaskImage_("enableMaskImage_", "Enable Mask Image", false),
      enableXMLReader_("enableXMLReader_", "Enable XML Normalization", false),
      ModelFilename_("ModelFilename_", "Input Model File:", "SVM File", VoreenApplication::app()->getUserDataPath(), "SVM file (*.svm)"),
      XMLFilename_("XMLFilename_", "Input XML File:", "XML File", VoreenApplication::app()->getUserDataPath(), "XML file (*.xml)"),
      inPort_(Port::INPORT, "IN Vector Image", 0),
      maskInPort_(Port::INPORT, "IN Mask Image", 0),
      outPort_(Port::OUTPORT, "OUT Vector Image", 0)
{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(maskInPort_);
    addPort(outPort_);

    addProperty(enableMaskImage_);
    addProperty(enableXMLReader_);
    addProperty(ModelFilename_);
    addProperty(XMLFilename_);

    XMLFilename_.setVisible(false);

    enableXMLReader_.onChange(CallMemberAction<OTBImageClassifierApplication>(this, &OTBImageClassifierApplication::updateReader));

    m_ClassificationFilter = ClassificationFilterType::New();
    statisticsReader = StatisticsReader::New();
    m_Rescaler = RescalerType::New();
}

Processor* OTBImageClassifierApplication::create() const {
    return new OTBImageClassifierApplication();
}

OTBImageClassifierApplication::~OTBImageClassifierApplication() {

}

void OTBImageClassifierApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBImageClassifierApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBImageClassifierApplication::getProcessorInfo() const {
    return "Image Classifier Application";
}

void OTBImageClassifierApplication::updateReader() {

    if (enableXMLReader_.get()) {
        XMLFilename_.setVisible(true);
    } else {
        XMLFilename_.setVisible(false);
    }
}

void OTBImageClassifierApplication::process() {

    try
    {
        MeasurementType  meanMeasurementVector;
        MeasurementType  stddevMeasurementVector;

        m_Model = MachineLearningModelFactoryType::CreateMachineLearningModel(ModelFilename_.get(),
                                                                                  MachineLearningModelFactoryType::ReadMode);
        LINFO("Model loaded.");

        if (enableXMLReader_.get()) {
            LINFO("Input image normalization activated.");
            statisticsReader->SetFileName(XMLFilename_.get());
            meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
            stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
            LINFO("mean used: " << meanMeasurementVector);
            LINFO("standard deviation used: " << stddevMeasurementVector);
            m_Rescaler->SetScale(stddevMeasurementVector);
            m_Rescaler->SetShift(meanMeasurementVector);
            m_Rescaler->SetInput(inPort_.getData());
            m_ClassificationFilter->SetInput(m_Rescaler->GetOutput());
        } else {
            LINFO("Input image normalization deactivated.");
            m_ClassificationFilter->SetInput(inPort_.getData());
        }

        if (enableMaskImage_.get()) {
            LINFO("Using input mask");
            m_ClassificationFilter->SetInputMask(maskInPort_.getData());
        }

        outPort_.setData(m_ClassificationFilter->GetOutput());
        LINFO("Image Classifier Application Connected");
    }
    catch (int e)
    {
        LERROR("Error in Image Classifier Application");
        return;
    }

}

}   // namespace

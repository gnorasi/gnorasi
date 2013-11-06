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

#include "otbcomputeimagesstatisticsapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBComputeImagesStatisticsApplication::loggerCat_("voreen.OTBComputeImagesStatisticsApplication");

OTBComputeImagesStatisticsApplication::OTBComputeImagesStatisticsApplication()
    :OTBImageFilterProcessor(),
      enableXMLWriter_("enableXMLWriter_", "Enable XML Output", false),
      computeButton_("computeButton", "Recalculate"),
      XMLFilename_("XMLFilename_", "Output XML File:", "XML File", VoreenApplication::app()->getUserDataPath(), "XML file (*.xml)", FileDialogProperty::SAVE_FILE),
      saveXMLButton_("saveXMLButton_", "Write Output to XML"),
      inPort_(Port::INPORT, "IN Vector Image", 0)

{

    addProperty(enableSwitch_);
    addPort(inPort_);

    addProperty(computeButton_);
    addProperty(enableXMLWriter_);
    addProperty(XMLFilename_);
    addProperty(saveXMLButton_);

    XMLFilename_.setVisible(false);
    saveXMLButton_.setVisible(false);

    enableXMLWriter_.onChange(CallMemberAction<OTBComputeImagesStatisticsApplication>(this, &OTBComputeImagesStatisticsApplication::updateWriter));
    computeButton_.onChange(CallMemberAction<OTBComputeImagesStatisticsApplication>(this, &OTBComputeImagesStatisticsApplication::process));
    saveXMLButton_.onChange(CallMemberAction<OTBComputeImagesStatisticsApplication>(this, &OTBComputeImagesStatisticsApplication::saveToXMLFile));

    statsEstimator = StreamingStatisticsVImageFilterType::New();
    writer = StatisticsWriter::New();

    hasFile=false;

}

Processor* OTBComputeImagesStatisticsApplication::create() const {
    return new OTBComputeImagesStatisticsApplication();
}

OTBComputeImagesStatisticsApplication::~OTBComputeImagesStatisticsApplication() {

}

void OTBComputeImagesStatisticsApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBComputeImagesStatisticsApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBComputeImagesStatisticsApplication::getProcessorInfo() const {
    return "Compute Image Statistics Application";
}

void OTBComputeImagesStatisticsApplication::updateWriter() {

    if (enableXMLWriter_.get()) {
        XMLFilename_.setVisible(true);
        saveXMLButton_.setVisible(true);
    } else {
        XMLFilename_.setVisible(false);
        saveXMLButton_.setVisible(false);    }
}

void OTBComputeImagesStatisticsApplication::process() {

    try
    {
        unsigned int nbSamples = 0;
        unsigned int nbBands = 0;

        nbBands = inPort_.getData()->GetNumberOfComponentsPerPixel();
        VectorImageType::SizeType size = inPort_.getData()->GetLargestPossibleRegion().GetSize();
        mean.SetSize(nbBands);
        mean.Fill(0.);
        variance.SetSize(nbBands);
        variance.Fill(0.);

        statsEstimator->SetInput(inPort_.getData());
        statsEstimator->Update();
        mean += statsEstimator->GetMean();

        for (unsigned int itBand = 0; itBand < nbBands; itBand++)
        {
            variance[itBand] += (size[0] * size[1] - 1) * (statsEstimator->GetCovariance())(itBand, itBand);
        }

        nbSamples += size[0] * size[1] * nbBands;
        variance /= (nbSamples - 1);

        stddev.SetSize(nbBands);
        stddev.Fill(0.);

        for (unsigned int i = 0; i < variance.GetSize(); ++i)
        {
            stddev[i] = vcl_sqrt(variance[i]);
        }

        LINFO("Compute Image Statistics Application Connected");

        LINFO("Computation Output");
        LINFO("------------------");
        LINFO("Mean: "<<mean);
        LINFO("Standard Deviation: "<<stddev);

    }
    catch (int e)
    {
        LERROR("Error in Compute Image Statistics Application");
        return;
    }

}

void OTBComputeImagesStatisticsApplication::saveToXMLFile() {

    std::string filename = XMLFilename_.get();
    ///< check if path was given
    if (!filename.empty())
    {
        hasFile = true;
    }

    if(hasFile && enableSwitch_.get())
    {
        // Write the Statistics via the statistic writer
        writer->SetFileName(XMLFilename_.get());
        writer->AddInput("mean", mean);
        writer->AddInput("stddev", stddev);
        writer->Update();
        LINFO(XMLFilename_.get()<<" written successfully!");

    }else if(!hasFile){
        LWARNING("XML File Path Not Set");
        return;
    }else if (!enableSwitch_.get()) {
        LWARNING("Processor is disabled");
        return;
    }
}

}   // namespace

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

#include "otbgreylevelcooccurrencematrixadvancedtexturecoefficientscalculatorprocessor.h"
#include "voreen/core/voreenapplication.h"
#include <iostream>
#include <fstream>

namespace voreen {

const std::string OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::loggerCat_("voreen.OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor");

OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      computeButton_("computeButton", "Force Calculation"),
      textFile_("textFile", "Output Text File:", "Text File", VoreenApplication::app()->getUserDataPath(), "Text file (*.txt)", FileDialogProperty::SAVE_FILE),
      saveTextButton_("saveButton", "Write Output to File")
{
    addProperty(enableSwitch_);
    addProperty(computeButton_);
    addProperty(textFile_);
    addProperty(saveTextButton_);
    addPort(inPort_);

    computeButton_.onChange(CallMemberAction<OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor>(this, &OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::process));
    saveTextButton_.onChange(CallMemberAction<OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor>(this, &OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::saveToFile));

    hasFile=false;

    generator = GeneratorType::New();
    filter = FilterType::New();
}

Processor* OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::create() const {
    return new OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor();
}

OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::~OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor() {

}

void OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::getProcessorInfo() const {
    return "Grey Level Cooccurrence Matrix Advanced Texture Coefficients Calculator Processor";
}

void OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::process() {

    try
    {
          ///< Set size parameter for otbImageToHistogramGenerator
          ///< *NOT* needed is using otbScalarToHistogramGenerator

//        SizeType size;
//        size[0] = 255;  // num of bins for the Red channel
//        size[1] = 255;  // num of bins for the Green channel
//        size[2] = 255;  // num of bins for the Blue channel

        generator->SetNumberOfBins(128);
        generator->SetMarginalScale(10.0);
        generator->SetInput(inPort_.getData());
        generator->Compute();

        ///< Initializing output variables
        variance = 0.0;
        mean = 0.0;
        sumAverage = 0.0;
        sumVariance = 0.0;
        sumEntropy = 0.0;
        differenceEntropy = 0.0;
        differenceVariance = 0.0;
        ic1 = 0.0;
        ic2 = 0.0;

        if (!enableSwitch_.get()) {
            LWARNING("Processor is disabled");
            return;
        }
        else
        {
            ///< Set Calculator input histogram & Compute outputs

            // TODO: Fix the convertion error between the two filters

            //filter->SetHistogram(generator->GetOutput());
            //filter->Compute();

            variance = filter->GetVariance();
            mean = filter->GetMean();
            sumAverage = filter->GetSumAverage();
            sumVariance = filter->GetSumVariance();
            sumEntropy = filter->GetSumEntropy();
            differenceEntropy = filter->GetDifferenceEntropy();
            differenceVariance = filter->GetDifferenceVariance();
            ic1 = filter->GetIC1();
            ic2 = filter->GetIC2();
            LINFO("Grey Level Cooccurrence Matrix Advanced Texture Coefficients Calculator Connected!");
        }
    }
    catch (int e)
    {
        LERROR("Error in Grey Level Cooccurrence Matrix Advanced Texture Coefficients Calculator");
        return;
    }
}

void OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor::saveToFile() {

    std::string filename = textFile_.get();
    ///< check if path was given
    if (!filename.empty())
    {
        hasFile = true;
    }

    if(hasFile && enableSwitch_.get())
    {
        ///< open the specified text file, write output and close
        std::ofstream myfile (filename.c_str());
        if (myfile.is_open())
        {
            myfile << "Grey Level Cooccurrence Matrix Advanced Texture Coefficients Calculator\n";
            myfile << "-----------------------------------------------------------------------\n";
            myfile << "OUTPUT\n";
            myfile << "======\n";
            myfile << "Variance: " << variance << "\n";
            myfile << "Mean: " << mean << "\n";
            myfile << "Sum Average: " << sumAverage << "\n";
            myfile << "Sum Variance: " << sumVariance << "\n";
            myfile << "Sum Entropy: " << sumEntropy << "\n";
            myfile << "Difference Entropy: " << differenceEntropy << "\n";
            myfile << "Difference Variance: " << differenceVariance << "\n";
            myfile << "IC1: " << ic1 << "\n";
            myfile << "IC2: " << ic2 << "\n";
            myfile.close();
            LINFO("File output written succesfully!");
        } else {
            LWARNING("Unable to open file");
        }
    }else if(!hasFile){
        LWARNING("Text File Path Not Set");
        return;
    }else if (!enableSwitch_.get()) {
        LWARNING("Processor is disabled");
        return;
    }

}

}   // namespace

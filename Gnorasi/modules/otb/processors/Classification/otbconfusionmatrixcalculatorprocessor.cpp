/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*                                                                              *
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

#include "otbconfusionmatrixcalculatorprocessor.h"
#include "voreen/core/voreenapplication.h"
#include <iostream>
#include <fstream>

namespace voreen {
const std::string OTBConfusionMatrixCalculatorProcessor::loggerCat_("voreen.OTBConfusionMatrixCalculatorProcessor");

OTBConfusionMatrixCalculatorProcessor::OTBConfusionMatrixCalculatorProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Referenced Vector Image Input", 0),
    inPort1_(Port::OUTPORT, "Produced Vector Image Output", 0),
    inPort2_(Port::INPORT, "Referenced Vector Data Input", 0),
    inPort3_(Port::OUTPORT, "Produced Vector Data Input",0),
    textFile_("textFile", "Output Text File:", "Text File", VoreenApplication::app()->getUserDataPath(), "Text file (*.txt)", FileDialogProperty::SAVE_FILE),
    saveTextButton_("saveButton", "Write Output to File"),
    executeButton_("executeButton", "Force Calculate")

{
    addPort(inPort_);
    addPort(inPort1_);
    addPort(inPort2_);
    addPort(inPort3_);
    addProperty(executeButton_);
    addProperty(textFile_);
    addProperty(saveTextButton_);

    confusioncalculator = ConfusionFilterType::New();
    hasFile=false;
    saveTextButton_.onChange(CallMemberAction<OTBConfusionMatrixCalculatorProcessor>(this, &OTBConfusionMatrixCalculatorProcessor::saveToFile));
    executeButton_.onChange(CallMemberAction<OTBConfusionMatrixCalculatorProcessor>(this, &OTBConfusionMatrixCalculatorProcessor::process));
}

void OTBConfusionMatrixCalculatorProcessor::update(){
    process();
}

Processor* OTBConfusionMatrixCalculatorProcessor::create() const {
  return new OTBConfusionMatrixCalculatorProcessor();
}

OTBConfusionMatrixCalculatorProcessor::~OTBConfusionMatrixCalculatorProcessor() {

}

void OTBConfusionMatrixCalculatorProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBConfusionMatrixCalculatorProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBConfusionMatrixCalculatorProcessor::getProcessorInfo() const {
    
    return "Computes a confusion matrix from 2 lists of labels";
}

void OTBConfusionMatrixCalculatorProcessor::process() {
  
    try
    {

        VImageType *imgref = inPort_.getData();
        VImageType *imgpro = inPort1_.getData();
        VectorDataType *vdataref = inPort2_.getData();
        VectorDataType *vdatapro = inPort3_.getData();
        if(!imgref || !imgpro || !vdataref || !vdatapro)
            return;

        // Project the vectorData in the Image Coodinate system
        VectorDataProjectionFilterType::Pointer vproj_ref = VectorDataProjectionFilterType::New();
        vproj_ref->SetInput(vdataref);
        vproj_ref->SetInputProjectionRef(vdataref->GetProjectionRef());
        vproj_ref->SetOutputKeywordList(imgref->GetImageKeywordlist());
        vproj_ref->SetOutputProjectionRef(vdataref->GetProjectionRef());
        // TODO add DEM support
        vproj_ref->Update();

        ListSampleGeneratorType::Pointer sampleGenerator_ref = ListSampleGeneratorType::New();

        sampleGenerator_ref->SetInput(imgref);
        sampleGenerator_ref->SetInputVectorData(vproj_ref->GetOutput());
        sampleGenerator_ref->SetValidationTrainingProportion(1.0); // All in validation

        sampleGenerator_ref->SetClassKey("Class");

        sampleGenerator_ref->Update();

        // Project the vectorData in the Image Coodinate system
        VectorDataProjectionFilterType::Pointer vproj_pro = VectorDataProjectionFilterType::New();
        vproj_pro->SetInput(vdatapro);
        vproj_pro->SetInputProjectionRef(vdatapro->GetProjectionRef());
        vproj_pro->SetOutputKeywordList(imgpro->GetImageKeywordlist());
        vproj_pro->SetOutputProjectionRef(vdatapro->GetProjectionRef());
        // TODO add DEM support
        vproj_pro->Update();

        ListSampleGeneratorType::Pointer sampleGenerator_pro = ListSampleGeneratorType::New();

        sampleGenerator_pro->SetInput(imgpro);
        sampleGenerator_pro->SetInputVectorData(vproj_pro->GetOutput());
        sampleGenerator_pro->SetValidationTrainingProportion(1.0); // All in validation

        sampleGenerator_pro->SetClassKey("Class");

        sampleGenerator_pro->Update();

        LabelListSampleType::Pointer validationLabeledListSampleRef = sampleGenerator_ref->GetValidationListLabel();
        LabelListSampleType::Pointer validationLabeledListSamplePro = sampleGenerator_pro->GetValidationListLabel();

        confusioncalculator->SetReferenceLabels(validationLabeledListSampleRef);
        confusioncalculator->SetProducedLabels(validationLabeledListSamplePro);

        confusioncalculator->Update();

        LINFO("Force Calculate completed successfully!");
    }
    catch (int e)
    {
    LERROR("Problem with confusion matrix!");
	return;
    }

}

void OTBConfusionMatrixCalculatorProcessor::saveToFile() {

    std::string filename = textFile_.get();
    if (!filename.empty())
    {
        hasFile = true;
    }

    if(hasFile)
    {
        // open the specified text file, write output and close
        std::ofstream myfile (filename.c_str());
        if (myfile.is_open())
        {
            // Has NOT been tested. Maybe a loop is needed for other outputs.
            myfile << "Confusion Matrix Calculator\n";
            myfile << "---------------------------\n";
            myfile << "OUTPUT\n";
            myfile << "======\n";
            myfile << "Confusion matrix: " << confusioncalculator->GetConfusionMatrix() << "\n";
            myfile.close();
            LINFO("File output written succesfully!");
        }else {
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


} // namespace

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

#include "otbfusionofclassificationsapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBFusionOfClassificationsApplication::loggerCat_("voreen.OTBFusionOfClassificationsApplication");

OTBFusionOfClassificationsApplication::OTBFusionOfClassificationsApplication()
    :OTBImageFilterProcessor(),
      inPort01_(Port::INPORT, "IN Multiband Image #1", 0),
      inPort02_(Port::INPORT, "IN Multiband Image #2", 0),
      inPort03_(Port::INPORT, "IN Multiband Image #3", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0),
      fusionMethod_("fusionMethod_", "Select fusion method"),
      csvPort01_(Port::INPORT, "IN CSV Text File #1", 0),
      csvPort02_(Port::INPORT, "IN CSV Text File #2", 0),
      csvPort03_(Port::INPORT, "IN CSV Text File #3", 0),      dempsterMOB("dempsterMOB", "Select mass of belief measurement type"),
      noDataLabel_("noDataLabel_", "Value for nodata pixels", 0),
      undecidedLabel_("undecidedLabel_", "Value for undecided pixels", 0)
{

    addProperty(enableSwitch_);
    addPort(inPort01_);
    addPort(inPort02_);
    addPort(inPort03_);
    addPort(csvPort01_);
    addPort(csvPort02_);
    addPort(csvPort03_);
    addPort(outPort_);
    addProperty(fusionMethod_);

    fusionMethod_.addOption("blank", "Select method:");
    fusionMethod_.addOption("majority", "Majority voting");
    fusionMethod_.addOption("dempster", "Dempster Shafer combination");

    fusionMethod_.onChange(CallMemberAction<OTBFusionOfClassificationsApplication>
                           (this,&OTBFusionOfClassificationsApplication::updateMethodSelection));


    addProperty(dempsterMOB);

    dempsterMOB.addOption("blank", "Select type:");
    dempsterMOB.addOption("precision", "Precision");
    dempsterMOB.addOption("recall", "Recall");
    dempsterMOB.addOption("overallAccuracy", "Overall Accuracy");
    dempsterMOB.addOption("Kappa", "Kappa");

    dempsterMOB.setVisible(false);

    addProperty(noDataLabel_);
    addProperty(undecidedLabel_);

    otbCaster = ImageCastFilterType::New();

}

Processor* OTBFusionOfClassificationsApplication::create() const {
    return new OTBFusionOfClassificationsApplication();
}

OTBFusionOfClassificationsApplication::~OTBFusionOfClassificationsApplication() {

}

void OTBFusionOfClassificationsApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBFusionOfClassificationsApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBFusionOfClassificationsApplication::getProcessorInfo() const {
    return "Fusion of Classifications Application";
}

void OTBFusionOfClassificationsApplication::updateMethodSelection() {

    if (fusionMethod_.get()=="dempster") {
        dempsterMOB.setVisible(true);
    } else {
        dempsterMOB.setVisible(false);
    }
}

bool OTBFusionOfClassificationsApplication::isReady() const {
    if (!isInitialized())
        return false;

    if(!outPort_.isConnected()) return false;

    if(!inPort01_.isConnected() && !inPort02_.isConnected() && !inPort03_.isConnected()) return false;

    if (fusionMethod_.get()=="dempster") {
        if(!csvPort01_.isConnected() && !csvPort02_.isConnected() && !csvPort03_.isConnected()) return false;
    }

    return true;
}

int OTBFusionOfClassificationsApplication::CSVConfusionMatrixFileReader(const std::string fileName, MapOfClassesType &mapOfClassesRefClX, ConfusionMatrixType &confusionMatrixClX)
{
    std::ifstream inFile;
    inFile.open(fileName.c_str());

    if (!inFile)
    {
        LERROR("Confusion Matrix File opening problem with file:" << fileName.c_str());
        return 0;
    }
    else
    {
        LabelImageType::PixelType labelRef = 0, labelProd = 0;
        std::string currentLine, refLabelsLine, prodLabelsLine, currentValue;
        const char endCommentChar = ':';
        const char separatorChar = ',';
        const char eolChar = '\n';
        std::getline(inFile, refLabelsLine, endCommentChar); // Skips the comments
        std::getline(inFile, refLabelsLine, eolChar); // Gets the first line after the comment char until the End Of Line char
        std::getline(inFile, prodLabelsLine, endCommentChar); // Skips the comments
        std::getline(inFile, prodLabelsLine, eolChar); // Gets the second line after the comment char until the End Of Line char

        std::istringstream issRefLabelsLine(refLabelsLine);
        std::istringstream issProdLabelsLine(prodLabelsLine);

        MapOfClassesType mapOfClassesProdClX;

        mapOfClassesRefClX.clear();
        mapOfClassesProdClX.clear();
        int itLab = 0;
        while (issRefLabelsLine.good())
        {
            std::getline(issRefLabelsLine, currentValue, separatorChar);
            labelRef = static_cast<LabelImageType::PixelType> (std::atoi(currentValue.c_str()));
            mapOfClassesRefClX[labelRef] = itLab;
            ++itLab;
        }

        itLab = 0;
        while (issProdLabelsLine.good())
        {
            std::getline(issProdLabelsLine, currentValue, separatorChar);
            labelProd = static_cast<LabelImageType::PixelType> (std::atoi(currentValue.c_str()));
            mapOfClassesProdClX[labelProd] = itLab;
            ++itLab;
        }

        unsigned int nbRefLabelsClk = mapOfClassesRefClX.size();
        unsigned int nbProdLabelsClk = mapOfClassesProdClX.size();
        ConfusionMatrixType confusionMatrixClXTemp;
        confusionMatrixClXTemp = ConfusionMatrixType(nbRefLabelsClk, nbProdLabelsClk);
        confusionMatrixClXTemp.Fill(0);

        // Reading the confusion matrix confusionMatrixClXTemp from the file
        for (unsigned int itRow = 0; itRow < nbRefLabelsClk; ++itRow)
        {
            //Gets the itRow^th line after the header lines with the labels
            std::getline(inFile, currentLine, eolChar);
            std::istringstream issCurrentLine(currentLine);
            unsigned int itCol = 0;
            while (issCurrentLine.good())
            {
                std::getline(issCurrentLine, currentValue, separatorChar);
                confusionMatrixClXTemp(itRow, itCol) = static_cast<ConfusionMatrixEltType> (std::atoi(currentValue.c_str()));
                ++itCol;
            }
        }

        MapOfClassesType::iterator  itMapOfClassesRef, itMapOfClassesProd;

        // Formatting confusionMatrixClX from confusionMatrixClXTemp in order to make confusionMatrixClX a square matrix
        // from the reference labels in mapOfClassesRefClX
        int indiceLabelRef = 0, indiceLabelProd = 0;
        int indiceLabelRefTemp = 0, indiceLabelProdTemp = 0;
        // Initialization of confusionMatrixClX
        confusionMatrixClX = ConfusionMatrixType(nbRefLabelsClk, nbRefLabelsClk);
        confusionMatrixClX.Fill(0);
        for (itMapOfClassesRef = mapOfClassesRefClX.begin(); itMapOfClassesRef != mapOfClassesRefClX.end(); ++itMapOfClassesRef)
        {
            // labels labelRef of mapOfClassesRefClX are already sorted
            labelRef = itMapOfClassesRef->first;
            indiceLabelRefTemp = itMapOfClassesRef->second;

            for (itMapOfClassesProd = mapOfClassesProdClX.begin(); itMapOfClassesProd != mapOfClassesProdClX.end(); ++itMapOfClassesProd)
            {
                // labels labelProd of mapOfClassesProdClX are already sorted
                labelProd = itMapOfClassesProd->first;
                indiceLabelProdTemp = itMapOfClassesProd->second;

                // If labelProd is present in mapOfClassesRefClX
                if (mapOfClassesRefClX.count(labelProd) != 0)
                {
                    // Indice of labelProd in mapOfClassesRefClX; itMapOfClassesRef->second elements are already SORTED
                    indiceLabelProd = mapOfClassesRefClX[labelProd];
                    confusionMatrixClX(indiceLabelRef, indiceLabelProd) = confusionMatrixClXTemp(indiceLabelRefTemp, indiceLabelProdTemp);
                }
            }
            ++indiceLabelRef;
        }
    }
    inFile.close();
    return 1;
}


void OTBFusionOfClassificationsApplication::process() {

    // Clear any previous filter
    m_Filters.clear();

    VectorImageListType* imageList;

    //Get all input images
    if (inPort01_.isConnected() && inPort01_.hasData()) {
        imageList->PushBack(inPort01_.getData());
    }
    if (inPort02_.isConnected() && inPort01_.hasData()) {
        imageList->PushBack(inPort02_.getData());
    }
    if (inPort03_.isConnected() && inPort01_.hasData()) {
        imageList->PushBack(inPort03_.getData());
    }

    if (fusionMethod_.get()=="majority")
    {
        LINFO("Fusion by Majority Voting");

        LabelVotingFilterType::Pointer labelVotingFilter = LabelVotingFilterType::New();
        labelVotingFilter->SetLabelForUndecidedPixels(undecidedLabel_.get());

        m_Filters.push_back(labelVotingFilter.GetPointer());

        //Iterate over all input images
        for (unsigned int imageId = 0; imageId < imageList->Size(); ++imageId)
        {
            VectorImageType* image = imageList->GetNthElement(imageId);

            ConverterType::Pointer converter = ConverterType::New();
            converter->SetInput(image);
            converter->SetChannel(1);

            labelVotingFilter->SetInput(imageId, converter->GetOutput());

            // Register filter
            m_Filters.push_back(converter.GetPointer());
        }
        otbCaster->SetInput(labelVotingFilter->GetOutput());

        try
        {
            otbCaster->Update();
        } catch (int e) {
            LERROR("Error in Fusion by Majority Voting!");
        }
        outPort_.setData(otbCaster->GetOutput());
    }
    else
    {
        if (fusionMethod_.get()=="dempster")
        {
            LINFO("Fusion with the Dempster Shafer method");

            ImageListType::Pointer imageListTemp = ImageListType::New();
            confusionMatrixToMassOfBeliefFilter = ConfusionMatrixToMassOfBeliefType::New();

            dsFusionOfClassifiersImageFilter = DSFusionOfClassifiersImageFilterType::New();
            dsFusionOfClassifiersImageFilter->SetLabelForUndecidedPixels(undecidedLabel_.get());

            m_Filters.push_back(confusionMatrixToMassOfBeliefFilter.GetPointer());
            m_Filters.push_back(dsFusionOfClassifiersImageFilter.GetPointer());

            //Get all confusion matrices filenames
            std::vector<std::string> confusionMatricesFilenameList;
            if (csvPort01_.isConnected() && csvPort01_.hasData()) {
                confusionMatricesFilenameList.push_back(csvPort01_.DataPath());
            }
            if (csvPort02_.isConnected() && csvPort02_.hasData()) {
                confusionMatricesFilenameList.push_back(csvPort02_.DataPath());
            }
            if (csvPort03_.isConnected() && csvPort03_.hasData()) {
                confusionMatricesFilenameList.push_back(csvPort03_.DataPath());
            }

            MassOfBeliefDefinitionMethod massOfBeliefDefMethod;

            if (dempsterMOB.get()=="precision"){
                LINFO("Masses of belief = PRECISION rates of each classifier (one rate per class label)");
                massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::PRECISION;
            } else {
                if (dempsterMOB.get()=="recall") {
                    LINFO("Masses of belief = RECALL rates of each classifier (one rate per class label)");
                    massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::RECALL;
                } else {
                    if (dempsterMOB.get()=="overallAccuracy") {
                        LINFO("Mass of belief = OVERALL ACCURACY of each classifier (one unique rate for all the class labels)");
                        massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::ACCURACY;
                    } else {
                        if (dempsterMOB.get()=="kappa") {
                            LINFO("Mass of belief = KAPPA coefficient of each classifier (one unique rate for all the class labels)");
                            massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::KAPPA;
                        } else {
                            LERROR("Non defined mass of belief type!");
                        }
                    }
                }
                //Iterate over all input images
                VectorOfMapOfMassesOfBeliefType vectorOfMapOfMassesOfBelief;
                for (unsigned int imageId = 0; imageId < imageList->Size(); ++imageId)
                {
                    VectorImageType* image = imageList->GetNthElement(imageId);

                    ConverterType::Pointer converter = ConverterType::New();
                    converter->SetInput(image);
                    converter->SetChannel(1);

                    imageListTemp->PushBack(converter->GetOutput());

                    MapOfClassesType mapOfClassesClk;
                    ConfusionMatrixType confusionMatrixClk;

                    CSVConfusionMatrixFileReader(confusionMatricesFilenameList[imageId], mapOfClassesClk, confusionMatrixClk);

                    confusionMatrixToMassOfBeliefFilter->SetMapOfClasses(mapOfClassesClk);
                    confusionMatrixToMassOfBeliefFilter->SetConfusionMatrix(confusionMatrixClk);
                    confusionMatrixToMassOfBeliefFilter->SetDefinitionMethod(massOfBeliefDefMethod);
                    try
                    {
                        confusionMatrixToMassOfBeliefFilter->Update();
                    } catch (int e) {
                        LERROR("Error in Confusion Matrix to Mass of Belief Filter!");
                    }

                    // Vector containing ALL the K (= nbClassifiers) std::map<Label, MOB> of Masses of Belief
                    vectorOfMapOfMassesOfBelief.push_back(confusionMatrixToMassOfBeliefFilter->GetMapMassOfBelief());
                    // Register filter
                    m_Filters.push_back(converter.GetPointer());
                }

                // Image List To VectorImage
                ImageListToVectorImageFilterType::Pointer imageListToVectorImageFilter = ImageListToVectorImageFilterType::New();
                imageListToVectorImageFilter->SetInput(imageListTemp);
                try
                {
                    imageListToVectorImageFilter->Update();
                } catch (int e) {
                    LERROR("Error in Image List To Vector Image Filter!");
                }

                LINFO("Number of input classification maps: " << imageListToVectorImageFilter->GetOutput()->GetNumberOfComponentsPerPixel());
                LINFO("Number of input confusion matrix files: " << vectorOfMapOfMassesOfBelief.size());
                LabelMassMapType::iterator itMap;
                for (unsigned int k = 0; k < vectorOfMapOfMassesOfBelief.size(); ++k)
                {
                    LabelMassMapType mapOfMassesOfBelief = vectorOfMapOfMassesOfBelief[k];
                    LINFO("Classifier[" << k << "]: ");
                    LINFO("\tConfusion Matrix file_" << k << ": " << confusionMatricesFilenameList[k]);
                    for (itMap = mapOfMassesOfBelief.begin(); itMap != mapOfMassesOfBelief.end(); ++itMap)
                    {
                        LINFO("\t\tMassOfBelief_Cl_" << k << "[label_" << itMap->first << "] = " << itMap->second );
                    }
                    LINFO("");
                }

                // DSFusionOfClassifiersImageFilter Inputs
                dsFusionOfClassifiersImageFilter->SetInput(imageListToVectorImageFilter->GetOutput());
                dsFusionOfClassifiersImageFilter->SetInputMapsOfMassesOfBelief(&vectorOfMapOfMassesOfBelief);
                dsFusionOfClassifiersImageFilter->SetLabelForNoDataPixels(noDataLabel_.get());
                dsFusionOfClassifiersImageFilter->SetLabelForUndecidedPixels(undecidedLabel_.get());

                //dsFusionOfClassifiersImageFilter->Update();

                otbCaster->SetInput(dsFusionOfClassifiersImageFilter->GetOutput());
                try
                {
                    otbCaster->Update();
                } catch (int e) {
                    LERROR("Error in DS Fusion of Classifiers Image Filter");
                }
                outPort_.setData(otbCaster->GetOutput());
            }
        }

    }
}

}   // namespace

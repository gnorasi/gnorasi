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

#include "otbtrainimagesclassifierapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBTrainImagesClassifierApplication::loggerCat_("voreen.OTBTrainImagesClassifierApplication");

Processor* OTBTrainImagesClassifierApplication::create() const {
    return new OTBTrainImagesClassifierApplication();
}

OTBTrainImagesClassifierApplication::~OTBTrainImagesClassifierApplication() {

}

void OTBTrainImagesClassifierApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBTrainImagesClassifierApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBTrainImagesClassifierApplication::getProcessorInfo() const {
    return "Train Images Classifier Application";
}

bool OTBTrainImagesClassifierApplication::isReady() const {

    if (!isInitialized())
        return false;

    if(!modelOutport_.isConnected() || !matrixOutport_.isConnected()) return false;

    if(!inPort01_.isConnected() && !inPort02_.isConnected() && !inPort03_.isConnected()) return false;

    if(!inDataPort01_.isConnected() && !inDataPort02_.isConnected() && !inDataPort03_.isConnected()) return false;

    if (enableXMLinput_.get() && XMLfilename_.get().empty()) return false;

    return true;
}

void OTBTrainImagesClassifierApplication::LogConfusionMatrix(ConfusionMatrixCalculatorType* confMatCalc)
{
    ConfusionMatrixCalculatorType::ConfusionMatrixType matrix = confMatCalc->GetConfusionMatrix();

    // Compute minimal width
    size_t minwidth = 0;

    for (unsigned int i = 0; i < matrix.Rows(); i++)
    {
        for (unsigned int j = 0; j < matrix.Cols(); j++)
        {
            std::ostringstream os;
            os << matrix(i, j);
            size_t size = os.str().size();

            if (size > minwidth)
            {
                minwidth = size;
            }
        }
    }

    MapOfIndicesType mapOfIndices = confMatCalc->GetMapOfIndices();

    MapOfIndicesType::const_iterator it = mapOfIndices.begin();
    MapOfIndicesType::const_iterator end = mapOfIndices.end();

    for (; it != end; ++it)
    {
        std::ostringstream os;
        os << "[" << it->second << "]";

        size_t size = os.str().size();
        if (size > minwidth)
        {
            minwidth = size;
        }
    }

    // Generate matrix string, with 'minwidth' as size specifier
    std::ostringstream os;

    // Header line
    for (size_t i = 0; i < minwidth; ++i)
        os << " ";
    os << " ";

    it = mapOfIndices.begin();
    end = mapOfIndices.end();
    for (; it != end; ++it)
    {
        os << "[" << it->second << "]" << " ";
    }

    os << std::endl;

    // Each line of confusion matrix
    for (unsigned int i = 0; i < matrix.Rows(); i++)
    {
        ConfusionMatrixCalculatorType::ClassLabelType label = mapOfIndices[i];
        os << "[" << std::setw(minwidth - 2) << label << "]" << " ";
        for (unsigned int j = 0; j < matrix.Cols(); j++)
        {
            os << std::setw(minwidth) << matrix(i, j) << " ";
        }
        os << std::endl;
    }

    LINFO("Confusion matrix (rows = reference labels, columns = produced labels):\n" << os.str());
}

void OTBTrainImagesClassifierApplication::Classify(ListSampleType::Pointer validationListSample, LabelListSampleType::Pointer predictedList)
{
    ///< TRIGGERS REFERENCE ERROR

    //    Classification
    model = MachineLearningModelFactoryType::CreateMachineLearningModel(modelOutport_.DataPath(),
                                                                        MachineLearningModelFactoryType::ReadMode);
    if (model.IsNull())
    {
        LERROR("Error when loading model " << modelOutport_.DataPath());
    }

    model->Load(modelOutport_.DataPath());
    model->SetInputListSample(validationListSample);
    model->SetTargetListSample(predictedList);
    model->PredictAll();
}

void OTBTrainImagesClassifierApplication::process() {

    //Create training and validation for list samples and label list samples
    ConcatenateLabelListSampleFilterType::Pointer concatenateTrainingLabels =
            ConcatenateLabelListSampleFilterType::New();
    ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples = ConcatenateListSampleFilterType::New();
    ConcatenateLabelListSampleFilterType::Pointer concatenateValidationLabels =
            ConcatenateLabelListSampleFilterType::New();
    ConcatenateListSampleFilterType::Pointer concatenateValidationSamples = ConcatenateListSampleFilterType::New();

    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;

    //--------------------------
    // Load measurements from images
    unsigned int nbBands = 0;

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

    VectorDataListType* vectorDataList;

    //Get all input vector data
    if (inDataPort01_.isConnected() && inDataPort01_.hasData()) {
        vectorDataList->PushBack(inDataPort01_.getData());
    }
    if (inDataPort02_.isConnected() && inDataPort02_.hasData()) {
        vectorDataList->PushBack(inDataPort02_.getData());
    }
    if (inDataPort03_.isConnected() && inDataPort03_.hasData()) {
        vectorDataList->PushBack(inDataPort03_.getData());
    }

    vdreproj = VectorDataReprojectionType::New();

    //Iterate over all input images
    for (unsigned int imgIndex = 0; imgIndex < imageList->Size(); ++imgIndex)
    {
        VectorImageType::Pointer image = imageList->GetNthElement(imgIndex);
        image->UpdateOutputInformation();

        if (imgIndex == 0)
        {
            nbBands = image->GetNumberOfComponentsPerPixel();
        }

        // read the Vectordata
        VectorDataType::Pointer vectorData = vectorDataList->GetNthElement(imgIndex);
        vectorData->Update();

        vdreproj->SetInputImage(image);
        vdreproj->SetInput(vectorData);
        vdreproj->SetUseOutputSpacingAndOriginFromImage(false);

        ///< to be implemented along elev parameters
        ////         Setup the DEM Handler
        //        otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

        vdreproj->Update();

        //Sample list generator
        ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

        sampleGenerator->SetInput(image);
        sampleGenerator->SetInputVectorData(vdreproj->GetOutput());

        sampleGenerator->SetClassKey(sampleVfn_.get());
        sampleGenerator->SetMaxTrainingSize(sampleMt_.get());
        sampleGenerator->SetMaxValidationSize(sampleMv_.get());
        sampleGenerator->SetValidationTrainingProportion(sampleVtr_.get());

        // take pixel located on polygon edge into consideration
        if (sampleEdg_.get())
        {
            sampleGenerator->SetPolygonEdgeInclusion(true);
        }

        sampleGenerator->Update();

        //Concatenate training and validation samples from the image
        concatenateTrainingLabels->AddInput(sampleGenerator->GetTrainingListLabel());
        concatenateTrainingSamples->AddInput(sampleGenerator->GetTrainingListSample());
        concatenateValidationLabels->AddInput(sampleGenerator->GetValidationListLabel());
        concatenateValidationSamples->AddInput(sampleGenerator->GetValidationListSample());
    }
    // Update
    concatenateTrainingSamples->Update();
    concatenateTrainingLabels->Update();
    concatenateValidationSamples->Update();
    concatenateValidationLabels->Update();

    if (concatenateTrainingSamples->GetOutputSampleList()->Size() == 0)
    {
        LERROR("No training samples, cannot perform SVM training.");
    }

    if (concatenateValidationSamples->GetOutputSampleList()->Size() == 0)
    {
        LWARNING("No validation samples.");
    }

    if (enableXMLinput_.get())
    {
        StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
        statisticsReader->SetFileName(XMLfilename_.get());
        meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
        stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
    }
    else
    {
        meanMeasurementVector.SetSize(nbBands);
        meanMeasurementVector.Fill(0.);
        stddevMeasurementVector.SetSize(nbBands);
        stddevMeasurementVector.Fill(1.);
    }

    // Shift scale the samples
    ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
    trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
    trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
    trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
    trainingShiftScaleFilter->Update();

    ListSampleType::Pointer validationListSample=ListSampleType::New();

    //Test if the validation test is empty
    if ( concatenateValidationSamples->GetOutput()->Get()->Size() != 0 )
    {
        ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
        validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
        validationShiftScaleFilter->SetShifts(meanMeasurementVector);
        validationShiftScaleFilter->SetScales(stddevMeasurementVector);
        validationShiftScaleFilter->Update();
        validationListSample = validationShiftScaleFilter->GetOutputSampleList();
    }

    ListSampleType::Pointer listSample;
    LabelListSampleType::Pointer labelListSample;

    listSample = trainingShiftScaleFilter->GetOutputSampleList();
    labelListSample = concatenateTrainingLabels->GetOutputSampleList();
    LINFO("Number of training samples: " << concatenateTrainingSamples->GetOutputSampleList()->Size());

    // Split the data set into training/validation set
    ListSampleType::Pointer trainingListSample = listSample;
    LabelListSampleType::Pointer trainingLabeledListSample = labelListSample;

    LabelListSampleType::Pointer validationLabeledListSample = concatenateValidationLabels->GetOutputSampleList();
    LINFO("Size of training set: " << trainingListSample->Size());
    LINFO("Size of validation set: " << validationListSample->Size());
    LINFO("Size of labeled training set: " << trainingLabeledListSample->Size());
    LINFO("Size of labeled validation set: " << validationLabeledListSample->Size());

    //--------------------------
    // Estimate model
    //--------------------------
    LabelListSampleType::Pointer predictedList = LabelListSampleType::New();
    const std::string classifierType = classifier_.get();


    ///< Implement classifier functions

        if (classifierType == "libsvm")
        {
            TrainLibSVM(trainingListSample, trainingLabeledListSample);
        }
    #ifdef OTB_USE_OPENCV
        else if (classifierType == "svm")
        {
            TrainSVM(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "boost")
        {
            TrainBoost(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "dt")
        {
            TrainDecisionTree(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "gbt")
        {
            TrainGradientBoostedTree(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "ann")
        {
            TrainNeuralNetwork(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "bayes")
        {
            TrainNormalBayes(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "rf")
        {
            TrainRandomForests(trainingListSample, trainingLabeledListSample);
        }
        else if (classifierType == "knn")
        {
            TrainKNN(trainingListSample, trainingLabeledListSample);
        }
    #endif

    //--------------------------
    // Performances estimation
    //--------------------------
    ListSampleType::Pointer performanceListSample=ListSampleType::New();
    LabelListSampleType::Pointer performanceLabeledListSample=LabelListSampleType::New();

    //Test the input validation set size
    if(validationLabeledListSample->Size() != 0)
    {
        performanceListSample = validationListSample;
        performanceLabeledListSample = validationLabeledListSample;
    }
    else
    {
        LWARNING("The validation set is empty. The performance estimation is done using the input training set in this case.");
        performanceListSample = trainingListSample;
        performanceLabeledListSample = trainingLabeledListSample;
    }

    Classify(performanceListSample, predictedList);

    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    LINFO("predicted list size == " << predictedList->Size());
    LINFO("validationLabeledListSample size == " << performanceLabeledListSample->Size());
    confMatCalc->SetReferenceLabels(performanceLabeledListSample);
    confMatCalc->SetProducedLabels(predictedList);

    confMatCalc->Compute();

    LINFO("training performances");
    LogConfusionMatrix(confMatCalc);

    for (unsigned int itClasses = 0; itClasses < confMatCalc->GetNumberOfClasses(); itClasses++)
    {
        ConfusionMatrixCalculatorType::ClassLabelType classLabel = confMatCalc->GetMapOfIndices()[itClasses];

        LINFO("Precision of class [" << classLabel << "] vs all: " << confMatCalc->GetPrecisions()[itClasses]);
        LINFO("Recall of class    [" << classLabel << "] vs all: " << confMatCalc->GetRecalls()[itClasses]);
        LINFO("F-score of class   [" << classLabel << "] vs all: " << confMatCalc->GetFScores()[itClasses]);
    }
    LINFO("Global performance, Kappa index: " << confMatCalc->GetKappaIndex());


    if (matrixOutport_.isConnected())
    {
        // Writing the confusion matrix in the output .CSV file

        MapOfIndicesType::iterator itMapOfIndicesValid, itMapOfIndicesPred;
        ClassLabelType labelValid = 0;

        ConfusionMatrixType confusionMatrix = confMatCalc->GetConfusionMatrix();
        MapOfIndicesType mapOfIndicesValid = confMatCalc->GetMapOfIndices();

        unsigned int nbClassesPred = mapOfIndicesValid.size();

        /////////////////////////////////////////////
        // Filling the 2 headers for the output file
        const std::string commentValidStr = "#Reference labels (rows):";
        const std::string commentPredStr = "#Produced labels (columns):";
        const char separatorChar = ',';
        std::ostringstream ossHeaderValidLabels, ossHeaderPredLabels;

        // Filling ossHeaderValidLabels and ossHeaderPredLabels for the output file
        ossHeaderValidLabels << commentValidStr;
        ossHeaderPredLabels << commentPredStr;

        itMapOfIndicesValid = mapOfIndicesValid.begin();

        while (itMapOfIndicesValid != mapOfIndicesValid.end())
        {
            // labels labelValid of mapOfIndicesValid are already sorted in otbConfusionMatrixCalculator
            labelValid = itMapOfIndicesValid->second;

            LINFO("mapOfIndicesValid[" << itMapOfIndicesValid->first << "] = " << labelValid);

            ossHeaderValidLabels << labelValid;
            ossHeaderPredLabels << labelValid;

            ++itMapOfIndicesValid;

            if (itMapOfIndicesValid != mapOfIndicesValid.end())
            {
                ossHeaderValidLabels << separatorChar;
                ossHeaderPredLabels << separatorChar;
            }
            else
            {
                ossHeaderValidLabels << std::endl;
                ossHeaderPredLabels << std::endl;
            }
        }

        std::ostringstream outFile;
        outFile << std::fixed;
        outFile.precision(10);

        /////////////////////////////////////
        // Writing the 2 headers
        outFile << ossHeaderValidLabels.str();
        outFile << ossHeaderPredLabels.str();
        /////////////////////////////////////

        unsigned int indexLabelValid = 0, indexLabelPred = 0;

        for (itMapOfIndicesValid = mapOfIndicesValid.begin(); itMapOfIndicesValid != mapOfIndicesValid.end(); ++itMapOfIndicesValid)
        {
            indexLabelPred = 0;

            for (itMapOfIndicesPred = mapOfIndicesValid.begin(); itMapOfIndicesPred != mapOfIndicesValid.end(); ++itMapOfIndicesPred)
            {
                // Writing the confusion matrix (sorted in otbConfusionMatrixCalculator) in the output file
                outFile << confusionMatrix(indexLabelValid, indexLabelPred);
                if (indexLabelPred < (nbClassesPred - 1))
                {
                    outFile << separatorChar;
                }
                else
                {
                    outFile << std::endl;
                }
                ++indexLabelPred;
            }

            ++indexLabelValid;
        }
        matrixOutport_.setData(outFile.str());
    }
}

void OTBTrainImagesClassifierApplication::TrainLibSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
    LibSVMType::Pointer libSVMClassifier = LibSVMType::New();
    libSVMClassifier->SetInputListSample(trainingListSample);
    libSVMClassifier->SetTargetListSample(trainingLabeledListSample);
    //SVM Option
    if (svmOpt_.get())
    {
        libSVMClassifier->SetParameterOptimization(true);
    }
    libSVMClassifier->SetC(svmCost_.get());

    if (libsvmKernel_.get()=="linear")
        libSVMClassifier->SetKernelType(LINEAR);
    else if (libsvmKernel_.get()=="gaussian")
        libSVMClassifier->SetKernelType(RBF);
    else if (libsvmKernel_.get()=="polynomial")
        libSVMClassifier->SetKernelType(POLY);
    else if (libsvmKernel_.get()=="sigmoid")
        libSVMClassifier->SetKernelType(SIGMOID);
    else if (libsvmKernel_.get()=="gaussian")
        libSVMClassifier->SetKernelType(LINEAR);
    else {
        LWARNING("Non defined kernel type!");
        return;
    }
    libSVMClassifier->Train();
    libSVMClassifier->Save(modelOutport_.DataPath());
}

#ifdef OTB_USE_OPENCV ///< OpenCV library is needed for the following functions
void OTBTrainImagesClassifierApplication::TrainSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
    SVMType::Pointer SVMClassifier = SVMType::New();
    SVMClassifier->SetInputListSample(trainingListSample);
    SVMClassifier->SetTargetListSample(trainingLabeledListSample);

    if (svmKernel_.get=="linear")
        SVMClassifier->SetKernelType(CvSVM::LINEAR);
    else if (svmKernel_.get()=="gaussian")
        SVMClassifier->SetKernelType(CvSVM::RBF);
    else if (svmKernel_.get()=="polynomial")
        SVMClassifier->SetKernelType(CvSVM::POLY);
    else if (svmKernel_.get()=="sigmoid")
        SVMClassifier->SetKernelType(CvSVM::SIGMOID);
    else if (svmKernel_.get()=="gaussian")
        SVMClassifier->SetKernelType(CvSVM::LINEAR);
    else {
        LWARNING("Non defined kernel type!");
        return;
    }

    if (svmType_.get=="cSupport")
        SVMClassifier->SetSVMType(CvSVM::C_SVC);
    else if (svmType_.get()=="nuSupport")
        SVMClassifier->SetSVMType(CvSVM::NU_SVC);
    else if (svmType_.get()=="distrEstimation")
        SVMClassifier->SetSVMType(CvSVM::ONE_CLASS);
    else {
        LWARNING("Non defined model type!");
        return;
    }
    SVMClassifier->SetC(svmCost_.get());
    SVMClassifier->SetNu(svmNu_.get());
    SVMClassifier->SetCoef0(svmCoef0_.get());
    SVMClassifier->SetGamma(svmGamma_.get());
    SVMClassifier->SetDegree(svmDegree_.get());

    if (svmOpt_.get())
    {
        SVMClassifier->SetParameterOptimization(true);
    }
    SVMClassifier->Train();
    SVMClassifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainBoost(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
    BoostType::Pointer boostClassifier = BoostType::New();
    boostClassifier->SetInputListSample(trainingListSample);
    boostClassifier->SetTargetListSample(trainingLabeledListSample);

    if (boostType_.get=="discrete")
        boostClassifier->SetBoostType(CvBoost::DISCRETE);
    else if (boostType_.get()=="real")
        boostClassifier->SetBoostType(CvBoost::REAL);
    else if (boostType_.get()=="logit")
        boostClassifier->SetBoostType(CvBoost::LOGIT);
    else if (boostType_.get()=="gentle")
        boostClassifier->SetBoostType(CvBoost::GENTLE);
    else {
        LWARNING("Non defined boost type!");
        return;
    }
    boostClassifier->SetWeakCount(boostWeak_.get());
    boostClassifier->SetWeightTrimRate(boostR_.get());
    boostClassifier->SetMaxDepth(boostMax_.get());

    boostClassifier->Train();
    boostClassifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainDecisionTree(ListSampleType::Pointer trainingListSample,
                                                            LabelListSampleType::Pointer trainingLabeledListSample)
{
    DecisionTreeType::Pointer classifier = DecisionTreeType::New();
    classifier->SetInputListSample(trainingListSample);
    classifier->SetTargetListSample(trainingLabeledListSample);
    classifier->SetMaxDepth(dtMax_.get());
    classifier->SetMinSampleCount(dtMin_.get());
    classifier->SetRegressionAccuracy(dtRa_.get());
    classifier->SetMaxCategories(dtCat_.get());
    classifier->SetCVFolds(dtF_.get());
    if (dtR_.get())
    {
        classifier->SetUse1seRule(false);
    }
    if (dtT_.get())
    {
        classifier->SetTruncatePrunedTree(false);
    }
    classifier->Train();
    classifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainGradientBoostedTree(
        ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
    GradientBoostedTreeType::Pointer classifier = GradientBoostedTreeType::New();
    classifier->SetInputListSample(trainingListSample);
    classifier->SetTargetListSample(trainingLabeledListSample);
    classifier->SetWeakCount(gbtW_.get());
    classifier->SetShrinkage(gbtS_.get());
    classifier->SetSubSamplePortion(gbtP_.get());
    classifier->SetMaxDepth(gbtMax_.get());

    classifier->Train();
    classifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainNeuralNetwork(ListSampleType::Pointer trainingListSample,
                                                             LabelListSampleType::Pointer trainingLabeledListSample)
{
    NeuralNetworkType::Pointer classifier = NeuralNetworkType::New();
    classifier->SetInputListSample(trainingListSample);
    classifier->SetTargetListSample(trainingLabeledListSample);

    if (annType_.get()=="reg")
        classifier->SetTrainMethod(CvANN_MLP_TrainParams::RPROP);
    else if (annType_.get()=="back")
        classifier->SetTrainMethod(CvANN_MLP_TrainParams::BACKPROP);
    else {
        LWARNING("Non defined train method type!");
        return;
    }

    std::vector<unsigned int> layerSizes;
    std::vector<std::string> sizes = annSizes_.get();

    unsigned int nbImageBands = trainingListSample->GetMeasurementVectorSize();
    layerSizes.push_back(nbImageBands);
    for (unsigned int i = 0; i < sizes.size(); i++)
    {
        unsigned int nbNeurons = boost::lexical_cast<unsigned int>(sizes[i]);
        layerSizes.push_back(nbNeurons);
    }

    unsigned int nbClasses = 0;
    LabelType currentLabel = 0, prevLabel = 0;
    for (unsigned int itLab = 0; itLab < trainingLabeledListSample->Size(); ++itLab)
    {
        currentLabel = trainingLabeledListSample->GetMeasurementVector(itLab);
        if ((currentLabel != prevLabel) || (itLab == 0))
        {
            ++nbClasses;
        }
        prevLabel = currentLabel;
    }

    layerSizes.push_back(nbClasses);
    classifier->SetLayerSizes(layerSizes);

    if (annFunction_.get()=="ident")
        classifier->SetActivateFunction(CvANN_MLP::IDENTITY);
    else if (annFunction_.get()=="sig")
        classifier->SetActivateFunction(CvANN_MLP::SIGMOID_SYM);
    else if (annFunction_.get()=="gau")
        classifier->SetActivateFunction(CvANN_MLP::GAUSSIAN);
    else {
        LWARNING("Non defined neuron activation function!");
        return;
    }

    classifier->SetAlpha(annAlpha_.get());
    classifier->SetBeta(annBeta_.get());
    classifier->SetBackPropDWScale(annBpdw_.get());
    classifier->SetBackPropMomentScale(annBpms_.get());
    classifier->SetRegPropDW0(annRdw_.get());
    classifier->SetRegPropDWMin(annRdwm_.get());

    if (annTerm_.get()=="iter")
        classifier->SetTermCriteriaType(CV_TERMCRIT_ITER);
    else if (annTerm_.get()=="eps")
        classifier->SetTermCriteriaType(CV_TERMCRIT_EPS);
    else if (annTerm_.get()=="all")
        classifier->SetTermCriteriaType(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS);
    else {
        LWARNING("Non defined termination criteria!");
        return;
    }

    classifier->SetEpsilon(annEps_.get());
    classifier->SetMaxIter(annIter_.get());
    classifier->Train();
    classifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainNormalBayes(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
  NormalBayesType::Pointer classifier = NormalBayesType::New();
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->Train();
  classifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainRandomForests(ListSampleType::Pointer trainingListSample,
                                                              LabelListSampleType::Pointer trainingLabeledListSample)
{
  RandomForestType::Pointer classifier = RandomForestType::New();
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetMaxDepth(rfMax_.get());
  classifier->SetMinSampleCount(rfMin_.get());
  classifier->SetRegressionAccuracy(rfRa_.get());
  classifier->SetMaxNumberOfCategories(rfCat_.get());
  classifier->SetMaxNumberOfVariables(rfVar_.get());
  classifier->SetMaxNumberOfTrees(rfNbTrees_.get());
  classifier->SetForestAccuracy(rfAcc_.get());

  classifier->Train();
  classifier->Save(modelOutport_.DataPath());
}

void OTBTrainImagesClassifierApplication::TrainKNN(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
  KNNType::Pointer knnClassifier = KNNType::New();
  knnClassifier->SetInputListSample(trainingListSample);
  knnClassifier->SetTargetListSample(trainingLabeledListSample);
  knnClassifier->SetK(knnNeigbors_.get());

  knnClassifier->Train();
  knnClassifier->Save(modelOutport_.DataPath());
}
#endif


}   // namespace

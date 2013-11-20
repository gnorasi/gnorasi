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

#ifndef OTBTRAINIMAGESCLASSIFIERAPPLICATION_H
#define OTBTRAINIMAGESCLASSIFIERAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbimageport.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "modules/otb/ports/otbvectordataport.h"
#include "modules/otb/ports/otbcsvtextport.h"

#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/floatproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"

//Image
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbListSampleGenerator.h"

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"

//Estimator
#include "otbMachineLearningModelFactory.h"

#ifdef OTB_USE_OPENCV
# include "otbKNearestNeighborsMachineLearningModel.h"
# include "otbRandomForestsMachineLearningModel.h"
# include "otbSVMMachineLearningModel.h"
# include "otbBoostMachineLearningModel.h"
# include "otbDecisionTreeMachineLearningModel.h"
# include "otbGradientBoostedTreeMachineLearningModel.h"
# include "otbNormalBayesMachineLearningModel.h"
# include "otbNeuralNetworkMachineLearningModel.h"
#endif

#include "otbLibSVMMachineLearningModel.h"

// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Validation
#include "otbConfusionMatrixCalculator.h"

#include "itkTimeProbe.h"
#include "otbStandardFilterWatcher.h"

// Normalize the samples
#include "otbShiftScaleSampleListFilter.h"

// List sample concatenation
#include "otbConcatenateSampleListFilter.h"

// Balancing ListSample
#include "otbListSampleToBalancedListSampleFilter.h"

// VectorData projection filter
#include "otbVectorDataProjectionFilter.h"

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

#include "otbImageList.h"


namespace voreen {

class OTBTrainImagesClassifierApplication : public OTBImageFilterProcessor
{
public:
    OTBTrainImagesClassifierApplication();

    virtual ~OTBTrainImagesClassifierApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Train Images Classifier Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    /** Images typedefs */
    typedef ImageType               MaskImageType;
    typedef otb::Image<unsigned int, 2>       LabelImageType;
    typedef otb::VectorImage<double, 2> VectorImageType;
    typedef otb::VectorData<double, 2> VectorDataType;

    typedef VectorImageType::PixelType         PixelType;
    typedef VectorImageType::InternalPixelType InternalPixelType;
    typedef otb::ImageList<VectorImageType>    VectorImageListType;
    typedef otb::ImageList<VectorDataType>     VectorDataListType;


    // Training vectordata
    typedef itk::VariableLengthVector<InternalPixelType> MeasurementType;

    // SampleList manipulation
    typedef otb::ListSampleGenerator<VectorImageType, VectorDataType> ListSampleGeneratorType;

    typedef ListSampleGeneratorType::ListSampleType ListSampleType;
    typedef ListSampleGeneratorType::LabelType LabelType;
    typedef ListSampleGeneratorType::ListLabelType LabelListSampleType;
    typedef otb::Statistics::ConcatenateSampleListFilter<ListSampleType> ConcatenateListSampleFilterType;
    typedef otb::Statistics::ConcatenateSampleListFilter<LabelListSampleType> ConcatenateLabelListSampleFilterType;

    // Statistic XML file Reader
    typedef otb::StatisticsXMLFileReader<MeasurementType> StatisticsReader;

    // Enhance List Sample  typedef otb::Statistics::ListSampleToBalancedListSampleFilter<ListSampleType, LabelListSampleType>      BalancingListSampleFilterType;
    typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

    // Machine Learning models
    typedef otb::MachineLearningModelFactory<InternalPixelType, ListSampleGeneratorType::ClassLabelType> MachineLearningModelFactoryType;
    typedef MachineLearningModelFactoryType::MachineLearningModelTypePointer ModelPointerType;
    ModelPointerType model;

#ifdef OTB_USE_OPENCV
    typedef otb::RandomForestsMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> RandomForestType;
    typedef otb::KNearestNeighborsMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> KNNType;
    typedef otb::SVMMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> SVMType;
    typedef otb::BoostMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> BoostType;
    typedef otb::DecisionTreeMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> DecisionTreeType;
    typedef otb::GradientBoostedTreeMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> GradientBoostedTreeType;
    typedef otb::NeuralNetworkMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> NeuralNetworkType;
    typedef otb::NormalBayesMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> NormalBayesType;
#endif

    typedef otb::LibSVMMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> LibSVMType;

    // Estimate performance on validation sample
    typedef otb::ConfusionMatrixCalculator<LabelListSampleType, LabelListSampleType> ConfusionMatrixCalculatorType;
    typedef ConfusionMatrixCalculatorType::ConfusionMatrixType ConfusionMatrixType;
    typedef ConfusionMatrixCalculatorType::MapOfIndicesType MapOfIndicesType;
    typedef ConfusionMatrixCalculatorType::ClassLabelType ClassLabelType;


    // VectorData projection filter
    typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;

    // Extract ROI
    typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, VectorImageType> VectorDataReprojectionType;
    VectorDataReprojectionType::Pointer vdreproj;


protected:

    virtual void setDescriptions() {
        setDescription("Train a classifier from multiple pairs of images and training vector data.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual bool isReady() const;

    void updateClassifierSelection(); ///< Properties visibility configuration.

    void LogConfusionMatrix(ConfusionMatrixCalculatorType* confMatCalc);
    void Classify(ListSampleType::Pointer validationListSample, LabelListSampleType::Pointer predictedList);

    void TrainLibSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainDecisionTree(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainGradientBoostedTree(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainNeuralNetwork(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainNormalBayes(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainRandomForests(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
    void TrainKNN(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);

private:

    // IO
    OTBVectorImagePort inPort01_;
    OTBVectorImagePort inPort02_;
    OTBVectorImagePort inPort03_;

    OTBVectorDataPort inDataPort01_;
    OTBVectorDataPort inDataPort02_;
    OTBVectorDataPort inDataPort03_;

    BoolProperty enableXMLinput_;
    FileDialogProperty XMLfilename_;

    OTBCSVTextPort matrixOutport_;
    OTBCSVTextPort modelOutport_;

    // elev
    ///< to be implemented

    // sample
    IntProperty sampleMt_;
    IntProperty sampleMv_;
    BoolProperty sampleEdg_;
    FloatProperty sampleVtr_;
    StringProperty sampleVfn_;

    // classifier filters
    StringOptionProperty classifier_;

    // LibSVM
    StringOptionProperty libsvmKernel_;
    FloatProperty libsvmCost_;
    BoolProperty libsvmOpt_;

    // SVM
    StringOptionProperty svmType_;
    StringOptionProperty svmKernel_;
    FloatProperty svmCost_;
    FloatProperty svmNu_;
    FloatProperty svmCoef0_;
    FloatProperty svmGamma_;
    FloatProperty svmDegree_;
    BoolProperty svmOpt_;

    // Boost
    StringOptionProperty boostType_;
    IntProperty boostWeak_;
    FloatProperty boostR_;
    IntProperty boostMax_;

    // Decision Tree
    IntProperty dtMax_;
    IntProperty dtMin_;
    FloatProperty dtRa_;
    IntProperty dtCat_;
    IntProperty dtF_;
    BoolProperty dtR_;
    BoolProperty dtT_;

    //Gradient Boosted Tree
    IntProperty gbtW_;
    FloatProperty gbtS_;
    FloatProperty gbtP_;
    IntProperty gbtMax_;

    //Artificial Neural Network
    StringOptionProperty annType_;
    StringProperty annSizes_;       ///< implemented as a stringlist
    StringOptionProperty annFunction_;
    FloatProperty annAlpha_;
    FloatProperty annBeta_;
    FloatProperty annBpdw_;
    FloatProperty annBpms_;
    FloatProperty annRdw_;
    FloatProperty annRdwm_;
    StringOptionProperty annTerm_;
    FloatProperty annEps_;
    IntProperty annIter_;

    // Random Forests
    IntProperty rfMax_;
    IntProperty rfMin_;
    FloatProperty rfRa_;
    IntProperty rfCat_;
    IntProperty rfVar_;
    IntProperty rfNbTrees_;
    FloatProperty rfAcc_;

    // KNN
    IntProperty knnNeigbors_;

    IntProperty RAND_;




    static const std::string loggerCat_; ///< Category used in logging
};

} // namespace

#endif // OTBTRAINIMAGESCLASSIFIERAPPLICATION_H

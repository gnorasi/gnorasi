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

OTBTrainImagesClassifierApplication::OTBTrainImagesClassifierApplication()
    :OTBImageFilterProcessor(),
      inPort01_(Port::INPORT, "IN Multiband Image #1", 0),
      inPort02_(Port::INPORT, "IN Multiband Image #2", 0),
      inPort03_(Port::INPORT, "IN Multiband Image #3", 0),
      inDataPort01_(Port::INPORT, "IN Vector Data #1", 0),
      inDataPort02_(Port::INPORT, "IN Vector Data #2", 0),
      inDataPort03_(Port::INPORT, "IN Vector Data #3", 0),
      enableXMLinput_("enableXMLinput_", "Enable XML input", false),
      XMLfilename_("XMLfilename_", "Input XML Statistics File:", "XML File", VoreenApplication::app()->getUserDataPath(), "XML file (*.xml)", FileDialogProperty::OPEN_FILE),
      matrixOutport_(Port::OUTPORT, "Output Confusion Matrix CSV Textport", 0),
      modelOutport_(Port::OUTPORT, "Output Model Textport", 0),

      sampleMt_("sampleMt_", "Maximum training sample size per class", 1000, -1, 5000),
      sampleMv_("sampleMv_", "Maximum validation sample size per class", 1000, -1, 5000),
      sampleEdg_("sampleEdg_", "On edge pixel inclusion", false),
      sampleVtr_("sampleVtr_", "Training and validation sample ration", 0.5f, 0.0f, 1.0f),
      sampleVfn_("sampleVfn_", "Name of the discrimination field", ""),

      classifier_("classifier_", "Classifier to use for the training"),

      libsvmKernel_("libsvmKernel_", "SVM Kernel Type"),
      libsvmCost_("libsvmCost_", "Cost Parameter C", 1.0f, 0.0f, 10.0f),
      libsvmOpt_("libsvmOpt_", "Parameters optimization", false),

      svmType_("svmType_", "SVM Model Type"),
      svmKernel_("svmKernel_", "SVM Kernel Type"),
      svmCost_("svmCost_", "Cost Parameter C", 1.0f, 0.0f, 10.0f),
      svmNu_("svmNu_", "Parameter nu of a SVM optimization problem", 0.0f),
      svmCoef0_("svmCoef0_", "Parameter coef0 of a kernel function", 0.0f),
      svmGamma_("svmGamma_", "Parameter gamma of a kernel function", 1.0f),
      svmDegree_("svmDegree_", "Parameter degree of a kernel function", 1.0f),
      svmOpt_("svmOpt_", "Parameters optimization", false),

      boostType_("boostType_", "Boost Type"),
      boostWeak_("boostWeak_", "The number of weak classifiers", 100, 0, 1000),
      boostR_("boostR_", "Weight Trim Rate", 0.95f, 0.00f, 1.00f),
      boostMax_("boostMax_", "Maximum depth of the tree", 1, 0, 65535),

      dtMax_("dtMax_", "Maximum depth of the tree", 65535, 1, 65535),
      dtMin_("dtMin_", "Minimum number of samples in each node", 10, 1, 500),
      dtRa_("dtRa_", "Termination criteria for regression tree", 0.01f , 0.00f, 1.00f),
      dtCat_("dtCat_", "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split", 10, 1, 100),
      dtF_("dtF_", "K-fold cross-validations", 10, 1, 100),
      dtR_("dtR_", "Set Use1seRule flag to false", false),
      dtT_("dtT_", "Set TruncatePrunedTree flag to false", false),

      gbtW_("gbtW_", "Minimum number of samples in each node", 10, 1, 500),
      gbtS_("gbtS_", "Termination criteria for regression tree", 0.01f , 0.00f, 1.00f),
      gbtP_("gbtP_", "Termination criteria for regression tree", 0.01f , 0.00f, 1.00f),
      gbtMax_("gbtMax_", "Minimum number of samples in each node", 10, 1, 500),

      annType_("annType_", "Train Method Type"),
      annSizes_("annSizes_", "Number of neurons in each intermediate layer", ""),
      annFunction_("annFunction_", "Neuron activation function type"),
      annAlpha_("annAlpha_", "Alpha parameter of the activation function", 1.00f , 0.00f, 1.00f),
      annBeta_("annBeta_", "Beta parameter of the activation function", 1.00f , 0.00f, 1.00f),
      annBpdw_("annBpdw_", "Strength of the weight gradient term in the BACKPROP method", 0.1f , 0.00f, 1.00f),
      annBpms_("annBpms_", "Strength of the momentum term", 0.1f , 0.00f, 1.00f),
      annRdw_("annRdw_", "Initial value Delta_0 of update-values Delta_{ij} in RPROP method", 0.1f , 0.00f, 1.00f),
      annRdwm_("annRdwm_", "Update-values lower limit Delta_{min} in RPROP method", 1e-7f , 0.00f, 1e-7f),
      annTerm_("annTerm_", "Termination criteria"),
      annEps_("annEps_", "Epsilon value used in the Termination criteria", 0.01f , 0.00f, 1.00f),
      annIter_("annIter_", "Maximum number of iterations used in the Termination criteria", 1000, 1, 1000),

      rfMax_("rfMax_", "Maximum depth of the tree", 5, 1, 100),
      rfMin_("rfMin_", "Minimum number of samples in each node", 10, 1, 100),
      rfRa_("rfRa_", "Termination Criteria for regression tree", 0.00f , 0.00f, 1.00f),
      rfCat_("rfCat_", "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split", 10, 1, 100),
      rfVar_("rfVar_", "Size of the randomly selected subset of features at each tree node", 0, 0, 500),
      rfNbTrees_("rfNbTrees_", "Maximum number of trees in the forest", 100, 1, 500),
      rfAcc_("rfAcc_", "Sufficient accuracy (OOB error)", 0.01f , 0.00f, 1.00f),

      knnNeigbors_("knnNeigbors_", "Minimum number of samples in each node", 32, 1, 500)
    //      RAND_("RAND_", "Set user defined seed", 10, 1, 500)

{
    // IO
    addPort(inPort01_);
    addPort(inPort02_);
    addPort(inPort03_);
    addPort(inDataPort01_);
    addPort(inDataPort02_);
    addPort(inDataPort03_);
    addPort(matrixOutport_);
    addPort(modelOutport_);

    addProperty(enableXMLinput_);
    addProperty(XMLfilename_);

    enableXMLinput_.onChange(CallMemberAction<OTBTrainImagesClassifierApplication>
                         (this,&OTBTrainImagesClassifierApplication::updateClassifierSelection));


    // elev
    ///< to be implemented

    // sample
    addProperty(sampleMt_);
    addProperty(sampleMv_);
    addProperty(sampleEdg_);
    addProperty(sampleVtr_);
    addProperty(sampleVfn_);

    addProperty(classifier_);
    classifier_.addOption("blank", "Select classifier:");
    classifier_.addOption("libsvm", "LibSVM");
#ifdef OTB_USE_OPENCV
    classifier_.addOption("svm", "SVM (OpenCV)");
    classifier_.addOption("boost", "Boost");
    classifier_.addOption("dt", "Decision Tree");
    classifier_.addOption("gbt", "Gradient Boosted Tree");
    classifier_.addOption("ann", "Artificial Neural Network");
    classifier_.addOption("bayes", "Normal Bayes");
    classifier_.addOption("rf", "Random Forests");
    classifier_.addOption("knn", "KNN");
#endif

    classifier_.onChange(CallMemberAction<OTBTrainImagesClassifierApplication>
                         (this,&OTBTrainImagesClassifierApplication::updateClassifierSelection));

    // LibSVM
    addProperty(libsvmKernel_);
    libsvmKernel_.addOption("blank", "Select kernel:");
    libsvmKernel_.addOption("linear", "Linear");
    libsvmKernel_.addOption("gaussian", "Gaussian radial basis function");
    libsvmKernel_.addOption("polynomial", "Polynomial");
    libsvmKernel_.addOption("sigmoid", "Sigmoid");

    addProperty(libsvmCost_);
    addProperty(libsvmOpt_);

    // SVM
    addProperty(svmType_);
    svmType_.addOption("blank", "Select type:");
    svmType_.addOption("cSupport", "C support vector classification");
    svmType_.addOption("nuSupport", "Nu support vector classification");
    svmType_.addOption("distrEstimation", "Distribution estimation (One Class SVM)");

    addProperty(svmKernel_);
    svmKernel_.addOption("blank", "Select kernel:");
    svmKernel_.addOption("linear", "Linear");
    svmKernel_.addOption("gaussian", "Gaussian radial basis function");
    svmKernel_.addOption("polynomial", "Polynomial");
    svmKernel_.addOption("sigmoid", "Sigmoid");

    addProperty(svmCost_);
    addProperty(svmNu_);
    addProperty(svmCoef0_);
    addProperty(svmGamma_);
    addProperty(svmDegree_);
    addProperty(svmOpt_);

    // Boost
    addProperty(boostType_);
    boostType_.addOption("blank", "Select type:");
    boostType_.addOption("discrete", "Discrete AdaBoost");
    boostType_.addOption("real", "Real AdaBoost");
    boostType_.addOption("logit", "LogitBoost");
    boostType_.addOption("gentle", "Gentle AdaBoost");

    addProperty(boostWeak_);
    addProperty(boostR_);
    addProperty(boostMax_);

    // Decision Tree
    addProperty(dtMax_);
    addProperty(dtMin_);
    addProperty(dtRa_);
    addProperty(dtCat_);
    addProperty(dtF_);
    addProperty(dtR_);
    addProperty(dtT_);

    //Gradient Boosted Tree
    addProperty(gbtW_);
    addProperty(gbtS_);
    addProperty(gbtP_);
    addProperty(gbtMax_);

    //Artificial Neural Network
    addProperty(annType_);
    annType_.addOption("blank", "Select type:");
    annType_.addOption("reg", "RPROP algorithm");
    annType_.addOption("back", "Back-propagation algorithm");

    addProperty(annSizes_);
    addProperty(annFunction_);
    annFunction_.addOption("blank", "Select function:");
    annFunction_.addOption("ident", "Identity function");
    annFunction_.addOption("sig", "Symmetrical Sigmoid function");
    annFunction_.addOption("gau", "Gaussian function (Not completely supported)");

    addProperty(annAlpha_);
    addProperty(annBeta_);
    addProperty(annBpdw_);
    addProperty(annBpms_);
    addProperty(annRdw_);
    addProperty(annRdwm_);
    addProperty(annTerm_);
    annTerm_.addOption("blank", "Select criteria:");
    annTerm_.addOption("iter", "Maximum number of iterations");
    annTerm_.addOption("eps", "Epsilon");
    annTerm_.addOption("all", "Max. iterations + Epsilon");

    addProperty(annEps_);
    addProperty(annIter_);

    // Random Forests
    addProperty(rfMax_);
    addProperty(rfMin_);
    addProperty(rfRa_);
    addProperty(rfCat_);
    addProperty(rfVar_);
    addProperty(rfNbTrees_);
    addProperty(rfAcc_);

    // KNN
    addProperty(knnNeigbors_);

    XMLfilename_.setVisible(false);

    libsvmKernel_.setVisible(false);
    libsvmCost_.setVisible(false);
    libsvmOpt_.setVisible(false);

    svmType_.setVisible(false);
    svmKernel_.setVisible(false);
    svmCost_.setVisible(false);
    svmNu_.setVisible(false);
    svmCoef0_.setVisible(false);
    svmGamma_.setVisible(false);
    svmDegree_.setVisible(false);
    svmOpt_.setVisible(false);

    boostType_.setVisible(false);
    boostWeak_.setVisible(false);
    boostR_.setVisible(false);
    boostMax_.setVisible(false);

    dtMax_.setVisible(false);
    dtMin_.setVisible(false);
    dtRa_.setVisible(false);
    dtCat_.setVisible(false);
    dtF_.setVisible(false);
    dtR_.setVisible(false);
    dtT_.setVisible(false);

    gbtW_.setVisible(false);
    gbtS_.setVisible(false);
    gbtP_.setVisible(false);
    gbtMax_.setVisible(false);

    annType_.setVisible(false);
    annSizes_.setVisible(false);
    annFunction_.setVisible(false);
    annAlpha_.setVisible(false);
    annBeta_.setVisible(false);
    annBpdw_.setVisible(false);
    annBpms_.setVisible(false);
    annRdw_.setVisible(false);
    annRdwm_.setVisible(false);
    annTerm_.setVisible(false);
    annEps_.setVisible(false);
    annIter_.setVisible(false);

    rfMax_.setVisible(false);
    rfMin_.setVisible(false);
    rfRa_.setVisible(false);
    rfCat_.setVisible(false);
    rfVar_.setVisible(false);
    rfNbTrees_.setVisible(false);
    rfAcc_.setVisible(false);

    knnNeigbors_.setVisible(false);
}


void OTBTrainImagesClassifierApplication::updateClassifierSelection() {

    if (enableXMLinput_.get()) {
        XMLfilename_.setVisible(true);
    } else {
        XMLfilename_.setVisible(false);
    }

    if (classifier_.get()=="blank" || classifier_.get()=="bayes") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="libsvm") {
        libsvmKernel_.setVisible(true);
        libsvmCost_.setVisible(true);
        libsvmOpt_.setVisible(true);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="svm") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(true);
        svmKernel_.setVisible(true);
        svmCost_.setVisible(true);
        svmNu_.setVisible(true);
        svmCoef0_.setVisible(true);
        svmGamma_.setVisible(true);
        svmDegree_.setVisible(true);
        svmOpt_.setVisible(true);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="boost") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(true);
        boostWeak_.setVisible(true);
        boostR_.setVisible(true);
        boostMax_.setVisible(true);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="dt") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(true);
        dtMin_.setVisible(true);
        dtRa_.setVisible(true);
        dtCat_.setVisible(true);
        dtF_.setVisible(true);
        dtR_.setVisible(true);
        dtT_.setVisible(true);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="gbt") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(true);
        gbtS_.setVisible(true);
        gbtP_.setVisible(true);
        gbtMax_.setVisible(true);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="ann") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(true);
        annSizes_.setVisible(true);
        annFunction_.setVisible(true);
        annAlpha_.setVisible(true);
        annBeta_.setVisible(true);
        annBpdw_.setVisible(true);
        annBpms_.setVisible(true);
        annRdw_.setVisible(true);
        annRdwm_.setVisible(true);
        annTerm_.setVisible(true);
        annEps_.setVisible(true);
        annIter_.setVisible(true);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(false);

    } else if (classifier_.get()=="rf") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(true);
        rfMin_.setVisible(true);
        rfRa_.setVisible(true);
        rfCat_.setVisible(true);
        rfVar_.setVisible(true);
        rfNbTrees_.setVisible(true);
        rfAcc_.setVisible(true);

        knnNeigbors_.setVisible(false);
    } else if (classifier_.get()=="knn") {
        libsvmKernel_.setVisible(false);
        libsvmCost_.setVisible(false);
        libsvmOpt_.setVisible(false);

        svmType_.setVisible(false);
        svmKernel_.setVisible(false);
        svmCost_.setVisible(false);
        svmNu_.setVisible(false);
        svmCoef0_.setVisible(false);
        svmGamma_.setVisible(false);
        svmDegree_.setVisible(false);
        svmOpt_.setVisible(false);

        boostType_.setVisible(false);
        boostWeak_.setVisible(false);
        boostR_.setVisible(false);
        boostMax_.setVisible(false);

        dtMax_.setVisible(false);
        dtMin_.setVisible(false);
        dtRa_.setVisible(false);
        dtCat_.setVisible(false);
        dtF_.setVisible(false);
        dtR_.setVisible(false);
        dtT_.setVisible(false);

        gbtW_.setVisible(false);
        gbtS_.setVisible(false);
        gbtP_.setVisible(false);
        gbtMax_.setVisible(false);

        annType_.setVisible(false);
        annSizes_.setVisible(false);
        annFunction_.setVisible(false);
        annAlpha_.setVisible(false);
        annBeta_.setVisible(false);
        annBpdw_.setVisible(false);
        annBpms_.setVisible(false);
        annRdw_.setVisible(false);
        annRdwm_.setVisible(false);
        annTerm_.setVisible(false);
        annEps_.setVisible(false);
        annIter_.setVisible(false);

        rfMax_.setVisible(false);
        rfMin_.setVisible(false);
        rfRa_.setVisible(false);
        rfCat_.setVisible(false);
        rfVar_.setVisible(false);
        rfNbTrees_.setVisible(false);
        rfAcc_.setVisible(false);

        knnNeigbors_.setVisible(true);
    }

}


}   // namespace

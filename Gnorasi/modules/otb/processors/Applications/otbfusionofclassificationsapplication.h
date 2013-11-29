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

#ifndef OTBFUSIONOFCLASSIFICATIONSAPPLICATION_H
#define OTBFUSIONOFCLASSIFICATIONSAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbimageport.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "modules/otb/ports/otbcsvtextport.h"

#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/floatproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "itkLabelVotingImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

#include "otbDSFusionOfClassifiersImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbConfusionMatrixToMassOfBelief.h"
#include "itkCastImageFilter.h";

#include <iostream>
#include <fstream>

#include "otbImageList.h"


namespace voreen {

class OTBFusionOfClassificationsApplication : public OTBImageFilterProcessor
{
public:
    OTBFusionOfClassificationsApplication();

    virtual ~OTBFusionOfClassificationsApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Fusion of Classifications Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;
    /** Images typedefs */
    typedef ImageType               MaskImageType;
    typedef otb::Image<unsigned int, Dimension>       LabelImageType;
    typedef otb::VectorImage<double, Dimension> VectorImageType;

    typedef otb::MultiToMonoChannelExtractROI<VectorImageType::InternalPixelType, LabelImageType::PixelType> ConverterType;

    // Majority Voting
    typedef itk::LabelVotingImageFilter<LabelImageType, LabelImageType> LabelVotingFilterType;

    // Caster
    typedef itk::CastImageFilter<LabelImageType, ImageType> ImageCastFilterType;
    ImageCastFilterType::Pointer otbCaster;

    // Dempster Shafer
    typedef unsigned long                                                            ConfusionMatrixEltType;
    typedef itk::VariableSizeMatrix<ConfusionMatrixEltType>                          ConfusionMatrixType;
    typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType, LabelImageType::PixelType>  ConfusionMatrixToMassOfBeliefType;
    typedef ConfusionMatrixToMassOfBeliefType::MapOfClassesType                      MapOfClassesType;
    ConfusionMatrixToMassOfBeliefType::Pointer confusionMatrixToMassOfBeliefFilter;

    typedef otb::ImageList<VectorImageType>                                         VectorImageListType;

    typedef otb::ImageList<LabelImageType>                                         ImageListType;
    typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType>        ImageListToVectorImageFilterType;

    typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod          MassOfBeliefDefinitionMethod;

    typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, LabelImageType> DSFusionOfClassifiersImageFilterType;
    typedef DSFusionOfClassifiersImageFilterType::VectorOfMapOfMassesOfBeliefType    VectorOfMapOfMassesOfBeliefType;
    typedef DSFusionOfClassifiersImageFilterType::LabelMassMapType                   LabelMassMapType;
    DSFusionOfClassifiersImageFilterType::Pointer dsFusionOfClassifiersImageFilter;

protected:

    virtual void setDescriptions() {
        setDescription("Fuses several classifications maps (up to 3 so far) of the same image on the basis of class labels.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual bool isReady() const;
    virtual void updateMethodSelection(); ///< Properties visibility configuration.

    int CSVConfusionMatrixFileReader(const std::string fileName, MapOfClassesType &mapOfClassesRefClX, ConfusionMatrixType &confusionMatrixClX);

private:

    OTBVectorImagePort inPort01_;
    OTBVectorImagePort inPort02_;
    OTBVectorImagePort inPort03_;

    StringOptionProperty fusionMethod_; ///< Select fusion method

    OTBCSVTextPort csvPort01_;
    OTBCSVTextPort csvPort02_;
    OTBCSVTextPort csvPort03_;

    StringOptionProperty dempsterMOB; ///< Select mass of belief measurement

    IntProperty noDataLabel_;
    IntProperty undecidedLabel_;

    OTBImagePort outPort_;

    std::vector<itk::LightObject::Pointer> m_Filters;

    static const std::string loggerCat_; ///< Category used in logging
};


} // namespace

#endif // OTBFUSIONOFCLASSIFICATIONSAPPLICATION_H

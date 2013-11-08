/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#ifndef OTBIMAGECLASSIFIERAPPLICATION_H
#define OTBIMAGECLASSIFIERAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/optionproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"

#include "itkVariableLengthVector.h"
#include "otbChangeLabelImageFilter.h"
#include "otbStandardWriterWatcher.h"
#include "otbStatisticsXMLFileReader.h"
#include "otbShiftScaleVectorImageFilter.h"
#include "otbImageClassificationFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbMachineLearningModelFactory.h"

#include "otbVectorImageToMatrixImageFilter.h"

namespace voreen {

class OTBImageClassifierApplication : public OTBImageFilterProcessor
{
public:
    OTBImageClassifierApplication();

    virtual ~OTBImageClassifierApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Image Classifier Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> VectorImageType;

    typedef itk::VariableLengthVector<VectorImageType::InternalPixelType>                   MeasurementType;
    typedef otb::StatisticsXMLFileReader<MeasurementType>                                        StatisticsReader;
    StatisticsReader::Pointer  statisticsReader;

    typedef otb::ShiftScaleVectorImageFilter<VectorImageType, VectorImageType>         RescalerType;
    RescalerType::Pointer m_Rescaler;

    typedef otb::ImageClassificationFilter<VectorImageType, ImageType, ImageType> ClassificationFilterType;
    typedef ClassificationFilterType::Pointer                                                    ClassificationFilterPointerType;
    typedef ClassificationFilterType::ModelType                                                  ModelType;
    typedef ModelType::Pointer                                                                   ModelPointerType;
    typedef ClassificationFilterType::ValueType                                                  ValueType;
    typedef ClassificationFilterType::LabelType                                                  LabelType;
    typedef otb::MachineLearningModelFactory<ValueType, LabelType>                               MachineLearningModelFactoryType;

    ClassificationFilterType::Pointer m_ClassificationFilter;

protected:

    virtual void setDescriptions() {
        setDescription("Performs a classification of the input image according to a model file");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void updateReader();    ///< Enables/Disables XML reader functionality.

private:

    OTBVectorImagePort inPort_;
    OTBImagePort maskInPort_;
    OTBImagePort outPort_;

    BoolProperty enableMaskImage_;
    BoolProperty enableXMLReader_;
    FileDialogProperty XMLFilename_;  ///< Path of the XML file.
    FileDialogProperty ModelFilename_;  ///< Path of the XML file.

    ModelPointerType m_Model;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBIMAGECLASSIFIERAPPLICATION_H

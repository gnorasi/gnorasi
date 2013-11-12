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

#ifndef OTBCOMPUTEIMAGESSTATISTICSAPPLICATION_H
#define OTBCOMPUTEIMAGESSTATISTICSAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "otbStatisticsXMLFileWriter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "itkVariableLengthVector.h"

namespace voreen {

class OTBComputeImagesStatisticsApplication : public OTBImageFilterProcessor
{
public:
    OTBComputeImagesStatisticsApplication();

    virtual ~OTBComputeImagesStatisticsApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Compute Image Statistics Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> VectorImageType;

    typedef double ValueType;
    typedef itk::VariableLengthVector<ValueType> MeasurementType;

    typedef otb::StreamingStatisticsVectorImageFilter<VectorImageType> StreamingStatisticsVImageFilterType;
    StreamingStatisticsVImageFilterType::Pointer statsEstimator;

    typedef otb::StatisticsXMLFileWriter<MeasurementType> StatisticsWriter;
    StatisticsWriter::Pointer writer;

protected:

    virtual void setDescriptions() {
        setDescription("Computes global mean and standard deviation for each band from an image and optionally saves the results in an XML file");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void updateWriter();    ///< Enables/Disables XML writer functionality.
    void saveToXMLFile();   ///< Saves the output to XML file.

private:

    OTBVectorImagePort inPort_;

    ButtonProperty computeButton_; ///< Manually triggers the calculator.
    FileDialogProperty XMLFilename_;  ///< Path of the XML file.
    ButtonProperty saveXMLButton_;    ///< Saves the output to XML.
    BoolProperty enableXMLWriter_;    ///< Enables XML writer functionality.

    bool hasFile;

    MeasurementType mean;
    MeasurementType variance;
    MeasurementType stddev;


    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBCOMPUTEIMAGESSTATISTICSAPPLICATION_H

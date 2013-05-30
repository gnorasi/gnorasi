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

#ifndef OTBGREYLEVELCOOCCURRENCEMATRIXADVANCEDTEXTURECOEFFICIENTSCALCULATORPROCESSOR_H
#define OTBGREYLEVELCOOCCURRENCEMATRIXADVANCEDTEXTURECOEFFICIENTSCALCULATORPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "voreen/core/properties/floatproperty.h"
#include "itkHistogram.h"


namespace voreen {

class OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor : public OTBImageFilterProcessor
{
public:

    OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor();

    virtual ~OTBGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Grey Level Cooccurrence Matrix Advanced Texture Coefficients Calculator"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef double                  MeasurementType;
    typedef itk::Statistics::Histogram<MeasurementType, 2> HistogramType;

    typedef double                   InputPixelType;
    typedef otb::Image<PixelType, 2> InputImageType;

    typedef itk::Statistics::ScalarImageToHistogramGenerator<InputImageType> GeneratorType;
    //typedef HistogramGeneratorType::SizeType SizeType;

    typedef otb::GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<GeneratorType::HistogramType> FilterType;

    GeneratorType::Pointer generator;
    FilterType::Pointer filter;

    /**
    * Saves the output to text file.
    */
   void saveToFile();

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBImagePort inPort_;
    bool hasFile;
    ButtonProperty computeButton_; ///< Triggers the calculator.
    FileDialogProperty textFile_;  ///< Path of the saved text file.
    ButtonProperty saveTextButton_;    ///< Saves the output to file.

    double variance;
    double mean;
    double sumAverage;
    double sumVariance;
    double sumEntropy;
    double differenceEntropy;
    double differenceVariance;
    double ic1;
    double ic2;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBGREYLEVELCOOCCURRENCEMATRIXADVANCEDTEXTURECOEFFICIENTSCALCULATORPROCESSOR_H

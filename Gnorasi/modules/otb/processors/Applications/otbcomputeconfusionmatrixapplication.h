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

#ifndef OTBCOMPUTECONFUSIONMATRIXAPPLICATION_H
#define OTBCOMPUTECONFUSIONMATRIXAPPLICATION_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "modules/otb/ports/otbvectordataport.h"
#include "voreen/core/ports/textport.h"

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "otbOGRDataSourceToLabelImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "otbOGRDataSourceWrapper.h"

#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"

#include "itkVariableSizeMatrix.h"
#include "otbConfusionMatrixMeasurements.h"

namespace voreen {

class OTBComputeConfusionMatrixApplication : public OTBImageFilterProcessor
{
public:
    OTBComputeConfusionMatrixApplication();

    virtual ~OTBComputeConfusionMatrixApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Compute Confusion Matrix Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef itk::ImageRegionConstIterator<ImageType> ImageIteratorType;

    typedef otb::OGRDataSourceToLabelImageFilter<ImageType> RasterizeFilterType;
    RasterizeFilterType::Pointer rasterizeReference;

    typedef otb::StreamingTraits<ImageType> StreamingTraitsType;

    typedef itk::ImageRegionSplitter<2> SplitterType;
    SplitterType::Pointer splitter;

    typedef ImageType::RegionType RegionType;

    typedef int                                              ClassLabelType;
    typedef unsigned long                                    ConfusionMatrixEltType;
    typedef itk::VariableSizeMatrix<ConfusionMatrixEltType>  ConfusionMatrixType;

    typedef std::map<
                     ClassLabelType,
                     std::map<ClassLabelType, ConfusionMatrixEltType>
                    > OutputConfusionMatrixType;

    // filter type
    typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurementsType;
    ConfusionMatrixMeasurementsType::Pointer confMatMeasurements;

    typedef ConfusionMatrixMeasurementsType::MapOfClassesType                     MapOfClassesType;
    typedef ConfusionMatrixMeasurementsType::MeasurementType                      MeasurementType;

protected:

    virtual void setDescriptions() {
        setDescription("Computes the confusion matrix of a classification");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual bool isReady() const;
    virtual void updateFormat();    ///< Properties visibility configuration.

    std::string LogConfusionMatrix(MapOfClassesType* mapOfClasses, ConfusionMatrixType* matrix);

private:

    OTBImagePort inPort_;
    OTBImagePort referenceLabelPort_;
    OTBVectorDataPort vectorDataPort_;
    TextPort csvPort_;

    StringOptionProperty groundTruthFormat_; ///< Select ground truth format (raster/vector)
    StringProperty vectorField_;
    IntProperty noDataLabel_;
    IntProperty ram_;   ///< Available memory for processing (in MB) required for local streaming calculations.

    std::stringstream outFile;
    ConfusionMatrixType m_MatrixLOG;
    OutputConfusionMatrixType m_Matrix;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBCOMPUTECONFUSIONMATRIXAPPLICATION_H

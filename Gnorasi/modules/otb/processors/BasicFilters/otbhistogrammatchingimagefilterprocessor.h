/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#ifndef OTBHISTOGRAMMATCHINGIMAGEFILTERPROCESSOR_H
#define OTBHISTOGRAMMATCHINGIMAGEFILTERPROCESSOR_H

#include "otbimagefilterprocessor.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkHistogram.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"

namespace voreen {
class OTBHistogramMatchingImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBHistogramMatchingImageFilterProcessor();

    virtual ~OTBHistogramMatchingImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Histogram Matching Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::Image<double, 2> ScalarImageType;
    typedef ScalarImageType::PixelType THistogramMeasurement;

    typedef itk::HistogramMatchingImageFilter<ScalarImageType, ScalarImageType, THistogramMeasurement> FilterType;
    FilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBImagePort sourcePort_;
    OTBImagePort referencePort_;
    OTBImagePort outPort_;

    IntProperty numberOfHistogramLevels_;
    IntProperty numberOfMatchPoints_;
    BoolProperty thresholdAtMeanIntensity_;

    static const std::string loggerCat_; ///< category used in logging
};

} //namespace

#endif // OTBHISTOGRAMMATCHINGIMAGEFILTERPROCESSOR_H

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

#ifndef OTBCLOUDDETECTIONIMAGEFILTERPROCESSOR_H
#define OTBCLOUDDETECTIONIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "otbCloudDetectionFunctor.h"
#include "otbCloudDetectionFilter.h"
#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"

namespace voreen {

class OTBCloudDetectionImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBCloudDetectionImageFilterProcessor();

    virtual ~OTBCloudDetectionImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Cloud Detection Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;
    typedef double InputPixelType;
    typedef double OutputPixelType;
    typedef otb::VectorImage<InputPixelType, Dimension> VectorImageType;
    typedef VectorImageType::PixelType                  VectorPixelType;

    typedef otb::Functor::CloudDetectionFunctor<VectorPixelType, OTBImageFilterProcessor::ImageType::PixelType> FunctorType;

    typedef otb::CloudDetectionFilter<VectorImageType, OTBImageFilterProcessor::ImageType, FunctorType> CloudDetectionFilterType;
    CloudDetectionFilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void updateBands(int bands); ///< Updates property visibility according to spectral bands number.

private:

    FloatProperty gaussianVarianceValue_;
    FloatProperty minThreshold_;
    FloatProperty maxThreshold_;

    //this filter so far supports only images with 8 spectral bands max
    IntProperty refPixel0_;
    IntProperty refPixel1_;
    IntProperty refPixel2_;
    IntProperty refPixel3_;
    IntProperty refPixel4_;
    IntProperty refPixel5_;
    IntProperty refPixel6_;
    IntProperty refPixel7_;
    unsigned int nbBands;

    OTBVectorImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif  // OTBCLOUDDETECTIONIMAGEFILTERPROCESSOR_H

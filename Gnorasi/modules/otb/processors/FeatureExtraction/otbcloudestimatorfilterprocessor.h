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

#ifndef OTBCLOUDESTIMATORFILTERPROCESSOR_H
#define OTBCLOUDESTIMATORFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "otbCloudEstimatorFilter.h"


namespace voreen {

class OTBCloudEstimatorFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBCloudEstimatorFilterProcessor();

    virtual ~OTBCloudEstimatorFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Cloud Estimator Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;


    typedef otb::VectorImage<double, 2> VectorImage;
    typedef otb::Image<double, 2> InternalImageType;

    typedef VectorImage::PixelType VectorPixelType;

    typedef otb::CloudEstimatorFilter<VectorImage, InternalImageType> FilterType;
    FilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void updateBands(int bands);    //Updates the display of properties

private:

    OTBVectorImagePort inPort_;
    OTBImagePort outPort_;

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

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBCLOUDESTIMATORFILTERPROCESSOR_H

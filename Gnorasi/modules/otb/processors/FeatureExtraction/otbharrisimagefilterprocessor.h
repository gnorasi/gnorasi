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

#ifndef OTBHARRISIMAGEFILTERPROCESSOR_H
#define OTBHARRISIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "otbHarrisImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBHarrisImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBHarrisImageFilterProcessor();

    virtual ~OTBHarrisImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Harris Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                           Dimension = 2;
    typedef unsigned char                               CharPixelType;
    typedef itk::Image<CharPixelType,Dimension>         CharImageType;

    //FILTER

    typedef otb::HarrisImageFilter<ImageType, ImageType> DerivativeImageFilterType;
    DerivativeImageFilterType::Pointer filter;

    typedef itk::RescaleIntensityImageFilter<ImageType, CharImageType> RescaleImageFilterType;
    RescaleImageFilterType::Pointer rescaler;

    //CASTERS

    typedef itk::CastImageFilter<CharImageType, ImageType> charToPortCastingFilterType;
    charToPortCastingFilterType::Pointer charToPortCaster;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty sigmaD_;
    FloatProperty sigmaI_;
    FloatProperty alpha_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBHARRISIMAGEFILTERPROCESSOR_H

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

#ifndef OTBDERIVATIVEIMAGEFILTERPROCESSOR_H
#define OTBDERIVATIVEIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/intproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "itkCastImageFilter.h"
#include "itkDerivativeImageFilter.h"

namespace voreen {

class OTBDerivativeImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBDerivativeImageFilterProcessor();

    virtual ~OTBDerivativeImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Derivative Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                           Dimension = 2;
    typedef unsigned char                               CharPixelType;
    typedef itk::Image<CharPixelType,Dimension>         CharImageType;
    typedef float                                       InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension>    InternalImageType;

    //FILTER

    typedef itk::DerivativeImageFilter<CharImageType, InternalImageType> DerivativeImageFilterType;
    DerivativeImageFilterType::Pointer filter;

    //CASTERS

    typedef itk::CastImageFilter<ImageType, CharImageType> PortToCharCastingFilterType;
    PortToCharCastingFilterType::Pointer portToCharCaster;

    typedef itk::CastImageFilter<InternalImageType, ImageType> InternToPortCastingFilterType;
    InternToPortCastingFilterType::Pointer internToPortCaster;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    IntProperty order_;
    IntProperty direction_;
    BoolProperty useImageSpacing_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBDERIVATIVEIMAGEFILTERPROCESSOR_H

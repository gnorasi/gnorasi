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

#ifndef OTBBINARYTHINNINGIMAGEFILTERPROCESSOR_H
#define OTBBINARYTHINNINGIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/intproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "itkCastImageFilter.h"
#include "itkBinaryThinningImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

namespace voreen {

class OTBBinaryThinningImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBBinaryThinningImageFilterProcessor();

    virtual ~OTBBinaryThinningImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Enhancement"; }
    virtual std::string getClassName() const { return "Binary Thinning Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                           Dimension = 2;
    typedef unsigned char                               CharPixelType;
    typedef itk::Image<CharPixelType,Dimension>         CharImageType;

    //PIPELINE FILTERS

    typedef itk::BinaryThinningImageFilter<CharImageType, CharImageType> BinaryThinningImageFilterType;
    BinaryThinningImageFilterType::Pointer filter;

    typedef itk::RescaleIntensityImageFilter<CharImageType, ImageType> RescaleIntensityImageFilterType;
    RescaleIntensityImageFilterType::Pointer rescaler;

    //CASTERS

    typedef itk::CastImageFilter<ImageType, CharImageType> PortToCharCastingFilterType;
    PortToCharCastingFilterType::Pointer portToCharCaster;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBBINARYTHINNINGIMAGEFILTERPROCESSOR_H

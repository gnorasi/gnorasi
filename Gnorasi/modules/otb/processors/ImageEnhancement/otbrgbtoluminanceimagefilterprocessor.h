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

#ifndef OTBRGBTOLUMINANCEIMAGEFILTERPROCESSOR_H
#define OTBRGBTOLUMINANCEIMAGEFILTERPROCESSOR_H

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "otbVectorImage.h"
#include "itkRGBPixel.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBRGBToLuminanceImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBRGBToLuminanceImageFilterProcessor();

    virtual ~OTBRGBToLuminanceImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Enchancement"; }
    virtual std::string getClassName() const { return "RGB To Luminance Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> VectorImageType;
    typedef itk::Image<itk::RGBPixel<unsigned char>, 2> RGBImageType;
    typedef otb::Image<unsigned char, 2> ScalarImageType;

    typedef itk::RGBToLuminanceImageFilter<RGBImageType, ScalarImageType> FilterType;
    FilterType::Pointer filter;

    //CASTERS

    typedef itk::VectorCastImageFilter<VectorImageType, RGBImageType> RGBCasterType;
    RGBCasterType::Pointer rgbCaster;

    typedef itk::CastImageFilter<ScalarImageType, ImageType> ScalarCasterType;
    ScalarCasterType::Pointer scalarCaster;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

//    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

private:

    OTBVectorImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBRGBTOLUMINANCEIMAGEFILTERPROCESSOR_H

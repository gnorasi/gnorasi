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

#ifndef OTBLABELMAPTOBINARYIMAGEFILTERPROCESSOR_H
#define OTBLABELMAPTOBINARYIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "otbAttributesMapOpeningLabelMapFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "../modules/otb/ports/otblabelmapport.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBLabelMapToBinaryImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBLabelMapToBinaryImageFilterProcessor();

    virtual ~OTBLabelMapToBinaryImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Label Map To Binary Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;
    typedef unsigned long LabelType;
    typedef unsigned char MaskPixelType;
    typedef otb::Image<MaskPixelType, Dimension> MaskImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;

    typedef itk::LabelMapToBinaryImageFilter<LabelMapType, MaskImageType> LabelMapToBinaryImageFilter;
    LabelMapToBinaryImageFilter::Pointer filter;

    typedef itk::CastImageFilter<MaskImageType, ImageType> MaskCasterType;
    MaskCasterType::Pointer caster;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    StringProperty attribute_;
    BoolProperty lowerThan_;
    FloatProperty thresh_;

    OTBLabelMapPort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBLABELMAPTOBINARYIMAGEFILTERPROCESSOR_H

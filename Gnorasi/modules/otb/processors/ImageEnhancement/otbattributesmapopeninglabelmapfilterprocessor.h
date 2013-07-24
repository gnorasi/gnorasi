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

#ifndef OTBATTRIBUTESMAPOPENINGLABELMAPFILTERPROCESSOR_H
#define OTBATTRIBUTESMAPOPENINGLABELMAPFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "otbAttributesMapOpeningLabelMapFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "../modules/otb/ports/otblabelmapport.h"

namespace voreen {

class OTBAttributesMapOpeningLabelMapFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBAttributesMapOpeningLabelMapFilterProcessor();

    virtual ~OTBAttributesMapOpeningLabelMapFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Enhancement"; }
    virtual std::string getClassName() const { return "Attributes Map Opening Label Map Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;
    typedef unsigned long LabelType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;

    typedef otb::AttributesMapOpeningLabelMapFilter<LabelMapType> OpeningLabelMapFilterType;
    OpeningLabelMapFilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void bypass(OTBLabelMapPort *inport, OTBLabelMapPort *outport); ///< Passes the image from inport to outport without changes.

private:

    StringProperty attribute_;
    BoolProperty lowerThan_;
    FloatProperty thresh_;

    OTBLabelMapPort inPort_;
    OTBLabelMapPort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBATTRIBUTESMAPOPENINGLABELMAPFILTERPROCESSOR_H

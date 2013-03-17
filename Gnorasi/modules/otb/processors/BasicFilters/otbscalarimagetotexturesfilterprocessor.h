/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
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

#ifndef OTBSCALARIMAGETOTEXTURESFILTERPROCESSOR_H
#define OTBSCALARIMAGETOTEXTURESFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbimagefilterprocessor.h"
#include "otbScalarImageToTexturesFilter.h"

namespace voreen {

class OTBScalarImageToTexturesFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBScalarImageToTexturesFilterProcessor();

    virtual ~OTBScalarImageToTexturesFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Haralick Texture Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::ScalarImageToTexturesFilter<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;

    typedef FilterType::OffsetType OffsetType;

    FilterType::Pointer filter;

protected:
    virtual void setDescriptions()
    {
        setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    //The radius parameter can be passed to the filter as a scalar parameter
    //if the neighborhood is square, or as SizeType in any case
    //For the moment a scalar square value is assumed
    IntProperty radius_;

    //The offset is always an array of N values,
    //where N is the number of dimensions of the image.
    //CHECK whether to make this offset automatically.
    IntProperty offSetX_;
    IntProperty offSetY_;



    OTBImagePort inPort_;
    OTBImagePort outPort_;


    static const std::string loggerCat_; ///< category used in logging



};


} //namespace

#endif // OTBSCALARIMAGETOTEXTURESFILTERPROCESSOR_H

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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
 
#ifndef OTBGRADIENTMAGNITUDEIMAGEFILTERPROCESSOR_H
#define OTBGRADIENTMAGNITUDEIMAGEFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbimagefilterprocessor.h"
#include "itkGradientMagnitudeImageFilter.h"

namespace voreen {

class OTBGradientMagnitudeImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBGradientMagnitudeImageFilterProcessor();
    virtual ~OTBGradientMagnitudeImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Gradient Magnitude"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef itk::GradientMagnitudeImageFilter<OTBGradientMagnitudeImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;


    FilterType::Pointer filter;


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

} //namespace

#endif // OTBGRADIENTMAGNITUDEIMAGEFILTERPROCESSOR_H

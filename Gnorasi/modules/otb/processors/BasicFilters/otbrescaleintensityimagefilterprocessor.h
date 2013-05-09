/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#ifndef OTBRESCALEINTENSITYIMAGEFILTERPROCESSOR_H
#define OTBRESCALEINTENSITYIMAGEFILTERPROCESSOR_H

#include "otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "itkRescaleIntensityImageFilter.h"

namespace voreen {

class OTBRescaleIntensityImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBRescaleIntensityImageFilterProcessor();

    virtual ~OTBRescaleIntensityImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Rescale Intensity Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef itk::RescaleIntensityImageFilter<ImageType, ImageType> FilterType;
    FilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty OutputMin_;
    FloatProperty OutputMax_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif  // OTBRESCALEINTENSITYIMAGEFILTERPROCESSOR_H

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

#ifndef OTBGRAYSCALECONNECTEDCLOSINGIMAGEFILTERPROCESSOR_H
#define OTBGRAYSCALECONNECTEDCLOSINGIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "itkGrayscaleConnectedClosingImageFilter.h"
#include "voreen/core/properties/floatproperty.h"

namespace voreen {

class OTBGrayscaleConnectedClosingImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBGrayscaleConnectedClosingImageFilterProcessor();

    virtual ~OTBGrayscaleConnectedClosingImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Enhancement"; }
    virtual std::string getClassName() const { return "Grayscale Connected Closing Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef itk::GrayscaleConnectedClosingImageFilter<ImageType, ImageType> GrayscaleConnectedClosingType;
    GrayscaleConnectedClosingType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    //FloatProperty seed_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBGRAYSCALECONNECTEDCLOSINGIMAGEFILTERPROCESSOR_H

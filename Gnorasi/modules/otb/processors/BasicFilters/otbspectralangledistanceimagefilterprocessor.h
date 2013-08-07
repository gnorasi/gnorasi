/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#ifndef OTBSPECTRALANGLEDISTANCEIMAGEFILTERPROCESSOR_H
#define OTBSPECTRALANGLEDISTANCEIMAGEFILTERPROCESSOR_H

#include "otbimagefilterprocessor.h"
#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "otbSpectralAngleDistanceImageFilter.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"

namespace voreen {

class OTBSpectralAngleDistanceImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBSpectralAngleDistanceImageFilterProcessor();

    virtual ~OTBSpectralAngleDistanceImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Spectral Angle Distance Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> MultiSpectralImageType;
    typedef otb::Image<double, 2> InternalImageType;

    typedef MultiSpectralImageType::PixelType MultiSpectralPixelType;

    typedef otb::SpectralAngleDistanceImageFilter<MultiSpectralImageType, InternalImageType> FilterType;
    FilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void updateBands(int bands);    //Updates the display of properties

private:

    OTBVectorImagePort inPort_;
    OTBImagePort outPort_;

    //this filter so far supports only images with 8 spectral bands max
    IntProperty refPixel0_;
    IntProperty refPixel1_;
    IntProperty refPixel2_;
    IntProperty refPixel3_;
    IntProperty refPixel4_;
    IntProperty refPixel5_;
    IntProperty refPixel6_;
    IntProperty refPixel7_;
    unsigned int nbBands;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBSPECTRALANGLEDISTANCEIMAGEFILTERPROCESSOR_H

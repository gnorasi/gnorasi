/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.             	*
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

#ifndef OTBGENERICROADEXTRACTIONFILTERPROCESSOR_H
#define OTBGENERICROADEXTRACTIONFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "otbGenericRoadExtractionFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbLikelihoodPathListFilter.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbDrawPathFilter.h"

namespace voreen {

class OTBGenericRoadExtractionFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBGenericRoadExtractionFilterProcessor();

    virtual ~OTBGenericRoadExtractionFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Generic Road Extraction Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;

    typedef otb::PolyLineParametricPathWithValue<PixelType, Dimension> PathType;

    typedef otb::GenericRoadExtractionFilter<ImageType, PathType> GenericRoadExtractionFilterType;
    GenericRoadExtractionFilterType::Pointer roadextractor;

    typedef otb::DrawPathListFilter<ImageType,PathType,ImageType> DrawPathFilterType;
    DrawPathFilterType::Pointer drawingFilter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    void update();

private:

    FloatProperty alpha_;
    FloatProperty amplitudeThreshold_;
    FloatProperty tolerance_;
    FloatProperty maxAngle_;
    FloatProperty firstMeanDistanceThreshold_;
    FloatProperty secondMeanDistanceThreshold_;
    FloatProperty distanceThreshold_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBGENERICROADEXTRACTIONFILTERPROCESSOR_H

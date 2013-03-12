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

#ifndef VRN_OTBROADEXTRACTIONPROCESSOR_H
#define VRN_OTBROADEXTRACTIONPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbRoadExtractionFilter.h"
#include "otbDrawPathListFilter.h"

namespace voreen {
  
class OTBRoadExtractionProcessor : public OTBImageFilterProcessor {
public:
    OTBRoadExtractionProcessor();
    virtual ~OTBRoadExtractionProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Feature Extranction"; }
    virtual std::string getClassName() const { return "Road Extraction"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    static const unsigned int                                           Dimension = 2;
    typedef double                                                      InputPixelType;
    typedef double                                                      OutputPixelType;
    typedef otb::VectorImage<InputPixelType, Dimension>                 VectorImageType;
    typedef otb::Image<InputPixelType,Dimension>                        ImageType;

    typedef otb::PolyLineParametricPathWithValue<InputPixelType,
        Dimension> PathType;

    typedef otb::RoadExtractionFilter<VectorImageType,PathType>         RoadExtractionFilterType;

    typedef otb::DrawPathListFilter<ImageType, PathType,
        ImageType>                                                      DrawPathFilterType;

    RoadExtractionFilterType::Pointer                                   roadextractor;
    DrawPathFilterType::Pointer                                         drawingFilter;
        

protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    void update();

private:

    OTBVectorImagePort  inPort_;
    OTBImagePort        outPort_;

    StringProperty      m_referencePixelProperty;
    FloatProperty       m_alphaProperty;
    FloatProperty       m_toleranceProperty;
    FloatProperty       m_amplitudeProperty;
    FloatProperty       m_resolutionProperty;
    FloatProperty       m_fistMeanDistanceThresholdProperty;
    FloatProperty       m_sendondMeanDistanceThresholdProperty;
    FloatProperty       m_angularThresholdProperty;
    FloatProperty       m_distanceThresholdProperty;
    FloatProperty       m_maxAngleProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBROADEXTRACTIONPROCESSOR_H

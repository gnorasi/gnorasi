/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.				*
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

#ifndef VRN_OTBLAPLACIANSEGMENTATIONLEVELSETIMAGEFILTERPROCESSOR_H
#define VRN_OTBLAPLACIANSEGMENTATIONLEVELSETIMAGEFILTERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkLaplacianSegmentationLevelSetImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

namespace voreen {
  
class OTBLaplacianSegmentationLevelSetImageFilterProcessor : public Processor {
public:
    OTBLaplacianSegmentationLevelSetImageFilterProcessor();
    virtual ~OTBLaplacianSegmentationLevelSetImageFilterProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Segmentation"; }
    virtual std::string getClassName() const { return "Laplacian Segmentation Level Set Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    
    typedef float PixelType;

    typedef itk::LaplacianSegmentationLevelSetImageFilter<OTBImagePort::ImageType,OTBImagePort::ImageType, double>  LaplacianSegmentationLevelSetImageFilter;

    typedef itk::GradientAnisotropicDiffusionImageFilter<OTBImagePort::ImageType, OTBImagePort::ImageType> SmoothingFilterType;
    
    LaplacianSegmentationLevelSetImageFilter::Pointer filter;

    SmoothingFilterType::Pointer smoothing;
        

protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();

    void updateIsoSurfaceProperty();
    void updateMaxRMSErrorProperty();
    void updateMaxIterationsProperty();
    void updateFeatureScalingProperty();
    void updateCurvatureScalingProperty();
    void updatePropagationScalingProperty();
    void updateAdvecitonScalingProperty();
    void updateUseSmoothingProperty();
    void updateSmoothingTimeStepProperty();
    void updateSmoothingNumberOfIterationsProperty();
    void updateSmoothingConductanceParameterProperty();
    void updateUseFeatureScalingProoperty();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBImagePort inPort1_;
    OTBImagePort inPort2_;
    OTBImagePort outPort_;

    FloatProperty   m_isoSurfaceProperty;
    FloatProperty   m_maxRMSErrorProperty;
    IntProperty     m_maxIterationsProperty;
    FloatProperty   m_featureScalingProperty;
    FloatProperty   m_curvatureScalingProperty;
    FloatProperty   m_propagationScalingProperty;
    FloatProperty   m_advectionScalingProperty;
    BoolProperty    m_useSmoothingFilterProperty;
    BoolProperty    m_useFeatureScalingProperty;

    FloatProperty   m_smoothingTimeStepProperty;
    IntProperty     m_smoothingnumberOfIterationsProperty;
    FloatProperty   m_smoothingconductanceParameterProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBLAPLACIANSEGMENTATIONLEVELSETIMAGEFILTERPROCESSOR_H

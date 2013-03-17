/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
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

#include "otblaplaciansegmentationlevelsetimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBLaplacianSegmentationLevelSetImageFilterProcessor::loggerCat_("voreen.OTBLaplacianSegmentationLevelSetImageFilterProcessor");

OTBLaplacianSegmentationLevelSetImageFilterProcessor::OTBLaplacianSegmentationLevelSetImageFilterProcessor()
    : Processor(),
    inPort1_(Port::INPORT, "Image Input", 0),
    inPort2_(Port::INPORT, "Feature Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    m_isoSurfaceProperty("isoSurface", "Iso Surface", 127.5f, 0.0f, 255.0f),
    m_maxRMSErrorProperty("maxRMS", "Max RMS", 0.002f),
    m_maxIterationsProperty("numberIterations", "Number of Iterations", 15, 0, 10000),
    m_useFeatureScalingProperty("useFeatureScaling", "Use Feature Scaling",true),
    m_featureScalingProperty("featureScaling","Feature Scaling", 1.0f),
    m_curvatureScalingProperty("curvatureScaling","Curvature Scaling",1.0f),
    m_propagationScalingProperty("propagationScaling","Propagation Scaling",1.0f),
    m_advectionScalingProperty("advectionScaling", "Advection Scaling", 0.0f),
    m_useSmoothingFilterProperty("useSmoothing", "Use Smoothing Filter Property"),
    m_smoothingTimeStepProperty("smoothingTimeStep", "Smoothing Time Step",0.125f),
    m_smoothingnumberOfIterationsProperty("smoothingNumberIterations", "Number of Smoothing Iterations",10),
    m_smoothingconductanceParameterProperty("smoothingConductance", "Smoothing Conductance",2.0f, 0.0f, 100.0f)
{
    addPort(inPort1_);
    addPort(inPort2_);
    addPort(outPort_);

    addProperty(m_isoSurfaceProperty);
    addProperty(m_maxRMSErrorProperty);
    addProperty(m_maxIterationsProperty);
    addProperty(m_useFeatureScalingProperty);
    addProperty(m_featureScalingProperty);
    addProperty(m_curvatureScalingProperty);
    addProperty(m_propagationScalingProperty);
    addProperty(m_advectionScalingProperty);
    addProperty(m_useSmoothingFilterProperty);
    addProperty(m_smoothingTimeStepProperty);
    addProperty(m_smoothingnumberOfIterationsProperty);
    addProperty(m_smoothingconductanceParameterProperty);

    m_smoothingconductanceParameterProperty.setVisible(false);
    m_smoothingnumberOfIterationsProperty.setVisible(false);
    m_smoothingTimeStepProperty.setVisible(false);
    m_advectionScalingProperty.setVisible(false);
    m_curvatureScalingProperty.setVisible(false);

    m_isoSurfaceProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateIsoSurfaceProperty));
    m_maxRMSErrorProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateMaxRMSErrorProperty));
    m_maxIterationsProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateMaxIterationsProperty));
    m_featureScalingProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateFeatureScalingProperty));
    m_propagationScalingProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updatePropagationScalingProperty));
    m_curvatureScalingProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateCurvatureScalingProperty));
    m_advectionScalingProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateAdvecitonScalingProperty));
    m_smoothingTimeStepProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateSmoothingTimeStepProperty));
    m_smoothingnumberOfIterationsProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateSmoothingNumberOfIterationsProperty));
    m_smoothingconductanceParameterProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateSmoothingConductanceParameterProperty));
    m_useSmoothingFilterProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateUseSmoothingProperty));
    m_useFeatureScalingProperty.onChange(CallMemberAction<OTBLaplacianSegmentationLevelSetImageFilterProcessor>(this,
                                            &OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateUseFeatureScalingProoperty));

    
    filter = LaplacianSegmentationLevelSetImageFilter::New();
    smoothing = SmoothingFilterType::New();
}

Processor* OTBLaplacianSegmentationLevelSetImageFilterProcessor::create() const {
  return new OTBLaplacianSegmentationLevelSetImageFilterProcessor();
}

OTBLaplacianSegmentationLevelSetImageFilterProcessor::~OTBLaplacianSegmentationLevelSetImageFilterProcessor() {

}

void OTBLaplacianSegmentationLevelSetImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBLaplacianSegmentationLevelSetImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBLaplacianSegmentationLevelSetImageFilterProcessor::getProcessorInfo() const {
    
    return "Segments structures in images based on a second derivative image features";
}

void OTBLaplacianSegmentationLevelSetImageFilterProcessor::process() {
  
    try
    {
        filter->SetInput(inPort1_.getData());

        filter->SetIsoSurfaceValue(m_isoSurfaceProperty.get());
        filter->SetMaximumRMSError(m_maxRMSErrorProperty.get());
        filter->SetNumberOfIterations(m_maxIterationsProperty.get());

        if(m_useFeatureScalingProperty.get()){
            filter->SetFeatureScaling(m_featureScalingProperty.get());
            filter->SetCurvatureScaling(m_curvatureScalingProperty.get());
        }else{
            filter->SetAdvectionScaling(m_advectionScalingProperty.get());
            filter->SetPropagationScaling(m_propagationScalingProperty.get());
        }


        if(m_useSmoothingFilterProperty.get()){
            smoothing->SetTimeStep(m_smoothingTimeStepProperty.get());
            smoothing->SetNumberOfIterations(m_smoothingnumberOfIterationsProperty.get());
            smoothing->SetConductanceParameter(m_smoothingconductanceParameterProperty.get());
            smoothing->SetInput(inPort2_.getData());
            filter->SetFeatureImage(smoothing->GetOutput());
        }else{
            filter->SetFeatureImage(inPort2_.getData());
        }

        filter->GetOutput()->UpdateOutputInformation();

        outPort_.setData((OTBImagePort::ImageType*)filter->GetOutput());
    }
    catch (int e)
    {
    LERROR("Problem with LAI from NDVI calculation!");
	return;
    }
    
}



void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateIsoSurfaceProperty(){
    double val = m_isoSurfaceProperty.get();

    filter->SetIsoSurfaceValue(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateMaxRMSErrorProperty(){
    double val = m_maxRMSErrorProperty.get();

    filter->SetMaximumRMSError(val);
}

void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateMaxIterationsProperty(){
    double val = m_maxIterationsProperty.get();

    filter->SetNumberOfIterations(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateFeatureScalingProperty(){
    double val = m_featureScalingProperty.get();

    filter->SetFeatureScaling(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateCurvatureScalingProperty(){
    double val = m_curvatureScalingProperty.get();

    filter->SetCurvatureScaling(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updatePropagationScalingProperty(){
    double val = m_propagationScalingProperty.get();

    filter->SetPropagationScaling(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateAdvecitonScalingProperty(){
    double val = m_propagationScalingProperty.get();

    filter->SetAdvectionScaling(val);
}

void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateSmoothingTimeStepProperty(){
    double val = m_smoothingTimeStepProperty.get();

    smoothing->SetTimeStep(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateSmoothingNumberOfIterationsProperty(){
    double val = m_smoothingnumberOfIterationsProperty.get();


    smoothing->SetNumberOfIterations(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateSmoothingConductanceParameterProperty(){
    double val = m_smoothingconductanceParameterProperty.get();

    smoothing->SetConductanceParameter(val);
}

void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateUseFeatureScalingProoperty(){
    double val = m_useFeatureScalingProperty.get();

    m_advectionScalingProperty.setVisible(!val);
    m_curvatureScalingProperty.setVisible(!val);
    m_featureScalingProperty.setVisible(val);
}


void OTBLaplacianSegmentationLevelSetImageFilterProcessor::updateUseSmoothingProperty(){
    bool val = m_useSmoothingFilterProperty.get();

    m_smoothingconductanceParameterProperty.setVisible(val);
    m_smoothingnumberOfIterationsProperty.setVisible(val);
    m_smoothingTimeStepProperty.setVisible(val);

    double timeStep = m_smoothingTimeStepProperty.get();
    int ni = m_smoothingnumberOfIterationsProperty.get();
    double cp = m_smoothingconductanceParameterProperty.get();

    smoothing->SetTimeStep(timeStep);
    smoothing->SetNumberOfIterations(ni);
    smoothing->SetConductanceParameter(cp);
}

} // namespace

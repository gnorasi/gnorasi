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

#ifndef OTBOPTICALCALIBRATIONAPPLICATION_H
#define OTBOPTICALCALIBRATIONAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "otbClampVectorImageFilter.h"
#include "otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter.h"
#include "otbGroundSpacingImageFunction.h"



namespace voreen {

class OTBOpticalCalibrationApplication : public OTBImageFilterProcessor
{
public:
    OTBOpticalCalibrationApplication();

    virtual ~OTBOpticalCalibrationApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Optical Calibration Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> VectorImageType;
    typedef otb::VectorImage<unsigned short> UInt16VectorImageType;

    typedef otb::ImageToLuminanceImageFilter<VectorImageType, VectorImageType> ImageToLuminanceImageFilterType;
    ImageToLuminanceImageFilterType::Pointer ImageToLuminanceFilter;

    typedef otb::LuminanceToReflectanceImageFilter<VectorImageType,VectorImageType> LuminanceToReflectanceImageFilterType;
    LuminanceToReflectanceImageFilterType::Pointer LuminanceToReflectanceFilter;

    typedef otb::MultiplyByScalarImageFilter<VectorImageType,VectorImageType> ScaleFilterType;
    ScaleFilterType::Pointer ScaleFilter;

    typedef otb::ClampVectorImageFilter<VectorImageType,VectorImageType> ClampFilterType;
    ClampFilterType::Pointer ClampFilter;

    typedef otb::ReflectanceToSurfaceReflectanceImageFilter<VectorImageType,VectorImageType> ReflectanceToSurfaceReflectanceImageFilterType;
    ReflectanceToSurfaceReflectanceImageFilterType::Pointer ReflectanceToSurfaceReflectanceFilter;


    typedef ReflectanceToSurfaceReflectanceImageFilterType::FilterFunctionValuesType FilterFunctionValuesType;
    typedef FilterFunctionValuesType::ValuesVectorType ValuesVectorType;
    typedef otb::AtmosphericCorrectionParameters AtmosphericCorrectionParametersType;
    typedef AtmosphericCorrectionParametersType::AerosolModelType AerosolModelType;

    typedef otb::SurfaceAdjacencyEffect6SCorrectionSchemeFilter<VectorImageType,VectorImageType> SurfaceAdjacencyEffect6SCorrectionSchemeFilterType;
    SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::Pointer SurfaceAdjacencyEffectFilter;

    typedef otb::GroundSpacingImageFunction<VectorImageType> GroundSpacingImageType;
    GroundSpacingImageType::Pointer GroundSpacing;

    typedef UInt16VectorImageType::IndexType   IndexType;
    typedef GroundSpacingImageType::FloatType  FloatType;
    typedef GroundSpacingImageType::ValueType  ValueType;
    typedef IndexType::IndexValueType IndexValueType;

protected:

    virtual void setDescriptions() {
    setDescription("Perform optical calibration TOA/TOC (Top Of Atmosphere/Top Of Canopy). Supported sensors: QuickBird, Ikonos, WorldView2, Formosat, Spot5, Pleiades");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

private:

    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;

    BoolProperty centerData_;
    IntProperty numberOfPrincipalComponentsReq_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBOPTICALCALIBRATIONAPPLICATION_H

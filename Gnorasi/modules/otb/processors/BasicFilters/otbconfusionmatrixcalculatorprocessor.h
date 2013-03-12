/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.				**                                                                              *
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

#ifndef VRN_OTBCONFUSIONMATRIXCALCULATORPROCESSOR_H
#define VRN_OTBCONFUSIONMATRIXCALCULATORPROCESSOR_H



#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"

#include "../../ports/otbvectorimageport.h"
#include "../../ports/otbvectordataport.h"

#include "otbVectorData.h"
#include "otbListSampleGenerator.h"

#include "otbConfusionMatrixCalculator.h"



namespace voreen {
  
class OTBConfusionMatrixCalculatorProcessor : public OTBImageFilterProcessor {
public:
    OTBConfusionMatrixCalculatorProcessor();
    virtual ~OTBConfusionMatrixCalculatorProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Confusion Matrix"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    static const unsigned int                                           Dimension = 2;
    typedef double                                                      PixelType;
    typedef double LabeledPixelType;
    typedef otb::VectorImage<PixelType,Dimension>                       VImageType;
    typedef otb::Image<LabeledPixelType,Dimension>                      LabeledImageType;
    typedef otb::VectorData<double, 2>                                  VectorDataType;

    // SampleList manipulation
    typedef otb::ListSampleGenerator<VImageType, VectorDataType>        ListSampleGeneratorType;
    typedef ListSampleGeneratorType::ListLabelType                      LabelListSampleType;

    typedef otb::ConfusionMatrixCalculator<LabelListSampleType,
    LabelListSampleType>                                                ConfusionFilterType;

    // Projection of a vectorData
    typedef otb::VectorDataProjectionFilter<VectorDataType,
        VectorDataType>                                                 VectorDataProjectionFilterType;

    ConfusionFilterType::Pointer                                        confusioncalculator;
        

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
    OTBVectorImagePort  inPort1_;
    OTBVectorDataPort   inPort2_;
    OTBVectorDataPort   inPort3_;

    ButtonProperty                                                      m_updateProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBCONFUSIONMATRIXCALCULATORPROCESSOR_H

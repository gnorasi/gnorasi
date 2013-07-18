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

#ifndef OTBMRFENERGYGAUSSIANCLASSIFICATIONFILTERPROCESSOR_H
#define OTBMRFENERGYGAUSSIANCLASSIFICATIONFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "otbMarkovRandomFieldFilter.h"
#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"

namespace voreen {

class OTBMRFEnergyGaussianClassificationFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBMRFEnergyGaussianClassificationFilterProcessor();
    virtual ~OTBMRFEnergyGaussianClassificationFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Classification"; }
    virtual std::string getClassName() const { return "Markov Classification Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                       Dimension = 2;
    typedef unsigned char                           LabelledPixelType;
    typedef otb::Image<LabelledPixelType,Dimension> LabelledImageType;

    typedef otb::MarkovRandomFieldFilter<ImageType, LabelledImageType> MarkovRandomFieldFilterType;
    MarkovRandomFieldFilterType::Pointer markovFilter;

    typedef otb::MRFSamplerRandom<ImageType, LabelledImageType> SamplerType;
    SamplerType::Pointer sampler;

    typedef otb::MRFOptimizerMetropolis OptimizerType;
    OptimizerType::Pointer optimizer;

    typedef otb::MRFEnergyPotts<LabelledImageType, LabelledImageType> EnergyRegularizationType;
    EnergyRegularizationType::Pointer energyRegularization;

    typedef otb::MRFEnergyGaussianClassification<ImageType, LabelledImageType> EnergyFidelityType;
    EnergyFidelityType::Pointer energyFidelity;

    typedef itk::RescaleIntensityImageFilter<LabelledImageType, LabelledImageType> RescalerType;
    RescalerType::Pointer rescaler;

    typedef itk::CastImageFilter<LabelledImageType, ImageType> LabelToPortCastingFilterType;
    LabelToPortCastingFilterType::Pointer labelToPortCaster;

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

    FloatProperty lambda_;
    FloatProperty optimizerTemperature_;
    IntProperty numberOfIterations_;
    BoolProperty useRandomValue_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBMRFENERGYGAUSSIANCLASSIFICATIONFILTERPROCESSOR_H

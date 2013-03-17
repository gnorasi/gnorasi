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

#ifndef VRN_OTBLAINDVIPROCESSOR_H
#define VRN_OTBLAINDVIPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbVegetationIndicesFunctor.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

namespace voreen {
  
class OTBLAINDVIProcessor : public Processor {
public:
    OTBLAINDVIProcessor();
    virtual ~OTBLAINDVIProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Radiometry"; }
    virtual std::string getClassName() const { return "Calculate LAI from NDVI"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    
    typedef double PixelType;
    typedef otb::Image<PixelType> ImageType;
    typedef otb::VectorImage<PixelType> VectorImageType;
    typedef otb::Functor::LAIFromNDVILogarithmic<VectorImageType::InternalPixelType,VectorImageType::InternalPixelType,ImageType::PixelType> LAIFunctor;
    typedef otb::MultiChannelRAndNIRIndexImageFilter<VectorImageType,
      ImageType,LAIFunctor> LAINDVIImageFilterType;
    
    LAINDVIImageFilterType::Pointer filter;
        

protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();
    void setRedBandIndex();
    void setNirBandIndex();

    void updateSoilProperty();
    void updateInfProperty();
    void updateCoefficientProperty();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBVectorImagePort inPort_;
    OTBImagePort outPort_;
    
    IntProperty redBand_;	     ///< Input RED band selector
    IntProperty nirBand_;	     ///< Input RED band selector

    FloatProperty m_ndviSoilProperty;
    FloatProperty m_NdviInfProperty;
    FloatProperty m_ExtinctionCoefficientProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBLAINDVIPROCESSOR_H

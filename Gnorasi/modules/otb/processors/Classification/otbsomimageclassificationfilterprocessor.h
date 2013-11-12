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

#ifndef OTBSOMIMAGECLASSIFICATIONFILTERPROCESSOR_H
#define OTBSOMIMAGECLASSIFICATIONFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "../../ports/otbsommapport.h"
#include "itkCastImageFilter.h"
#include "otbSOMImageClassificationFilter.h"
#include "otbSOMMap.h"


namespace voreen {

class OTBSOMImageClassificationFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBSOMImageClassificationFilterProcessor();
    virtual ~OTBSOMImageClassificationFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Classification"; }
    virtual std::string getClassName() const { return "SOM Image Classification"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                                           Dimension = 2;
    typedef unsigned short                                              LabeledPixelType;
    typedef otb::VectorImage<PixelType,Dimension>                       VectorImageType;
    typedef otb::Image<LabeledPixelType,Dimension>                      LabeledImageType;

    typedef otb::SOMMap<VectorImageType::PixelType>                     SOMMapType;
    typedef otb::SOMImageClassificationFilter<VectorImageType, LabeledImageType, SOMMapType> ClassificationFilterType;
    ClassificationFilterType::Pointer filter;

    typedef itk::CastImageFilter<LabeledImageType, ImageType> LabelToPortCastingFilterType;
    LabelToPortCastingFilterType::Pointer labelToPortCaster;

//    typedef itk::CastImageFilter<VectorDoubleImageType, InputImageType> VectorCastingFilterType;
//    VectorCastingFilterType::Pointer vectorCaster;

protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    void update();

private:

    OTBVectorImagePort inPort_;
    OTBSOMMapPort SOMMapPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBSOMIMAGECLASSIFICATIONFILTERPROCESSOR_H

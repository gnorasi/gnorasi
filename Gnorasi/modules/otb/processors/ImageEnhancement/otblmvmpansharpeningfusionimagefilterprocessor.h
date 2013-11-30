/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#ifndef OTBLMVMPANSHARPENINGFUSIONIMAGEFILTERPROCESSOR_H
#define OTBLMVMPANSHARPENINGFUSIONIMAGEFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "../../ports/otblabelmapport.h"
#include "otbLmvmPanSharpeningFusionImageFilter.h"
#include "itkVectorCastImageFilter.h"

namespace voreen {

class OTBLmvmPanSharpeningFusionImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBLmvmPanSharpeningFusionImageFilterProcessor();

    virtual ~OTBLmvmPanSharpeningFusionImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Enhancement"; }
    virtual std::string getClassName() const { return "LMVM Pan Sharpening Fusion Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;
    typedef otb::Image<double, Dimension> ImageType;
    typedef otb::VectorImage<double, Dimension> VectorImageType;
    typedef otb::VectorImage<unsigned short int, Dimension> VectorIntImageType;


    typedef otb::LmvmPanSharpeningFusionImageFilter<ImageType, VectorImageType, VectorIntImageType> LmvmFusionFilterType;
    LmvmFusionFilterType::Pointer fusion;

    typedef itk::VectorCastImageFilter<VectorIntImageType, VectorImageType> VectorCastFilterType;
    VectorCastFilterType::Pointer caster;

protected:
    virtual void setDescriptions()
    {
        setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    IntProperty filterRadius_;

    OTBImagePort inPort_;
    OTBVectorImagePort vectorInPort_;
    OTBVectorImagePort vectorOutPort_;

    static const std::string loggerCat_; ///< category used in logging

};

} //namespace

#endif // OTBLMVMPANSHARPENINGFUSIONIMAGEFILTERPROCESSOR_H

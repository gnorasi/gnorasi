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

#ifndef OTBRASTERIZEVECTORDATAFILTERPROCESSOR_H
#define OTBRASTERIZEVECTORDATAFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/intproperty.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbvectordataport.h"
#include "../../ports/otbvectorimageport.h"
#include "otbImage.h"
#include "otbVectorData.h"
#include "otbRasterizeVectorDataFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBRasterizeVectorDataFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBRasterizeVectorDataFilterProcessor();

    virtual ~OTBRasterizeVectorDataFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Rasterize Vector Data Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                           Dimension = 2;
    typedef otb::VectorImage<double, 2> VectorImageType;

    typedef otb::VectorData<double, 2> DataType;

    //FILTER

    typedef otb::RasterizeVectorDataFilter<DataType, VectorImageType, VectorImageType> RasterizeVectorDataFilterType;
    RasterizeVectorDataFilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void update(); //set add color property visibility
    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport);

private:

    OTBVectorDataPort inVectorDataPort_;
    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;

    BoolProperty useAddColor_;
    IntProperty burnValuePix_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBRASTERIZEVECTORDATAFILTERPROCESSOR_H

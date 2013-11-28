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

#ifndef OTBLABELIMAGETOVECTORDATAFILTERPROCESSOR_H
#define OTBLABELIMAGETOVECTORDATAFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otblabelimageport.h"
#include "../../ports/otbvectordataport.h"
#include "otbVectorData.h"
#include "otbLabelImageToVectorDataFilter.h"

namespace voreen {

class OTBLabelImageToVectorDataFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBLabelImageToVectorDataFilterProcessor();

    virtual ~OTBLabelImageToVectorDataFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Label Image to Vector Data Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                   Dimension = 2;

    typedef unsigned long                       PixelType;
    typedef otb::Image<PixelType, Dimension>    LabelledImageType;
    typedef otb::VectorData<double, Dimension>  VectorDataType;

    typedef otb::LabelImageToVectorDataFilter<LabelledImageType> VectorizationFilterType;
    VectorizationFilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBLabelImagePort inPort_;
    OTBVectorDataPort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBLABELIMAGETOVECTORDATAFILTERPROCESSOR_H

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) ****** - All rights reserved.                                  *
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

#ifndef OTBDOTPRODUCTIMAGEFILTERPROCESSOR_H
#define OTBDOTPRODUCTIMAGEFILTERPROCESSOR_H

#include "otbimagefilterprocessor.h"
#include "otbVectorImage.h"
#include "../../ports/otbvectorimageport.h"
#include "otbDotProductImageFilter.h"

namespace voreen {

class OTBDotProductImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBDotProductImageFilterProcessor();

    virtual ~OTBDotProductImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Dot Product Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> VectorImageType;

    typedef otb::DotProductImageFilter<VectorImageType, VectorImageType> FilterType;
    FilterType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

private:

    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBDOTPRODUCTIMAGEFILTERPROCESSOR_H

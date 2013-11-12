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

#ifndef OTBZEROCROSSINGBASEDEDGEDETECTIONIMAGEFILTERPROCESSOR_H
#define OTBZEROCROSSINGBASEDEDGEDETECTIONIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"
#include "voreen/core/properties/floatproperty.h"

namespace voreen {

class OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor();

    virtual ~OTBZeroCrossingBasedEdgeDetectionImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Zero Crossing Based Edge Detection Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType> ZeroCrossingType;
    ZeroCrossingType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty variance_;
    FloatProperty maxError_;
    FloatProperty backgroundValue_;
    FloatProperty foregroundValue_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBZEROCROSSINGBASEDEDGEDETECTIONIMAGEFILTERPROCESSOR_H

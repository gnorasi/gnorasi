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

#ifndef OTBMORPHOLOGICALWATERSHEDIMAGEFILTERPROCESSOR_H
#define OTBMORPHOLOGICALWATERSHEDIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include "voreen/core/properties/intproperty.h"
#include "modules/otb/ports/otblabelimageport.h"
#include "voreen/core/properties/floatproperty.h"


namespace voreen {

class OTBMorphologicalWatershedImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBMorphologicalWatershedImageFilterProcessor();

    virtual ~OTBMorphologicalWatershedImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Enhancement"; }
    virtual std::string getClassName() const { return "Morphological Watershed Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    //virtual void bypass(OTBImagePort *inport, OTBLabelImagePort *outport); ///< Passes the image from inport to outport without changes.

    typedef itk::Image<unsigned long, 2> LabeledImageType;

    typedef itk::MorphologicalWatershedImageFilter<ImageType, LabeledImageType> MorphologicalWatershedType;
    MorphologicalWatershedType::Pointer filter;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

//    virtual void bypass(OTBImagePort *inport, LabeledImageType *outport); ///< Passes the image from inport to outport without changes.


private:

    BoolProperty fullyConnected_;
    FloatProperty waterShedLevel_;
    BoolProperty markWatershedLine_;

    OTBImagePort inPort_;
    OTBLabelImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBMORPHOLOGICALWATERSHEDIMAGEFILTERPROCESSOR_H

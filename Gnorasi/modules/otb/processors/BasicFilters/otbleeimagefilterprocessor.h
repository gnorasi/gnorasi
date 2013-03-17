/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#ifndef OTBLEEIMAGEFILTERPROCESSOR_H
#define OTBLEEIMAGEFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbimagefilterprocessor.h"
#include "otbLeeImageFilter.h"

namespace voreen {

class OTBLeeImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBLeeImageFilterProcessor();
    virtual ~OTBLeeImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Edge-preserving Speckle (LEE) image filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::LeeImageFilter<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;


    FilterType::Pointer filter;


protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:
    //Size of window to be used for the computation
    //Filter Size—3 x 3, 5 x 5, 7 x 7, 9 x 9, 11 x 11
    IntProperty radius_;
    //The Number of Looks parameter controls
    //image smoothing and estimates noise variance.
    //The smaller the value is, the better the smoothing effect and filter performance.
    //A larger value retains more image features.
    //Default value 1.0
    FloatProperty numberOfLooks_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;


    static const std::string loggerCat_; ///< category used in logging

};

} //namespace

#endif // OTBLEEIMAGEFILTERPROCESSOR_H


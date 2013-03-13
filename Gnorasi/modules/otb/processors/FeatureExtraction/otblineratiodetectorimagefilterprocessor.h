/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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
 
#ifndef OTBLINERATIODETECTORIMAGEFILTERPROCESSOR_H
#define OTBLINERATIODETECTORIMAGEFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbimageport.h"
#include "otbLineRatioDetectorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

namespace voreen {

class OTBLineRatioDetectorImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBLineRatioDetectorImageFilterProcessor();

    virtual ~OTBLineRatioDetectorImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "OTBLineRatioDetectorImageFilter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef  float OutputPixelType;
    typedef otb::Image<OutputPixelType,  2>   OutputImageType;

    typedef otb::LineRatioDetectorImageFilter<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;
    //typedef itk::RescaleIntensityImageFilter<OTBImageFilterProcessor::ImageType,OutputImageType> RescalerFilterType;


    FilterType::Pointer filter;
    //RescalerFilterType::Pointer rescaler_Filter;

protected:
    virtual void setDescriptions()
    {
        setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    //minimum length of line
    IntProperty lengthLine_;
    //minimum Width of line
    IntProperty widthLine_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging

};

} //namespace

#endif // OTBLINERATIODETECTORIMAGEFILTERPROCESSOR_H

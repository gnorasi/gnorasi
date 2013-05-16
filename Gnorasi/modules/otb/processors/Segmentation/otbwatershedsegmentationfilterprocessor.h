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
#ifndef OTBWATERSHEDSEGMENTATIONFILTERPROCESSOR_H
#define OTBWATERSHEDSEGMENTATIONFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include <itkRGBPixel.h>
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "otbWatershedSegmentationFilter.h"

namespace voreen {

class OTBWatershedSegmentationFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBWatershedSegmentationFilterProcessor();

    virtual ~OTBWatershedSegmentationFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Segmentation"; }
    virtual std::string getClassName() const { return "Watershed Segmentation"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;


    typedef itk::RGBPixel<unsigned char>   RGBPixelType;
    typedef otb::Image<RGBPixelType, 2>    RGBImageType;
    typedef itk::Vector<float, 3>          VectorPixelType;
    typedef itk::Image<VectorPixelType, 2> VectorImageType;
    typedef itk::Image<unsigned long, 2>   LabeledImageType;
    typedef itk::Image<float, 2>           ScalarImageType;

    typedef itk::VectorCastImageFilter<RGBImageType, VectorImageType>
	CastFilterType;
    typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType,
        VectorImageType> DiffusionFilterType;
    typedef itk::VectorGradientMagnitudeImageFilter<VectorImageType, float, ScalarImageType>
	GradientMagnitudeFilterType;
    typedef itk::WatershedImageFilter<OTBImageFilterProcessor::ImageType> WatershedFilterType;

    WatershedFilterType::Pointer waterShedFilter;

protected:
    virtual void setDescriptions()
    {
        setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    //Level controls watershed depth
    FloatProperty waterShedLevel_;
    //controls the lower thresholding of the input
    FloatProperty waterShedThreshold_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging

};

} //namespace


#endif // OTBWATERSHEDSEGMENTATIONFILTERPROCESSOR_H

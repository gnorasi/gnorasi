/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

#ifndef VRN_OTBTWOVECTORIMAGESBANDMATHPROCESSOR_H
#define VRN_OTBTWOVECTORIMAGESBANDMATHPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/stringproperty.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbBandMathImageFilter.h"


namespace voreen {
  
class OTBTwoVectorImagesBandMathProcessor : public Processor {
public:
    OTBTwoVectorImagesBandMathProcessor();
    virtual ~OTBTwoVectorImagesBandMathProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Radiometry"; }
    virtual std::string getClassName() const { return "Band Math Calculator (Two Images)"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    
    typedef double PixelType;
    typedef otb::Image<PixelType> ImageType;
    typedef otb::VectorImage<PixelType> VectorImageType;
    typedef otb::ImageList<ImageType>   ImageListType;

    typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType>
						  VectorImageToImageListType;
    typedef otb::BandMathImageFilter<ImageType>   FilterType;
    
    VectorImageToImageListType::Pointer imageList;
    VectorImageToImageListType::Pointer imageList2;
    FilterType::Pointer filter;
      

protected:
    void process();
    void setExpresion();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBVectorImagePort inPort_;
    OTBVectorImagePort inPort2_;
    OTBImagePort outPort_;
    
    StringProperty expression_;	     ///< Math expression to compute

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBTWOVECTORIMAGESBANDMATHPROCESSOR_H

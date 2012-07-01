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

#ifndef VRN_OTBMEANSHIFTSEGMENTATIONPROCESSOR_H
#define VRN_OTBMEANSHIFTSEGMENTATIONPROCESSOR_H


#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "otbimagefilterprocessor.h"
#include "otbMeanShiftVectorImageFilter.h"
#include "../../ports/otbvectorimageport.h"
#include "../../ports/otblabelimageport.h"
#include "otbVectorImage.h"

namespace voreen {
  
class OTBMeanShiftSegmentationProcessor : public OTBImageFilterProcessor {

public:
    OTBMeanShiftSegmentationProcessor();
    virtual ~OTBMeanShiftSegmentationProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Image Segmentation"; }
    virtual std::string getClassName() const { return "Mean Shift Image Segmentation"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    virtual bool isReady() const;
    
    typedef otb::VectorImage<double, 2> VectorImageType;
    typedef otb::Image<unsigned long, 2> LabeledImageType;
    typedef otb::MeanShiftVectorImageFilter<VectorImageType, VectorImageType, LabeledImageType> FilterType;
    FilterType::Pointer filter;
    
protected:
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

private:

    IntProperty spatialRadius_; 
    FloatProperty rangeRadius_;
    IntProperty minRegionSize_; 
    FloatProperty scale_;

    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;
    OTBVectorImagePort outPort2_;
    OTBLabelImagePort outPort3_;
    OTBLabelImagePort outPort4_;
    OTBVectorImagePort outPort5_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBMEANSHIFTSEGMENTATIONPROCESSOR_H

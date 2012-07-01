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

#ifndef VRN_OTBBINARYTHRESHOLDFILTERPROCESSOR_H
#define VRN_OTBBINARYTHRESHOLDFILTERPROCESSOR_H


#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbimagefilterprocessor.h"
#include "itkBinaryThresholdImageFilter.h"


namespace voreen {
  
class OTBBinaryThresholdFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBBinaryThresholdFilterProcessor();
    virtual ~OTBBinaryThresholdFilterProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Image Segmentation"; }
    virtual std::string getClassName() const { return "Binary Threshold Image Segmentation"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    
    typedef itk::BinaryThresholdImageFilter<OTBImageFilterProcessor::ImageType,
				OTBImageFilterProcessor::ImageType>  FilterType;
    FilterType::Pointer filter;
        

protected:
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty insideValue_;
    FloatProperty outsideValue_;
    FloatProperty lowerThreshold_;
    FloatProperty upperThreshold_;
    
    OTBImagePort inPort_;
    OTBImagePort outPort_;

    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBBINARYTHRESHOLDFILTERPROCESSOR_H

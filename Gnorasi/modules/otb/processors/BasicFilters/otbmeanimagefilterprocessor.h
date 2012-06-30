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

#ifndef VRN_OTBMEANIMAGEFILTERPROCESSOR_H
#define VRN_OTBMEANIMAGEFILTERPROCESSOR_H


#include "voreen/core/properties/intproperty.h"
#include "otbimagefilterprocessor.h"
#include "itkMeanImageFilter.h"


namespace voreen {
  
class OTBMeanImageFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBMeanImageFilterProcessor();
    virtual ~OTBMeanImageFilterProcessor();
    
    virtual Processor* create() const { return new OTBMeanImageFilterProcessor(); }
    
    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Mean Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    
    typedef itk::MeanImageFilter<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;
    FilterType::Pointer filter;
        

protected:
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    IntProperty filterSize_;
    
    OTBImagePort inPort_;
    OTBImagePort outPort_;

    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBMEANIMAGEFILTERPROCESSOR_H

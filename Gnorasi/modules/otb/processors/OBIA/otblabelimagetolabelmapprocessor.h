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

#ifndef VRN_OTBLABELIMAGETOLABELMAPPROCESSOR_H
#define VRN_OTBLABELIMAGETOLABELMAPPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "../../ports/otblabelimageport.h"
#include "../../ports/otblabelmapport.h"
#include "otbImage.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"


namespace voreen {
  
class OTBLabelImageToLabelMapProcessor : public Processor {
public:
    OTBLabelImageToLabelMapProcessor();
    virtual ~OTBLabelImageToLabelMapProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Label Image to Object Map"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    //virtual bool isReady() const;
    
    typedef unsigned long           LabelType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>
							    LabelMapFilterType;
    LabelMapFilterType::Pointer labelMapFilter;
    
    
protected:
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBLabelImagePort inPort_;
    OTBLabelMapPort outPort_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBLABELIMAGETOLABELMAPPROCESSOR_H

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

#ifndef VRN_OTBLABELIMAGETOCLASSIMAGEPROCESSOR_H
#define VRN_OTBLABELIMAGETOCLASSIMAGEPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "../../ports/otblabelimageport.h"
#include "voreen/core/ports/textport.h"
#include "otbImage.h"
#include "itkImageLinearConstIteratorWithIndex.h" 
#include "itkImageLinearIteratorWithIndex.h"
#include <sstream>
#include <map>

namespace voreen {

class OTBLabelImageToClassImageProcessor : public Processor {
public:
    OTBLabelImageToClassImageProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Label Image to Classified Image"; }
    virtual std::string getCategory() const  { return "OBIA"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isReady() const;
    
    typedef unsigned long           LabelType;
    typedef unsigned short          ClassType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef LabeledImageType* LabelImagePointer;
    
    typedef itk::ImageLinearIteratorWithIndex<LabeledImageType>      IteratorType; 
    typedef itk::ImageLinearConstIteratorWithIndex<LabeledImageType> ConstIteratorType;
    
    LabelImagePointer inLabelImg;
    LabelImagePointer outLabelImg;
   
    void update();

protected:
    virtual void setDescriptions() {
	setDescription("Processor to replace segmentation object id with class id in order to produce a classified image.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    
    OTBLabelImagePort inPort_;
    TextPort inTextPort_;
    OTBLabelImagePort outPort_;
    
    ButtonProperty updateButton_;    ///< Update
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBOBJECTCLASSIFCATIONTOLABELMAPPROCESSOR_H

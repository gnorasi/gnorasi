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

#ifndef VRN_OTBLABELMAPTOCLASSIMAGEPPROCESSOR_H
#define VRN_OTBLABELMAPTOCLASSIMAGEPPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "../../ports/otblabelimageport.h"
#include "../../ports/otblabelmapport.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"


namespace voreen {
  
class OTBLabelMapToClassImageProcessor : public Processor {
public:
    OTBLabelMapToClassImageProcessor();
    virtual ~OTBLabelMapToClassImageProcessor();
    
    virtual Processor* create() const { return new OTBLabelMapToClassImageProcessor(); }
    
    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Object Map to Classified Image"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    virtual bool isReady() const;
    
    typedef unsigned long           LabelType;
    typedef unsigned short	    ClassType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef otb::Image<ClassType, 2> ClassLabeledImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, ClassType> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    
    typedef otb::LabelMapWithClassLabelToClassLabelImageFilter<LabelMapType, ClassLabeledImageType>
							    LabelMapFilterType;
    LabelMapFilterType::Pointer labelMapFilter;
    
    typedef otb::ImageFileWriter<ClassLabeledImageType> WriterType;
    WriterType::Pointer writer;
    
    
    void saveImage();
    void clearImage();
    
    
protected:
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    bool hasImage;
    
    OTBLabelMapPort inPort_;
    
    FileDialogProperty imageFile_;  ///< Path of the saved image file.
    ButtonProperty clearImage_;      ///< Executes clearImage().
    ButtonProperty saveImageButton_;    ///< saves the image.
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBLABELMAPTOCLASSIMAGEPPROCESSOR_H

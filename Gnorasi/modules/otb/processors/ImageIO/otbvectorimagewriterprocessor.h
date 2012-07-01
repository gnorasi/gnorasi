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

#ifndef VRN_OTBVECTORIMAGEWRITERPROCESSOR_H
#define VRN_OTBVECTORIMAGEWRITERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "../../ports/otbvectorimageport.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

namespace voreen {

class OTBVectorImageWriterProcessor : public Processor {
public:
    OTBVectorImageWriterProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "MultiBand Image File Writer"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    
    //OTB specific stuff***************************************************
    //Default 32 bit image writer
    typedef double                   DoublePixelType;
    typedef otb::VectorImage<DoublePixelType, 2> ImageType;
    typedef ImageType* 		     ImagePointer;
    typedef otb::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer;
    
    //16 bit image writer
    typedef unsigned short             IntegerPixelType;
    typedef otb::VectorImage<IntegerPixelType, 2> IntegerImageType;
    typedef otb::ImageFileWriter<IntegerImageType> IntegerWriterType;
    IntegerWriterType::Pointer int_writer;
    typedef otb::VectorRescaleIntensityImageFilter<
      ImageType,
      IntegerImageType>    IntegerRescalerFilterType;
    IntegerRescalerFilterType::Pointer  intrescaler;
    
    //float image writer
    typedef float             FloatPixelType;
    typedef otb::VectorImage<FloatPixelType, 2> FloatImageType;
    typedef otb::ImageFileWriter<FloatImageType> FloatWriterType;
    FloatWriterType::Pointer float_writer;
    typedef otb::VectorRescaleIntensityImageFilter<
      ImageType,
      FloatImageType>    FloatRescalerFilterType;
    FloatRescalerFilterType::Pointer  floatrescaler;
    
    //8bit image writer
    //TODO: set default???
    typedef unsigned char             BytePixelType;
    typedef otb::VectorImage<BytePixelType, 2> ByteImageType;
    typedef otb::ImageFileWriter<ByteImageType> ByteWriterType;
    ByteWriterType::Pointer byte_writer;
    typedef otb::VectorRescaleIntensityImageFilter<
      ImageType,
      ByteImageType>    ByteRescalerFilterType;
    ByteRescalerFilterType::Pointer  byterescaler;
    
    //**********************************************************************
    
     /**
     * Saves the image.
     *
     * @note The processor must have been initialized
     *       before calling this function.
     */
    void saveImage();
    
    /**
     * Clears the loaded image. Processor needs to be initialized when calling this function.
     */
    void clearImage();
    

protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    bool hasImage;
  
    OTBVectorImagePort inport_;
    FileDialogProperty imageFile_;  ///< Path of the saved image file.
    ButtonProperty clearImage_;      ///< Executes clearImage().
    StringOptionProperty imageType_;///< Select output image type
    FloatProperty clip_; 	///< Perform rescaling on output image
    FloatProperty min_; 		///< Minimum output value
    FloatProperty max_; 		///< Maximum output value
    ButtonProperty saveImageButton_;    ///< saves the image.
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBVECTORIMAGEWRITERPROCESSOR_H

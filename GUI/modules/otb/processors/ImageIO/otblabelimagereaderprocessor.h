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

#ifndef VRN_OTBLABELIMAGEREADERPROCESSOR_H
#define VRN_OTBLABELIMAGEREADERPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "../../ports/otblabelimageport.h"
#include "otbImage.h"
#include "otbImageFileReader.h"


namespace voreen {

/**
 * Loads an label image file from disk and supplies the SmartPointer to it.
 */
  
class OTBLabelImageReaderProcessor : public Processor {
public:
    OTBLabelImageReaderProcessor();
    virtual ~OTBLabelImageReaderProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "LabelImageFileReader"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    typedef unsigned int             PixelType;
    typedef otb::Image<PixelType, 2> ImageType;
    typedef ImageType* 		     ImagePointer;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader;
    
        /**
     * Loads the image specified by filename.
     *
     * @note The processor must have been initialized
     *       before calling this function.
     *
     * @param filename the image to load
     */
    void loadImage();
    
     /**
     * Returns the currently assigned image pointer.
     */
    ImagePointer getImage();
    
    //virtual bool usesExpensiveComputation() const { return true; }
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    virtual std::string getProcessorInfo() const;

    

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:
    void setOutPortData();
    void readData();
    void updateView();
    bool hasImage;
    
    /**
     * Clears the loaded image. Processor needs to be initialized when calling this function.
     */
    void clearImage();

    OTBLabelImagePort outPort_;
    
    FileDialogProperty imageFile_;  ///< Path of the loaded image file.
    ButtonProperty clearImage_;      ///< Executes clearImage().

    ImagePointer pDataOut_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBLABELIMAGEREADERPROCESSOR_H


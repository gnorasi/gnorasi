/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_IMAGEWRITERPROCESSOR_H
#define VRN_IMAGEWRITERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "../ports/otbimageport.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"

namespace voreen {

class ImageWriterProcessor : public Processor {
public:
    ImageWriterProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "otbImageWriter"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    
    //OTB specific stuff***************************************************
    //Default 32 bit image writer
    typedef double                   DoublePixelType;
    typedef otb::Image<DoublePixelType, 2> ImageType;
    typedef ImageType* 		     ImagePointer;
    typedef otb::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer;
    
    //16 bit image writer
    typedef unsigned int             IntegerPixelType;
    typedef otb::Image<IntegerPixelType, 2> IntegerImageType;
    typedef otb::ImageFileWriter<IntegerImageType> IntegerWriterType;
    IntegerWriterType::Pointer int_writer;
    
    //float image writer
    typedef float             FloatPixelType;
    typedef otb::Image<FloatPixelType, 2> FloatImageType;
    typedef otb::ImageFileWriter<FloatImageType> FloatWriterType;
    FloatWriterType::Pointer float_writer;
    
    //8bit image writer
    //TODO: set default???
    typedef unsigned char             BytePixelType;
    typedef otb::Image<BytePixelType, 2> ByteImageType;
    typedef otb::ImageFileWriter<ByteImageType> ByteWriterType;
    ByteWriterType::Pointer byte_writer;
    
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
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    bool hasImage;
  
    OTBImagePort inport_;
    FileDialogProperty imageFile_;  ///< Path of the saved image file.
    StringOptionProperty imageType_;///< Select output image type
    ButtonProperty clearImage_;      ///< Executes clearImage().
    ButtonProperty saveImageButton_;    ///< saves the image.
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_IMAGEWRITERPROCESSOR_H

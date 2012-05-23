/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_IMAGEREADERPROCESSOR_H
#define VRN_IMAGEREADERPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "../ports/otbimageport.h"
#include "otbImage.h"
#include "otbImageFileReader.h"


namespace voreen {

/**
 * Loads an image file from disk and supplies the SmartPointer to it.
 */
  
class ImageReaderProcessor : public Processor {
public:
    ImageReaderProcessor();
    virtual ~ImageReaderProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "otbImageReader"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }
    
    typedef double                   PixelType;
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
    void loadImage(const std::string& filename);
    
     /**
     * Returns the currently assigned image pointer.
     */
    const ImagePointer getImage() const;
    
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

    OTBImagePort outPort_;
    
    FileDialogProperty imageFile_;  ///< Path of the loaded image file.
    ButtonProperty clearImage_;      ///< Executes clearImage().

    ImagePointer pDataOut_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_IMAGEREADERPROCESSOR_H


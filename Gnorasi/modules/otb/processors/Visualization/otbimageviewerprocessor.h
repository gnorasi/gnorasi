#ifndef VRN_OTBIMAGEVIEWERPROCESSOR_H
#define VRN_OTBIMAGEVIEWERPROCESSOR_H

#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"
#include "../../ports/otblabelmapport.h"
#include "../../ports/otblabelimageport.h"

namespace voreen {

class VRN_CORE_API OTBImageViewerProcessor : public Processor {
public:
    OTBImageViewerProcessor();
    virtual ~OTBImageViewerProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Otb Image Viewer"; }
    virtual std::string getCategory() const  { return "Visualization"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

    
     /**
     * Shows the image.
     *
     * @note The processor must have been initialized
     *       before calling this function.
     */
    void showImage();

//    ImagePointer getImagePointer() { return outPort_.getData(); }

    void loadImage();

    std::string getFilePath(){ return pTextData_; }
    
    
protected:
    virtual void setDescriptions() {
        setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    void updateView();
  
    OTBImagePort inPort_;
    OTBImagePort outPort_;		///< passes the image directly to a next processor

    OTBVectorImagePort inPort2_;
    OTBVectorImagePort outPort2_;

    OTBLabelMapPort lblMapPort_;
//    OTBLabelImagePort lblImagePort_;

    ButtonProperty showImageButton_;    ///< shows the image.

    FileDialogProperty loadImageFile_;  ///< Path of the RDF file.

    std::string pTextData_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBIMAGEVIEWERPROCESSOR_H

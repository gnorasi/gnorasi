/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_OTBIMAGEFILTERPROCESSOR_H
#define VRN_OTBIMAGEFILTERPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"



namespace voreen {

/**
 * Base class for image filter processors.
 *
 */
  
class OTBImageFilterProcessor : public Processor {
public:
    OTBImageFilterProcessor();
    virtual ~OTBImageFilterProcessor();
    
    typedef double                   PixelType;
    typedef otb::Image<PixelType, 2> ImageType;
    typedef ImageType* 		     ImagePointer;
    
    virtual std::string getProcessorInfo() const;

protected:
    virtual void bypass(OTBImagePort *inport, OTBImagePort *outport); ///< Passes the image from inport to outport without changes.
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    BoolProperty enableSwitch_; ///< Should be used to control if bypass or actual image processing is applied

private:

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBIMAGEFILTERPROCESSOR_H

/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_OTBCONVOLUTIONIMAGEFILTERPROCESSOR_H
#define VRN_OTBCONVOLUTIONIMAGEFILTERPROCESSOR_H


#include "voreen/core/properties/intproperty.h"
#include "otbimagefilterprocessor.h"
#include "otbConvolutionImageFilter.h"
#include "itkArray.h"

namespace voreen {

/**
 * Base class for image filter processors.
 *
 */
  
class OTBConvolutionImageFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBConvolutionImageFilterProcessor();
    virtual ~OTBConvolutionImageFilterProcessor();
    
    virtual Processor* create() const { return new OTBConvolutionImageFilterProcessor(); }
    
    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "otbConvolutionImageFilter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }
    
    virtual std::string getProcessorInfo() const;
    
    typedef otb::ConvolutionImageFilter<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;
    FilterType::Pointer filter;
    typedef itk::Array<double> ArrayType;
    

protected:
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    IntProperty filterSize_;
    StringProperty kernel_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBCONVOLUTIONIMAGEFILTERPROCESSOR_H

#include "otbdiscretegaussianimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBDiscreteGaussianImageFilterProcessor::loggerCat_("voreen.OTBDiscreteGaussianImageFilterProcessor");


OTBDiscreteGaussianImageFilterProcessor::OTBDiscreteGaussianImageFilterProcessor()
    : OTBImageFilterProcessor(),
    gaussianVarianceValue_("gaussianVarianceValue", "Gaussian Variance Value", 1.0f, 0.0f, 1024.0f),
    maxKernelWidthValue_("maxKernelWidthValue", "Max Kernel Width Value", 30.0f, 0.0f, 1024.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
    {
    addProperty(enableSwitch_);
    addProperty(gaussianVarianceValue_);
    addProperty(maxKernelWidthValue_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    }

Processor* OTBDiscreteGaussianImageFilterProcessor::create() const {
return new OTBDiscreteGaussianImageFilterProcessor();
}

OTBDiscreteGaussianImageFilterProcessor::~OTBDiscreteGaussianImageFilterProcessor() {

}

void OTBDiscreteGaussianImageFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBDiscreteGaussianImageFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBDiscreteGaussianImageFilterProcessor::getProcessorInfo() const {

return "Gaussian Image Filtering Processor";
}

void OTBDiscreteGaussianImageFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }

    //// The filter requires the user to provide a value for the variance
    // associated with the Gaussian kernel. The method \code{SetVariance()} is
    // used for this purpose. The discrete Gaussian is constructed as a
    // convolution kernel. The maximum kernel size can be set by the user. Note
    // that the combination of variance and kernel-size values may result in a
    // truncated Gaussian kernel.
    //
    // \index{itk::DiscreteGaussianImageFilter!SetVariance()}
    // \index{itk::DiscreteGaussianImageFilter!SetMaximumKernelWidth()}
    //
    //Property validation
    filter->SetVariance(gaussianVarianceValue_.get());
    filter->SetMaximumKernelWidth(maxKernelWidthValue_.get());

    try
    {
    filter->SetInput(inPort_.getData());

    filter->Update();

    outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
    LERROR("Problem with Gaussian Image Filter!");
    return;
    }
}




} //namespace

/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "otbconvolutionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBConvolutionImageFilterProcessor::loggerCat_("voreen.OTBConvolutionImageFilterProcessor");

OTBConvolutionImageFilterProcessor::OTBConvolutionImageFilterProcessor()
    : OTBImageFilterProcessor(),
    filterSize_("filterSize", "Filter Radius", 1),
    kernel_("kernel", "Filter Kernel in CSV", "1;1;1;1;1;1;1;1;1"),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(filterSize_);
    addProperty(kernel_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

OTBConvolutionImageFilterProcessor::~OTBConvolutionImageFilterProcessor() {

}

void OTBConvolutionImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBConvolutionImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBConvolutionImageFilterProcessor::getProcessorInfo() const {
    
    return "Base Filtering Processor";
}

void OTBConvolutionImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    
    int rad = filterSize_.get();
    int filternum = (2*rad+1)*(2*rad+1);
    ArrayType array(filternum);
    std::string line = kernel_.get();
    int itemnum = 0;
    std::istringstream linestream(line);
    std::string item;
    try
    {
	while (std::getline (linestream, item, ';'))
	{
	    double temp = (double)atof(item.c_str());
	    //LINFO("Testing kernel element: " << temp);
	    if(itemnum<filternum){
		array.SetElement(itemnum,temp);
	    }
	    itemnum++;
	}
    }
    catch (int e)
    {
	LERROR("Problem converting kernel element to number!");
	return;
    }
    
    if(filternum!=itemnum){
	LWARNING("Kernel size does not match provided elements");
	return;
    }else{
	LINFO("Kernel setup is OK");
    }
    
    filter->SetFilter(array);
    filter->SetInput(inPort_.getData());
    outPort_.setData(filter->GetOutput());
    
}


} // namespace

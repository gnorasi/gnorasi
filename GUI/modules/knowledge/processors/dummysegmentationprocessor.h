/**********************************************************************
 * GNORASI                                                            *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_DUMMYSEGMENTATIONPROCESSOR_H
#define VRN_DUMMYSEGMENTATIONPROCESSOR_H

#include "voreen/core/processors/renderprocessor.h"
#include "voreen/core/ports/textport.h"
//#include "voreen/core/processors/processorwidgetfactory.h"

//#include "voreen/core/properties/floatproperty.h"


namespace voreen {

class DummySegmentationProcessor : public RenderProcessor {
public:
    DummySegmentationProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Dummy Segmentation"; }
    virtual std::string getCategory() const  { return "Segmentation"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;

protected:
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    RenderPort inport_;
    TextPort outport_;
    //FloatProperty saturation_;
};

} // namespace

#endif // VRN_VRN_DUMMYSEGMENTATIONPROCESSOR_H

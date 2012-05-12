#ifndef VRN_CLASSIFIERWSPROCESSOR_H
#define VRN_CLASSIFIERWSPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/ports/textport.h"


namespace voreen {

class ClassifierWSProcessor : public Processor {
public:
    ClassifierWSProcessor();
    virtual ~ClassifierWSProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "ClassifierWS"; }
    virtual std::string getCategory() const  { return "Classification"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }
    //virtual bool usesExpensiveComputation() const { return true; }
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    virtual std::string getProcessorInfo() const;

    /// sets the link to the TextData object after calling WS (outport data actually)
    void setTextDataOut(std::string);
    /// returns the link to the TextData object (inport data actually...)
    const std::string getTextData() const;



protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:
    void setOutPortData();
    void readData();
    void updateView();

    TextPort inPort_;
    TextPort outPort_;

    std::string pTextData_;
    std::string pTextDataOut_;

    StringProperty serverURLupdate_;
    StringProperty serverURLquery_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_CLASSIFIERWSPROCESSOR_H


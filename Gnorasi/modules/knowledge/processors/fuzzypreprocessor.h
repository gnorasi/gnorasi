#ifndef VRN_FUZZYPREPROCESSOR_H
#define VRN_FUZZYPREPROCESSOR_H

#include "voreen/core/ports/textport.h"
#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "../../otb/ports/otblabelmapport.h"

namespace voreen {

class VRN_CORE_API FuzzyPreProcessor : public Processor {
public:
    FuzzyPreProcessor();
    virtual ~FuzzyPreProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Fuzzy Pre Processor"; }
    virtual std::string getCategory() const  { return "Classification"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

    std::string getFilePath(){ return pTextData_; }

    /*!
     * \brief setOutputData
     */
    void setOutputData(const OTBLabelMapPort::LabelMapPointer& pointer) { outLblMapPort_.setData(pointer); }
    
    void setTextOutputData(const std::string text) { outTextPort_.setData(text); }
    
protected:
    virtual void setDescriptions() {
        setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    void updateView();

    OTBLabelMapPort inLblMapPort_;
    OTBLabelMapPort outLblMapPort_;
    TextPort        outTextPort_;

    FileDialogProperty loadImageFile_;  ///< Path of the RDF file.

    std::string pTextData_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // FUZZYPREPROCESSOR_H

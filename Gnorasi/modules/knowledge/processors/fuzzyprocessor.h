#ifndef VRN_FUZZYPROCESSOR_H
#define VRN_FUZZYPROCESSOR_H

#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "../../otb/ports/otblabelmapport.h"

namespace voreen {

class VRN_CORE_API FuzzyProcessor : public Processor {
public:
    FuzzyProcessor();
    virtual ~FuzzyProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Fuzzy Processor"; }
    virtual std::string getCategory() const  { return "Classification"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

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

    OTBLabelMapPort inLblMapPort_;
    OTBLabelMapPort outLblMapPort_;

    FileDialogProperty loadImageFile_;  ///< Path of the RDF file.

    std::string pTextData_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_FUZZYPROCESSOR_H

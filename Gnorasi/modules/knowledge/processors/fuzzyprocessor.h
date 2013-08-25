#ifndef VRN_FUZZYPROCESSOR_H
#define VRN_FUZZYPROCESSOR_H

#include "voreen/core/ports/textport.h"
#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "../../otb/ports/otblabelmapport.h"
#include "modules/knowledge/ports/ontologyport.h"
#include "../ports/fuzzyruleport.h"

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
	
	std::string getOntologyData() { return inOntologyPort_.getData(); }

    std::string getFilePath(){ return pTextData_; }

    /*!
     * \brief setTextOutputData
     *  Set the text data in csv format
     * \param text
     */
    void setTextOutputData(const std::string text) { outTextPort_.setData(text); }

    /*!
     * \brief setFuzzyRuleTextOutputData
     *  Set the fuzzy rule data to xmk format
     * \param text
     */
    void setFuzzyRuleTextOutputData(const std::string text) { outFuzzyRulePort_.setData(text); }
    
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
    FuzzyRulePort   outFuzzyRulePort_;
	OntologyPort    inOntologyPort_;
    TextPort        outTextPort_;

    std::string pTextData_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_FUZZYPROCESSOR_H

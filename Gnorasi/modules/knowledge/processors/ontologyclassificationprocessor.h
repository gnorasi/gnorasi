#ifndef VRN_ONTOLOGYCLASSIFICATIONPROCESSOR_H
#define VRN_ONTOLOGYCLASSIFICATIONPROCESSOR_H

#include "voreen/core/ports/textport.h"
#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "../../otb/ports/otblabelmapport.h"
//#include "modules/knowledge/ports/ontologyport.h"
//#include "../ports/fuzzyruleport.h"

namespace voreen {

class VRN_CORE_API OntologyClassificationProcessor : public Processor {
public:
    OntologyClassificationProcessor();
    virtual ~OntologyClassificationProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "OntologyClassification Processor"; }
    virtual std::string getCategory() const  { return "Classification"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
//    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
	
//	std::string getOntologyData() { return inOntologyPort_.getData(); }

    std::string getFilePath();

    /*!
     * \brief setTextOutputData
     *  Set the text data in csv format
     * \param text
     */
//    void setTextOutputData(const std::string text) { outTextPort_.setData(text); }

    /*!
     * \brief setFuzzyRuleTextOutputData
     *  Set the fuzzy rule data to xmk format
     * \param text
     */
//    void setFuzzyRuleTextOutputData(const std::string text) { outFuzzyRulePort_.setData(text); }
    
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
//    FuzzyRulePort   outFuzzyRulePort_;
//	OntologyPort    inOntologyPort_;
    TextPort        outTextPort_;
    TextPort        outXmlPort;

    std::string pTextData_;

    FileDialogProperty XMLFile_;  ///< Path of the saved image file.
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_FUZZYPROCESSOR_H

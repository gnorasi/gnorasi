#ifndef VRN_KNOWLEDGEWEBSERVICEPROCESSOR_H
#define VRN_KNOWLEDGEWEBSERVICEPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/ports/textport.h"
#include "../ports/ontologyport.h"
#include "../ports/georuleport.h"
#include "../ports/fuzzyruleport.h"

#include "curl/curl.h"
#include <cstring>

namespace voreen {

class KnowledgeWebServiceProcessor : public Processor {

public:
    KnowledgeWebServiceProcessor();
    ~KnowledgeWebServiceProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Knowledge Web Service"; }
    virtual std::string getCategory() const  { return "Classification"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    virtual std::string getProcessorInfo() const;

    /// sets the link to the TextData object after calling WS (outport data actually)
    void setTextDataOut(std::string);
    /// returns the link to the TextData object (inport data actually...)
    const std::string getOntologyData() const;
    const std::string getGeoRuleData() const;
    const std::string getFuzzyRuleData() const;
    const std::string getObjectMapData() const;
    char* composeURL(std::string target);


protected:
    virtual void setDescriptions() {
        setDescription("Web Service Classification processor.");
    }
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:
    void setOutPortData();
    void readData();
    void updateView();
    static size_t getRequestData(void *ptr, size_t size, size_t nmemb, std::string *stream);
    void initConnection(CURL* curlHandle, char *truncate);
    void closeConnection(CURL* curlHandle);
    void sendOntology(CURL* curlHandle);
    void sendObjectMapData(CURL* curlHandle, char *srid, char *parseFeatures);
    void sendFuzzyRules(CURL* curlHandle);
    void sendGeoRules(CURL* curlHandle);
    void getClassificationResults(CURL* curlHandle);
    void getQueryResults(CURL* curlHandle, char *query);

    OntologyPort inOntologyPort_;
    GeoRulePort inGeoRulePort_;
    FuzzyRulePort inFuzzyRulePort_;
    TextPort inObjectMapPort_;
    TextPort outPort_;

    std::string pOntologyData_;
    std::string pGeoRuleData_;
    std::string pFuzzyRuleData_;
    std::string pObjectMapData_;
    std::string pTextDataOut_;

    StringProperty serverURLupdate_;

    ButtonProperty update_;

    static const std::string loggerCat_; ///< category used in logging
};

} //namespace
#endif // KNOWLEDGEWEBSERVICEPROCESSOR_H

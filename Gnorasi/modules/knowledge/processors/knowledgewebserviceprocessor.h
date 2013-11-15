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
    const std::string getXMLData() const;
    const std::string getCSVData() const;
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
    void getClassificationResults(CURL* curlHandle);
    void getQueryResults(CURL* curlHandle, char *query);

    void parseHierarchyAndRules(CURL* curlHandle);
    void processUserOntology(CURL* curlHandle);
    void processFuzzyProperties(CURL* curlHandle);
    void processUserRules(CURL* curlHandle);

    TextPort inXMLPort_;
    TextPort inCSVPort_;
    TextPort outPort_;

    std::string pXMLData_;
    std::string pCSVData_;
    std::string pTextDataOut_;

    StringProperty serverURLupdate_;

    ButtonProperty update_;

    static const std::string loggerCat_; ///< category used in logging
};

} //namespace
#endif // KNOWLEDGEWEBSERVICEPROCESSOR_H

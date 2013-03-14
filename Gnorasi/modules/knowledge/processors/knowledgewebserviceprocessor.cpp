#include "knowledgewebserviceprocessor.h"
#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/processors/processorwidgetfactory.h"

#include <stdio.h>

namespace voreen {
const std::string KnowledgeWebServiceProcessor::loggerCat_("voreen.KnowledgeWebServiceProcessor");

KnowledgeWebServiceProcessor::KnowledgeWebServiceProcessor()
    : Processor()
    , inOntologyPort_(Port::INPORT, "Input ontology", 0)
    , inGeoRulePort_(Port::INPORT, "Input geo rules", 0)
    , inFuzzyRulePort_(Port::INPORT, "Input fuzzy rules", 0)
    , inObjectMapPort_(Port::INPORT, "Input object map data", 0)
    , outPort_(Port::OUTPORT, "CSV output classification", 0)
    , serverURLupdate_("serverURLupdate_", "Web service endpoint", "http://localhost:9998/georulesservice")
    , update_("updateButton", "Send request")
    , pTextDataOut_("")
{
    // register ports and properties
    addPort(inOntologyPort_);
    addPort(inGeoRulePort_);
    addPort(inFuzzyRulePort_);
    addPort(inObjectMapPort_);
    addPort(outPort_);
    addProperty(serverURLupdate_);
    update_.onChange(CallMemberAction<KnowledgeWebServiceProcessor>(this, &KnowledgeWebServiceProcessor::updateView));
    addProperty(update_);
}

KnowledgeWebServiceProcessor::~KnowledgeWebServiceProcessor() {}

Processor* KnowledgeWebServiceProcessor::create() const {
    return new KnowledgeWebServiceProcessor();
}

void KnowledgeWebServiceProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();

    //if (inPort_.hasData()) {
    readData();
        //calculate();
    //}
    //setOutPortData();
}

void KnowledgeWebServiceProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");

    //pTextData_ = "";
    pOntologyData_ = "";
    pGeoRuleData_ = "";
    pFuzzyRuleData_ = "";

    pTextDataOut_ = "";

    Processor::deinitialize();
}

bool KnowledgeWebServiceProcessor::isReady() const {
/*    if (!inOntologyPort_.isConnected() || !inOntologyPort_.hasData())
        return false;

    if (!inObjectMapPort_.isConnected() || !inObjectMapPort_.hasData())
        return false;

    return true;*/
    return false;
}

bool KnowledgeWebServiceProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void KnowledgeWebServiceProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
        outPort_.setData(pTextDataOut_);
    //}
}

void KnowledgeWebServiceProcessor::setTextDataOut(std::string outTextData) {
    pTextDataOut_ = outTextData;
    setOutPortData();
}

const std::string KnowledgeWebServiceProcessor::getOntologyData() const {
    return pOntologyData_;
}

const std::string KnowledgeWebServiceProcessor::getGeoRuleData() const {
    return pGeoRuleData_;
}

const std::string KnowledgeWebServiceProcessor::getFuzzyRuleData() const {
    return pFuzzyRuleData_;
}


const std::string KnowledgeWebServiceProcessor::getObjectMapData() const {
    return pObjectMapData_;
}

std::string KnowledgeWebServiceProcessor::getProcessorInfo() const {
    return "Perform classification using a knowledge web service";
}

void KnowledgeWebServiceProcessor::readData() {

    if (inOntologyPort_.hasData() && inOntologyPort_.hasChanged())
        pOntologyData_ = inOntologyPort_.getData();

    if (inGeoRulePort_.hasData() && inGeoRulePort_.hasChanged())
        pGeoRuleData_ = inGeoRulePort_.getData();

    if (inFuzzyRulePort_.hasData() && inFuzzyRulePort_.hasChanged())
        pFuzzyRuleData_ = inFuzzyRulePort_.getData();

    if (inObjectMapPort_.hasData() && inObjectMapPort_.hasChanged())
        pObjectMapData_ = inObjectMapPort_.getData();

    //pTextData_ = inPort_.getData();
}

void KnowledgeWebServiceProcessor::updateView() {
    //if (getProcessorWidget()){
    //    getProcessorWidget()->updateFromProcessor();
    //}
    process();
}

void KnowledgeWebServiceProcessor::process() {
    tgtAssert(isInitialized(), "not initialized");

    readData();
    //updateView();

    int exitStatus = 0;
    std::string retData;

    //1. initialize connection GET
    //2. send ontology PUT
    //3. send region and features (with flag for region ids only or with regions ids with feature) PUT
    //4. send geo rules POST
    //5. send fuzzy rules POST
    //6. execute fuzzy rules GET
    //7. execute geo rules GET
    //8. get classification ids GET

    curl_global_init(CURL_GLOBAL_ALL);
    char* curlErrStr = new char[CURL_ERROR_SIZE];
    CURL* curlHandle = curl_easy_init();
    if(curlHandle) {
        curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, curlErrStr);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &KnowledgeWebServiceProcessor::getRequestData);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &retData);

        initConnection(curlHandle, "truncate");
        LINFO(retData);
        retData = "";

        sendOntology(curlHandle);
        LINFO(retData);
        retData = "";

        sendObjectMapData(curlHandle, "2100", "false");
        LINFO(retData);
        retData = "";

        getQueryResults(curlHandle, "SELECT ?regions WHERE {?regions rdf:type gno:Region}");

        setTextDataOut(retData);
        retData = "";

        /* Clean-up libcurl */
        curl_easy_cleanup(curlHandle);
    }
    else {
        exitStatus = 1;
        LERROR("libcurl failed initialization");
    }

    curl_global_cleanup();
    delete[] curlErrStr;

/*    if (!inPort_.hasData() ){
        pTextData_ = "";

        outPort_.setData(pTextDataOut_);
        if (getProcessorWidget()){
            getProcessorWidget()->updateFromProcessor();
        }
    }*/
}

size_t KnowledgeWebServiceProcessor::getRequestData(void *ptr, size_t size, size_t nmemb, std::string *stream){
    //stream->clear();
    //stream->reserve(size*nmemb);
    //stream->assign((char*)ptr, size*nmemb);

    stream->append((char*)ptr, size*nmemb);

    return size*nmemb;
}

char* KnowledgeWebServiceProcessor::composeURL(std::string target) {
    std::string baseUrl = serverURLupdate_.get().c_str();
    std::string urlString;
    urlString.append(baseUrl.append(target));
    char *url = new char[urlString.length()+1];
    if(url == NULL) {
        LERROR("Unable to allocate memory for URL");
        return 0;
    }
    urlString.copy(url, urlString.length());
    url[urlString.length()] = '\0';

    return url;
}

void KnowledgeWebServiceProcessor::initConnection(CURL* curlHandle, char *truncate) {
    char *url = composeURL("/init");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    char *truncateEncoded = curl_escape(truncate,0);

    char *postfields = new char[strlen(truncateEncoded)+100];
    sprintf(postfields,"truncate=%s", truncateEncoded);

    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, postfields);

    CURLcode curlErr = curl_easy_perform(curlHandle);
    if(curlErr) {
        LWARNING(curl_easy_strerror(curlErr));
    }

    //revert to HTTP GET
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, NULL);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);

    delete[] postfields;
    delete[] url;
    curl_free(truncateEncoded);
}

void KnowledgeWebServiceProcessor::sendOntology(CURL* curlHandle) {
    char *url = composeURL("/sendUserOntology");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);
    //wchar_t *temp;

    char *dataEncoded = curl_escape(getOntologyData().c_str(),0);
    char *postfields = new char[strlen(dataEncoded)+50];
    sprintf(postfields,"ontology=%s&context=", dataEncoded);

    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, postfields);

    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");

    CURLcode curlErr = curl_easy_perform(curlHandle);
    if(curlErr) {
        LWARNING(curl_easy_strerror(curlErr));
    }

    //revert to HTTP GET
    curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, NULL);
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, NULL);

    delete[] url;
    delete[] postfields;
    curl_free(dataEncoded);
}

void KnowledgeWebServiceProcessor::sendObjectMapData(CURL* curlHandle, char *srid, char *parseFeatures) {
    char *url = composeURL("/sendCSVDataString");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    char *urlEncoded = curl_escape(getObjectMapData().c_str(),0);
    char *postfields = new char[strlen(urlEncoded)+50];
    sprintf(postfields,"csvstring=%s&srid=%s&context=&parsefeatures=%s", urlEncoded, srid, parseFeatures);

    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, postfields);

    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");

    CURLcode curlErr = curl_easy_perform(curlHandle);
    if(curlErr) {
        LWARNING(curl_easy_strerror(curlErr));
    }

    //revert to HTTP GET
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, NULL);
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, NULL);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);

    delete[] url;
    delete[] postfields;
    curl_free(urlEncoded);
}

void KnowledgeWebServiceProcessor::sendFuzzyRules(CURL* curlHandle) {
    if (getFuzzyRuleData().compare("") != 0) {
        char *url = composeURL("/execFuzzyRules");
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);

        char *fuzzyRulesEncoded = curl_escape(getFuzzyRuleData().c_str(),0);

        char *postfields = new char[strlen(fuzzyRulesEncoded)+100];
        sprintf(postfields,"rules=%s&context=", fuzzyRulesEncoded);

        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, postfields);

        CURLcode curlErr = curl_easy_perform(curlHandle);
        if(curlErr) {
            LWARNING(curl_easy_strerror(curlErr));
        }

        //revert to HTTP GET
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, NULL);
        curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);

        delete[] postfields;
        delete[] url;
        curl_free(fuzzyRulesEncoded);
    }
}

void KnowledgeWebServiceProcessor::sendGeoRules(CURL* curlHandle) {
    if (getGeoRuleData().compare("") != 0) {
        char *url = composeURL("/ruleString");
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);

        char *geoRulesEncoded = curl_escape(getGeoRuleData().c_str(),0);

        char *postfields = new char[strlen(geoRulesEncoded)+100];
        sprintf(postfields,"rules=%s&context=", geoRulesEncoded);

        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, postfields);

        CURLcode curlErr = curl_easy_perform(curlHandle);
        if(curlErr) {
            LWARNING(curl_easy_strerror(curlErr));
        }

        //revert to HTTP GET
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, NULL);
        curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);

        delete[] postfields;
        delete[] url;
        curl_free(geoRulesEncoded);
    }
}

void KnowledgeWebServiceProcessor::getClassificationResults(CURL* curlHandle) {
    char *url = composeURL("/getResults");

     char *params = new char[strlen(url)+50];
    sprintf(params, "%s?context=", url);

    curl_easy_setopt(curlHandle, CURLOPT_URL, params);

    CURLcode curlErr = curl_easy_perform(curlHandle);
    if(curlErr) {
        LWARNING(curl_easy_strerror(curlErr));
    }

    delete[] url;
    delete[] params;
}

void KnowledgeWebServiceProcessor::getQueryResults(CURL* curlHandle, char *query) {
    char *url = composeURL("/getResultsQuery");

    char *queryEncoded = curl_escape(query,0);
    char *params = new char[strlen(queryEncoded)+strlen(url)+50];
    sprintf(params, "%s?query=%s", url, queryEncoded);

    curl_easy_setopt(curlHandle, CURLOPT_URL, params);

    CURLcode curlErr = curl_easy_perform(curlHandle);
    if(curlErr) {
        LWARNING(curl_easy_strerror(curlErr));
    }

    delete[] url;
    delete[] params;
    curl_free(queryEncoded);
}


}//namespace

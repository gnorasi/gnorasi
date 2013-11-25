#include "knowledgewebserviceprocessor.h"
#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/processors/processorwidgetfactory.h"

#include <stdio.h>

namespace voreen {
const std::string KnowledgeWebServiceProcessor::loggerCat_("voreen.KnowledgeWebServiceProcessor");

KnowledgeWebServiceProcessor::KnowledgeWebServiceProcessor()
    : Processor()
    , inXMLPort_(Port::INPORT, "Input XML user hierarchy", 0)
    , inCSVPort_(Port::INPORT, "Input CSV fuzzy data", 0)
    , outPort_(Port::OUTPORT, "CSV output classification", 0)
    , serverURLupdate_("serverURLupdate_", "Web service endpoint", "http://localhost:9998/georulesservice")
    , update_("updateButton", "Send request")
    , pTextDataOut_("")
{
    // register ports and properties
    addPort(inXMLPort_);
    addPort(inCSVPort_);

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

    pXMLData_ = "";
    pCSVData_ = "";

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

const std::string KnowledgeWebServiceProcessor::getXMLData() const {
    return pXMLData_;
}

const std::string KnowledgeWebServiceProcessor::getCSVData() const {
    return pCSVData_;
}


std::string KnowledgeWebServiceProcessor::getProcessorInfo() const {
    return "Perform classification using a knowledge web service";
}

void KnowledgeWebServiceProcessor::readData() {

    if (inXMLPort_.hasData() && inXMLPort_.hasChanged())
        pXMLData_ = inXMLPort_.getData();

    if (inCSVPort_.hasData() && inCSVPort_.hasChanged())
        pCSVData_ = inCSVPort_.getData();

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

    /*
    1. initialize connection, GET initConnection
    2. parse and separate XML input to ontology and rules, POST parseHierarchyAndRules
    3. add the ontology to the classification service, GET processUserOntology
    4. add the fuzzy properties data from CSV to the service, POST processFuzzyProperties
    5. execute the rules on the data, GET processUserRules
    6. get classification ids, GET getClassificationResults
    */

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


        parseHierarchyAndRules(curlHandle);
        //LINFO(retData);
        retData = "";

        processUserOntology(curlHandle);
        //LINFO(retData);
        retData = "";

        processFuzzyProperties(curlHandle);
        //LINFO(retData);
        retData = "";

        processUserRules(curlHandle);
        //LINFO(retData);
        retData = "";

        getClassificationResults(curlHandle);

        //single call classification
        //performClassification(curlHandle);

        int lines = countNewlines(retData);
        if (lines < 2)
            LERROR("Classification results not obtained!");
        else {
            char *info = new char[100];
            std::sprintf(info, "%d objects were classified", lines-1);
            //std::string info = std::to_string((long double)lines-1);
            //LINFO(retData);
            LINFO(info);
        }

        setTextDataOut(retData);
        retData = "";

        closeConnection(curlHandle);

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

void KnowledgeWebServiceProcessor::closeConnection(CURL* curlHandle) {
    char *url = composeURL("/close");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    CURLcode curlErr = curl_easy_perform(curlHandle);
    if(curlErr) {
        LWARNING(curl_easy_strerror(curlErr));
    }

    //revert to HTTP GET
    //curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, NULL);
    //curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);

    delete[] url;
}

void KnowledgeWebServiceProcessor::parseHierarchyAndRules(CURL *curlHandle) {
    char *url = composeURL("/parseGnorasiHierarchyAndRules");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    char *xmlInputEncoded = curl_escape(getXMLData().c_str(),0);

    char *postfields = new char[strlen(xmlInputEncoded)+100];
    sprintf(postfields,"contents=%s&context=", xmlInputEncoded);

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
    curl_free(xmlInputEncoded);
}

void KnowledgeWebServiceProcessor::processUserOntology(CURL *curlHandle) {
    char *url = composeURL("/processGnorasiUserOntology");

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

void KnowledgeWebServiceProcessor::processFuzzyProperties(CURL *curlHandle) {
    char *url = composeURL("/processGnorasiFuzzyProperties");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    char *csvInputEncoded = curl_escape(getCSVData().c_str(),0);

    char *postfields = new char[strlen(csvInputEncoded)+100];
    sprintf(postfields,"contents=%s&context=", csvInputEncoded);

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
    curl_free(csvInputEncoded);
}

void KnowledgeWebServiceProcessor::processUserRules(CURL *curlHandle) {
    char *url = composeURL("/processGnorasiUserRules");

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

void KnowledgeWebServiceProcessor::performClassification(CURL *curlHandle) {
    char *url = composeURL("/performClassification");
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);

    char *csvInputEncoded = curl_escape(getCSVData().c_str(),0);
    char *xmlInputEncoded = curl_escape(getXMLData().c_str(),0);

    char *postfields = new char[strlen(csvInputEncoded)+strlen(xmlInputEncoded)+100];
    sprintf(postfields,"xmlcontents=%s&csvcontents=%s&context=", xmlInputEncoded, csvInputEncoded);

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
    curl_free(csvInputEncoded);
    curl_free(xmlInputEncoded);
}


int KnowledgeWebServiceProcessor::countNewlines(std::string str) {
    int counter = 0;
    std::size_t found = 0;

    while ((found = str.find("\n", found+1)) != std::string::npos) {
        counter++;
    }

    return counter;
}

}//namespace

/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

#include "retrieveclassificationdatawidget.h"
#include "voreen/qt/voreenapplicationqt.h"


#include <QGridLayout>
#include <QMainWindow>
#include <QLabel>
#include <QtCore/QVector>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QObject>
#include <QUrl>
#include <QThread>
#include <QtXml>
#include <QDomDocument>


namespace voreen {

const std::string RetrieveClassificationDataWidget::loggerCat_("voreen.RetrieveClassificationDataWidget");

RetrieveClassificationDataWidget::RetrieveClassificationDataWidget(QWidget* parent, RetrieveClassificationDataProcessor* retrieveClassificationDataProcessor)
    : QProcessorWidget(retrieveClassificationDataProcessor, parent)
{
    tgtAssert(retrieveClassificationDataProcessor, "No RetrieveClassificationDataProcessor processor");

    setWindowTitle(QString::fromStdString(retrieveClassificationDataProcessor->getName()));
    resize(400, 280);
    setPosition(20,20);
}

RetrieveClassificationDataWidget::~RetrieveClassificationDataWidget() {
    delete editor_;
}

void RetrieveClassificationDataWidget::initialize() {
    QProcessorWidget::initialize();

    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    editor_ = new QPlainTextEdit();
    editor_->appendPlainText("init");
    layout->addWidget(editor_);

    setLayout(layout);

    hide();

    initialized_ = true;
}

void RetrieveClassificationDataWidget::updateFromProcessor() {
    RetrieveClassificationDataProcessor* retrieveClassificationDataProcessor = dynamic_cast<RetrieveClassificationDataProcessor*>(processor_);

    editor_->clear();
    retrieveClassificationDataProcessor->setTextDataOut("");
    if ( !retrieveClassificationDataProcessor->getTextData().empty() ) {

        std::string result =  invokeWebService(retrieveClassificationDataProcessor->getTextData().c_str());

        editor_->appendPlainText("-------- RESULT -------");
        editor_->appendPlainText(result.c_str());
        retrieveClassificationDataProcessor->setTextDataOut(result);
    }
}

std::string RetrieveClassificationDataWidget::invokeWebService(std::string input) {
    //QString serverQuery = "http://160.40.50.66:2020/gnorasi";

    if (input.compare("Data ready") == 0) {
        RetrieveClassificationDataProcessor* retrieveClassificationDataProcessor = dynamic_cast<RetrieveClassificationDataProcessor*>(processor_);

        StringProperty* serverURLquery_ = static_cast<StringProperty*>(retrieveClassificationDataProcessor->getProperty("serverURLquery_"));

        QNetworkAccessManager *manager = new QNetworkAccessManager();
        QNetworkRequest request;
        QNetworkReply *reply;
        QEventLoop loop;
        QUrl url;

        std::string serverQuery = serverURLquery_->get().c_str();

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QString prefix = "PREFIX gno: <http://www.gnorasi.gr/ontology#>\n" \
                        "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n" \
                        "PREFIX owl: <http://www.w3.org/2002/07/owl#>\n" \
                        "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n" \
                        "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";

        LINFO("All data sent");

        //issue query to get regions
        QString query = "SELECT ?regionID ?objectClassID " \
                        "WHERE { " \
                            "?region gno:depictsObject ?y ; " \
                            "gno:hasID ?regionID . ?y gno:hasObjectClassID ?objectClassID . " \
                        "}";

        url.clear();
        url.setUrl(QString::fromStdString(serverQuery));
        url.addQueryItem("query",prefix+query);
        url.addQueryItem("output", "xml");

        request.setUrl(url);

        reply = manager->get(request);
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            LWARNING("Error in connection while retrieving data");
            return "Error in connection while retrieving data";
         }

        QString replyBody = reply->readAll();

        QString csv = parseResults(replyBody);

        LINFO("Classification results obtained");

        url.clear();
        reply->deleteLater();
        manager->deleteLater();

        return csv.toStdString();
    }

    return "Classification data retrieval failed";
}


QString RetrieveClassificationDataWidget::parseResults(QString resultString) {
    QDomDocument doc("results");

    doc.setContent(resultString);

    QString ns = "http://www.w3.org/2005/sparql-results#";

    QDomNodeList nodeList = doc.elementsByTagName("sparql").at(0).toElement().elementsByTagName("head").at(0).toElement().elementsByTagName("variable");

    //we need at least 2 values, region ID and Class ID
    if (nodeList.size() < 2)
        return 0;

    QString csvHeader = "ID;CLASS";
    QString csvValues = "";

    QString* variables = new QString[nodeList.size()];
    for (int i=0;i<nodeList.size();i++) {
        variables[i] = nodeList.at(i).attributes().namedItem("name").nodeValue();
        if (i>1)
            csvHeader += ";"+variables[i];
    }
    csvHeader += "\n";

    nodeList = doc.elementsByTagName("sparql").at(0).toElement().elementsByTagName("results").at(0).toElement().elementsByTagName("result");
    for (int i=0;i<nodeList.size();i++) {
        QDomNodeList bindings = nodeList.at(i).toElement().elementsByTagName("binding");
        //position 0 is the ID
        //position 1 is the class ID
        //the rest are undefined
        QDomNodeList IDs = bindings.at(0).toElement().elementsByTagName("literal");
        if (IDs.size() > 0) {
            QString ID = IDs.at(0).toElement().text();
            csvValues += ID;
        }

        QDomNodeList classIDs = bindings.at(1).toElement().elementsByTagName("literal");
        if (classIDs.size() > 0) {
            QString classID = classIDs.at(0).toElement().text();
            csvValues += ";"+classID;
        }

        if (bindings.size() > 2) {
            QString* otherValues = new QString[bindings.size()-2];
            for (int j=0;j<bindings.size()-2;j++) {
                QDomNodeList otherList = bindings.at(j+2).toElement().elementsByTagName("literal");
                if (otherList.size() > 0) {
                    otherValues[j] = otherList.at(0).toElement().text();
                }
                else {
                    otherList = bindings.at(j+2).toElement().elementsByTagName("uri");
                    if (otherList.size() > 0) {
                        otherValues[j] = otherList.at(0).toElement().text();
                    }
                }
                csvValues += ";"+otherValues[j];
            }
            delete[] otherValues;
        }
        csvValues += "\n";
    }

    delete[] variables;

    return csvHeader+csvValues;
}


} //namespace voreen


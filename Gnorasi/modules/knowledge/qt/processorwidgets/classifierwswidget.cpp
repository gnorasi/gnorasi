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

#include "classifierwswidget.h"
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

const std::string ClassifierWSWidget::loggerCat_("voreen.ClassifierWSWidget");

ClassifierWSWidget::ClassifierWSWidget(QWidget* parent, ClassifierWSProcessor* classifierWSProcessor)
    : QProcessorWidget(classifierWSProcessor, parent)
{
    tgtAssert(classifierWSProcessor, "No ClassifierWSProcessor processor");

    setWindowTitle(QString::fromStdString(classifierWSProcessor->getName()));
    resize(400, 280);
    setPosition(20,20);
}

ClassifierWSWidget::~ClassifierWSWidget() {
    delete editor_;
}

void ClassifierWSWidget::initialize() {
    QProcessorWidget::initialize();

    //ClassifierWSProcessor* classifierWSProcessor = dynamic_cast<ClassifierWSProcessor*>(processor_);
    //QString s = classifierWSProcessor->getName().c_str();
    //QLabel* label = new QLabel();
    //label->setText(s);
    ////connect(plugin_, SIGNAL(modified()), this, SIGNAL(modified()));

    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    //layout->addWidget(label);

    editor_ = new QPlainTextEdit();
    editor_->appendPlainText("init");
    layout->addWidget(editor_);

    setLayout(layout);

    //show();
    hide();


    initialized_ = true;

}

void ClassifierWSWidget::updateFromProcessor() {
    ClassifierWSProcessor* classifierWSProcessor = dynamic_cast<ClassifierWSProcessor*>(processor_);

    editor_->clear();
    classifierWSProcessor->setTextDataOut("");
    if ( !classifierWSProcessor->getTextData().empty() ) {

        //editor_->appendPlainText( classifierWSProcessor->getTextData().c_str() );

        std::string result =  invokeWebService(classifierWSProcessor->getTextData().c_str());
        editor_->appendPlainText("-------- RESULT -------");
        editor_->appendPlainText(result.c_str());
        classifierWSProcessor->setTextDataOut(result);
    }


}

std::string ClassifierWSWidget::invokeWebService(std::string input) {
    //QString serverUpdate = "http://160.40.50.66:2020/update/gnorasi";
    //QString serverQuery = "http://160.40.50.66:2020/gnorasi";

    ClassifierWSProcessor* classifierWSProcessor = dynamic_cast<ClassifierWSProcessor*>(processor_);

    StringProperty* serverURLUpdate_ = static_cast<StringProperty*>(classifierWSProcessor->getProperty("serverURLupdate_"));
    StringProperty* serverURLquery_ = static_cast<StringProperty*>(classifierWSProcessor->getProperty("serverURLquery_"));

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    QNetworkReply *reply;
    QEventLoop loop;
    QUrl url;
    QUrl params;

    std::string serverUpdate = serverURLUpdate_->get().c_str();
    std::string serverQuery = serverURLquery_->get().c_str();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString prefix = "PREFIX gno: <http://www.gnorasi.gr/ontology#>\n" \
                    "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n" \
                    "PREFIX owl: <http://www.w3.org/2002/07/owl#>\n" \
                    "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n" \
                    "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";

    QString error;
    int lineNum,col;
    QDomDocument doc = readMappings("../data/mapping/mappings.xml", &error, &lineNum, &col);

    if (!doc.isDocument()) {
        QString ret = "Error in file \"mappings.xml\" in line "+ QString::number(lineNum) + ", column " + QString::number(col) + ": " + error;
        return ret.toStdString();
    }

    QString qinput = QString::fromStdString(input);
    QTextStream stream(&qinput);
    QString line;

    //read the header line
    QStringList headers = stream.readLine().split(";");

    //parse the header line in a vector
    QVector<QVector<QString> > headersList = parseHeaders(headers);

    //read the rest of the lines with the data
    long count = 0;
    QString updateString = "";\

    LINFO("Sending data to knowledge web service...");

    while (!stream.atEnd()) {
        line = stream.readLine();
        count++;

        QStringList parts = line.split(";");
        if (parts.size() != headersList.size()) {
            LWARNING("Line "<< count << "contains " << parts.size() << " values. Should be " << headersList.size());
            continue;
        }

        QString id = "";
        for (int i=0;i<parts.size();i++) {
            QString part = parts[i];

            QVector<QString> headerAtI = headersList[i];

            if (headerAtI[0] == "ID") {
                id = part;
                updateString += "gno:region_"+id+" rdf:type gno:Region . \n";
                updateString += "gno:region_"+id+" gno:hasID \""+id+"\"^^xsd:int . \n";
            }
            else {
                QString value = part;
                updateString += "gno:region_"+id+" gno:hasFeature gno:feature_"+id+"_"+QString::number(i)+" . \n";

                //for the demo the 1st position for each CSV header entry (headerAtI[0]) has the STATS keyword.
                QDomNodeList nodeList = doc.elementsByTagName("feature").at(0).toElement().elementsByTagName(headerAtI[0]); //STATS
                QString featureType = nodeList.at(0).attributes().namedItem("type").nodeValue();
                QString featureProperty = nodeList.at(0).attributes().namedItem("property").nodeValue();
                updateString += "gno:feature_"+id+"_"+QString::number(i)+" gno:"+featureProperty+" gno:"+featureType+" . \n";

                for (int j=1;j<headerAtI.size();j++) {
                    QDomNode node = nodeList.at(0).toElement().elementsByTagName(headerAtI[j]).at(0);
                    QString property = node.attributes().namedItem("property").nodeValue();
                    QString propertyType = node.toElement().text();

                    updateString += "gno:feature_"+id+"_"+QString::number(i)+" gno:"+property+" gno:"+propertyType+" . \n";
                }
                updateString += "gno:feature_"+id+"_"+QString::number(i)+" gno:featureHasValue \""+value+"\"^^xsd:float . \n";
            }
        }

        int batch = 100;
        if ((count-1)%batch == 0) {
            url.setUrl(QString::fromStdString(serverUpdate));
            params.addQueryItem("request",prefix+"INSERT DATA {"+updateString+"}");

            request.setUrl(url);

            reply = manager->post(request, params.encodedQuery());

            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            if (reply->error() != QNetworkReply::NoError) {
                LWARNING("Error in connection");
                return "Error in connection";
             }

            reply->close();
            url.clear();
            params.clear();

            updateString = "";
        }
    }

    //insert the last entries if they exist
    if (updateString != "") {
        url.setUrl(QString::fromStdString(serverUpdate));
        params.addQueryItem("request",prefix+"INSERT DATA {"+updateString+"}");

        request.setUrl(url);

        reply = manager->post(request, params.encodedQuery());

        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            LWARNING("Error in connection");
            return "Error in connection";
         }

        reply->close();
        url.clear();
        params.clear();

        updateString = "";
    }
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
        LWARNING("Error in connection");
        return "Error in connection";
     }

    QString replyBody = reply->readAll();

    QString csv = parseResults(replyBody);

    LINFO("Classification results obtained");

    url.clear();
    reply->deleteLater();
    manager->deleteLater();

    return csv.toStdString();
}


QVector<QVector<QString> > ClassifierWSWidget::parseHeaders(QStringList headers) {

    QVector<QVector<QString> > h(headers.size());

    for (int i=0; i<headers.size();i++) {
        QString headerValue = headers.at(i);
        QStringList headerValueList = headerValue.split("::");
        for (int j=0;j<headerValueList.size();j++) {
            QString val = headerValueList[j];
            h[i].append(val);
        }
    }

    return h;
}

QDomDocument ClassifierWSWidget::readMappings(QString mapFile, QString* error, int* line, int *col) {
    QDomDocument doc("mappings");
    QFile file(mapFile);

    file.open(QIODevice::ReadOnly);

    doc.setContent(&file, error, line, col);

    file.close();

    return doc;
}

QString ClassifierWSWidget::parseResults(QString xml2) {
    QDomDocument doc("results");

    doc.setContent(xml2);

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


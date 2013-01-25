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

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    QNetworkReply *reply;
    QEventLoop loop;
    QUrl url;
    QUrl params;

    std::string serverUpdate = serverURLUpdate_->get().c_str();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString prefix = "PREFIX gno: <http://www.gnorasi.gr/ontology#>\n" \
                    "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n" \
                    "PREFIX owl: <http://www.w3.org/2002/07/owl#>\n" \
                    "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n" \
                    "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";

    QString error;
    int lineNum,col;
    //QDomDocument doc = readMappings("../data/mapping/mappings.xml", &error, &lineNum, &col);
    QString mapFile = qApp->applicationDirPath() + "/mappings.xml";
    LINFO(mapFile.toStdString());
    QDomDocument doc = readMappings(mapFile, &error, &lineNum, &col);

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

    LINFO("Clearing old data...");
    clearData(serverUpdate, headersList, doc);

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

            //

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

        int batch = 50;
        if ((count-1)%batch == 0) {
            url.setUrl(QString::fromStdString(serverUpdate));
            params.addQueryItem("request",prefix+"INSERT DATA {"+updateString+"}");
            //params.addQueryItem("request",prefix+"INSERT DATA { GRAPH <"+context+"> {"+updateString+"} }");

            //LINFO(updateString.toStdString());

            request.setUrl(url);

            reply = manager->post(request, params.encodedQuery());

            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            if (reply->error() != QNetworkReply::NoError) {
                LWARNING("Error in connection for sending data");
                return "Error in connection for sending data";
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
        //params.addQueryItem("request",prefix+"INSERT DATA { GRAPH <"+context+"> {"+updateString+"} }");

        request.setUrl(url);

        reply = manager->post(request, params.encodedQuery());

        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            LWARNING("Error in connection for sending data");
            return "Error in connection for sending data";
         }

        reply->close();
        url.clear();
        params.clear();

        updateString = "";
    }
    LINFO("All data sent");

    url.clear();
    reply->deleteLater();
    manager->deleteLater();

    return "Data ready";
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
    if (file.exists() == false) {
        LWARNING(" Mappings file does not exist");
    }

    doc.setContent(&file, error, line, col);

    file.close();

    return doc;
}

void ClassifierWSWidget::clearData(std::string serverUpdate, QVector<QVector<QString> > headersList, QDomDocument doc) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    QNetworkReply *reply;
    QEventLoop loop;
    QUrl url;
    QUrl params;

    QString prefix = "PREFIX gno: <http://www.gnorasi.gr/ontology#>\n" \
                    "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n" \
                    "PREFIX owl: <http://www.w3.org/2002/07/owl#>\n" \
                    "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n" \
                    "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";

    //clearing old data --
    for (int i=0;i<headersList.size();i++) {
        QString deleteString = "";
        QVector<QString> headerAtI = headersList[i];
        deleteString = "?x rdf:type gno:Region . ?x gno:hasID ?id . ?x gno:hasFeature ?feat . ";
        if (headerAtI[0] != "ID") {
            QDomNodeList nodeList = doc.elementsByTagName("feature").at(0).toElement().elementsByTagName(headerAtI[0]); //STATS
            QString featureType = nodeList.at(0).attributes().namedItem("type").nodeValue();
            QString featureProperty = nodeList.at(0).attributes().namedItem("property").nodeValue();
            deleteString += "?feat gno:"+featureProperty+" gno:"+featureType+" . \n";
            for (int j=1;j<headerAtI.size();j++) {
                QDomNode node = nodeList.at(0).toElement().elementsByTagName(headerAtI[j]).at(0);
                QString property = node.attributes().namedItem("property").nodeValue();
                QString propertyType = node.toElement().text();

                deleteString += "?feat gno:"+property+" gno:"+propertyType+" . \n";
            }
            deleteString += "?feat gno:featureHasValue ?val . \n";
        }
        url.setUrl(QString::fromStdString(serverUpdate));
        params.addQueryItem("request",prefix + "DELETE {"+deleteString+"} WHERE {"+deleteString+"}");
        request.setUrl(url);
        reply = manager->post(request, params.encodedQuery());
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        if (reply->error() != QNetworkReply::NoError) {
            LWARNING("Error in connection for DELETE");
        }
        reply->close();
        url.clear();
        params.clear();
    }

    QString deleteString = "DELETE {?x gno:depictsObject ?obj} WHERE {?x gno:depictsObject ?obj}";
    url.setUrl(QString::fromStdString(serverUpdate));
    params.addQueryItem("request",prefix + deleteString);
    request.setUrl(url);
    reply = manager->post(request, params.encodedQuery());
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        LWARNING("Error in connection for DELETE");
    }
    reply->close();
    url.clear();
    params.clear();
    //--
}


} //namespace voreen


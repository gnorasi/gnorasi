/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "classifierwswidget.h"
#include "voreen/qt/voreenapplicationqt.h"


#include <QGridLayout>
#include <QMainWindow>
#include <QLabel>

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

        editor_->appendPlainText( classifierWSProcessor->getTextData().c_str() );

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

    std::string serverUpdate = serverURLUpdate_->get().c_str();
    std::string serverQuery = serverURLquery_->get().c_str();

    QString prefix = "PREFIX gno: <http://www.gnorasi.gr/ontologies/gnorasi.owl#>\n" \
                    "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n" \
                    "PREFIX owl: <http://www.w3.org/2002/07/owl#>\n" \
                    "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n" \
                    "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    QNetworkReply *reply;
    QEventLoop loop;
    QUrl url;
    QUrl params;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString error;
    int line, col;

    // read XML string
    QDomDocument doc("instances");
    if (!doc.setContent(QString::fromStdString(input), &error, &line, &col)) {
        // Voreen style exit
        return "Input Incompatible !!";
    }

    // get MSAVI values
    QDomNodeList nodeList = doc.elementsByTagName("regions").at(0).toElement().elementsByTagName("region");
    if (nodeList.count() > 0) {
        for (int i=0;i<nodeList.count();i++) {
            int id = nodeList.at(i).attributes().namedItem("id").nodeValue().toInt();
            QDomNodeList msaviNode = nodeList.at(i).toElement().elementsByTagName("msavi");
            float msaviVal = msaviNode.at(0).toElement().text().toFloat();

            QString ins1 = "gno:objectInstance_"+QString::number(id)+" rdf:type gno:ImageRegion . " \
                    "gno:objectInstanceFeature_"+QString::number(id)+" rdf:type gno:Spectral . " \
                    "gno:objectInstanceFeatureDescriptor_"+QString::number(id)+" rdf:type gno:MSAVI . " \
                    "gno:objectInstanceFeatureDescriptorValue_"+QString::number(id)+" rdf:type gno:SingleValue . " \
                    "gno:objectInstance_"+QString::number(id)+" gno:hasFeature gno:objectInstanceFeature_"+QString::number(id)+" . " \
                    "gno:objectInstanceFeature_"+QString::number(id)+" gno:hasFeatureDescriptor gno:objectInstanceFeatureDescriptor_"+QString::number(id)+" . " \
                    "gno:objectInstanceFeatureDescriptor_"+QString::number(id)+" gno:hasFeatureDescriptorValue gno:objectInstanceFeatureDescriptorValue_"+QString::number(id)+" . " \
                    "gno:objectInstanceFeatureDescriptorValue_"+QString::number(id)+" gno:hasValueFloat \""+QString::number(msaviVal)+"\"^^xsd:float . ";

            //url.setUrl(serverURLupdate_.getData());
            url.setUrl(QString::fromStdString(serverUpdate));

            params.addQueryItem("request",prefix+"INSERT DATA {"+ins1+"}");

            request.setUrl(url);

            reply = manager->post(request, params.encodedQuery());

            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            reply->close();
            url.clear();
            params.clear();
        }
    }

    //QString query = "SELECT ?region ?type WHERE {?region gno:depictsObject ?y . ?y rdf:type ?type}";
    QString query = "SELECT ?region ?y WHERE {?region gno:depictsObject ?y }";

    url.clear();
    url.setUrl(QString::fromStdString(serverQuery));
    url.addQueryItem("query",prefix+query);
    url.addQueryItem("output", "xml");

    request.setUrl(url);

    reply = manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //QString replyBody = reply->readAll();
    QByteArray replyBody = reply->readAll();
    QString results = QString(replyBody);

    reply->close();
    url.clear();

    // delete on server side //
    //clear data from XML
    QString deleteClause = "DELETE {" \
                            "?reg rdf:type gno:ImageRegion . " \
                            "?feat rdf:type gno:Spectral . " \
                            "?featdesc rdf:type gno:MSAVI . " \
                            "?featdescv rdf:type gno:SingleValue . " \
                            "?reg gno:hasFeature ?feat . " \
                            "?feat gno:hasFeatureDescriptor ?featdesc . " \
                            "?featdesc gno:hasFeatureDescriptorValue ?featdescv . " \
                            "?featdescv gno:hasValueFloat ?val . " \
                    "} WHERE {" \
                            "?reg rdf:type gno:ImageRegion . " \
                            "?feat rdf:type gno:Spectral . " \
                            "?featdesc rdf:type gno:MSAVI . " \
                            "?featdescv rdf:type gno:SingleValue . " \
                            "?reg gno:hasFeature ?feat . " \
                            "?feat gno:hasFeatureDescriptor ?featdesc . " \
                            "?featdesc gno:hasFeatureDescriptorValue ?featdescv . " \
                            "?featdescv gno:hasValueFloat ?val . " \
                    "}";

    url.clear();
    url.setUrl(QString::fromStdString(serverUpdate));
    params.clear();
    params.addQueryItem("request",prefix+deleteClause);

    request.setUrl(url);

    reply = manager->post(request, params.encodedQuery());
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    reply->deleteLater();
    manager->deleteLater();

    return results.toStdString();
}

} //namespace voreen


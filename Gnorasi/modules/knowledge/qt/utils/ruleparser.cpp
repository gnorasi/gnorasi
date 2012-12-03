#include "ruleparser.h"

#include "ruleutility.h"
#include "../models/rule.h"
#include "../models/ruleitem.h"

#include <QFile>
#include <QDomElement>

#define KEY_NAMESPACEXMLNS      "nsxmlns"
#define KEY_NAMESPACEXMLBASE    "nsxmlbase"

using namespace voreen;

RuleParser::RuleParser(QObject *parent) :
    QObject(parent)
{
}

void RuleParser::parseFile(const QString &filePath, QString *error){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly )){
        if(error)
            *error = tr("Could not open the file");
        return;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
        if(error)
            *error = tr("Could not set the content to the dom document");
    }

    if(!validateNamespaces(doc)){
        if(error)
            *error = tr("Namespace miss match could not continue..");
        return;
    }

    RuleUtility *ru                                 = RuleUtility::instance();
    ru->clear();

    QDomNode rulesNode                              = doc.lastChild();
    if(rulesNode.isElement()){
        //
        QDomElement rulesElement = rulesNode.toElement();
        QString namespaceXmlBase                    = rulesElement.attribute(QString::fromAscii(KEY_NAMESPACEXMLBASE));
        QString namespaceXmlns                      = rulesElement.attribute(QString::fromAscii(KEY_NAMESPACEXMLNS));

        ru->setNamespaces(namespaceXmlns,namespaceXmlBase);

        QDomNode ruleNode = rulesNode.firstChild();
        while(!ruleNode.isNull()){
            //
            if(ruleNode.isElement()){
                //
                QDomElement ruleElement             = ruleNode.toElement();
                QString rid                         = ruleElement.attribute("id");

                Rule *pRule                         = ru->createRule(rid);

                // parse the body part
                QDomNode bodyNode                   = ruleNode.firstChild();
                QDomNode ruleItemNode               = bodyNode.firstChild();

                while(!ruleItemNode.isNull()){
                    //
                    if(ruleItemNode.isElement()){

                        QDomElement ruleItemElement = ruleItemNode.toElement();

                        QString regid               = ruleItemElement.attribute("id");
                        QString headclasstext       = ruleItemElement.attribute("class");
                        QString propertutext        = ruleItemElement.attribute("property");
                        QString leveltext           = ruleItemElement.attribute("level");

                        QVector<QVariant> data;
                        data << regid << headclasstext << propertutext << leveltext;

                        RuleItem *pRuleItem         = new RuleItem(data);

                        pRule->appendRuleItem(pRuleItem);
                    }

                    ruleItemNode = ruleItemNode.nextSibling();
                }

                // parse the head part
                QDomNode headNode                   = ruleNode.lastChild();
                ruleItemNode                        = headNode.firstChild();
                while(!ruleItemNode.isNull()){
                    //
                    if(ruleItemNode.isElement()){
                        //
                        QDomElement ruleItemElement = ruleItemNode.toElement();

                        QString headclasstext       = ruleItemElement.attribute("class");
                        double confidence           = ruleItemElement.attribute("confidence").toDouble();

                        pRule->setHeadClass(headclasstext);
                        pRule->setConfidence(confidence);
                    }

                    ruleItemNode = ruleItemNode.nextSibling();
                }
            }

            ruleNode = ruleNode.nextSibling();
        }
    }
}

//
bool RuleParser::validateNamespaces(const QDomDocument &doc){
    QDomNode rulesNode              = doc.lastChild();
    if(rulesNode.isElement()){
        //
        QDomElement rulesElement    = rulesNode.toElement();
        QString namespaceXmlBase    = rulesElement.attribute(QString::fromAscii(KEY_NAMESPACEXMLBASE));
        QString namespaceXmlns      = rulesElement.attribute(QString::fromAscii(KEY_NAMESPACEXMLNS));

        RuleUtility *ru = RuleUtility::instance();

        QString _cnamespaceXmlBase  = ru->namespaceXmlBase();
        QString _cnamespaceXmlns    = ru->namespaceXmlns();

        if(!_cnamespaceXmlBase.compare(namespaceXmlBase) && !_cnamespaceXmlns.compare(namespaceXmlns))
            return true;
    }

    return false;
}

#include "rulewriter.h"
#include "../models/rule.h"
#include "../models/ruleitem.h"
#include "ruleutility.h"

#include <QDebug>

using namespace voreen;

#define RULES_TAG               "rules"

#define NS_KEY                  "ns"
#define NS_VALUE                "http://rdf.opensahara.com/search#"

#define PREFIX_KEY              "prefix"
#define PREFIX_VALUE            "search"

#define RULE_TAG                "rule"
#define ID_KEY                  "id"
#define BODY_KEY                "body"
#define REGION_TAG              "region"
#define CLASS_KEY               "class"
#define PROPERTY_KEY            "property"
#define LEVEL_KEY               "level"
#define HEAD_TAG                "head"
#define CONFIDENCE_KEY          "confidence"

#define KEY_NAMESPACEXMLNS      "nsxmlns"
#define KEY_NAMESPACEXMLBASE    "nsxmlbase"

//
RuleWriter::RuleWriter(QObject *parent) :
    helperCounter(0), QObject(parent)
{
}

//
QString RuleWriter::docToText(){
  return doc.toString(4);
}

//
void RuleWriter::createDocument(){
  helperCounter = 0;

  doc = QDomDocument();

  QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
  doc.appendChild(xmlDeclaration);

  rootElement = doc.createElement(QString::fromAscii(RULES_TAG));
  rootElement.setAttribute(QString::fromAscii(NS_KEY),QString::fromAscii(NS_VALUE));
  rootElement.setAttribute(QString::fromAscii(PREFIX_KEY),QString::fromAscii(PREFIX_VALUE));

  doc.appendChild(rootElement);
}

void RuleWriter::appendData(){
    RuleUtility *ru = RuleUtility::instance();
    QList<Rule*> list = ru->ruleList();
    QList<Rule*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        Rule *pRule = *i;

        QDomElement ruleElement = doc.createElement(QString::fromAscii(RULE_TAG));
        ruleElement.setAttribute(QString::fromAscii(ID_KEY),pRule->id());

        QDomElement bodyElement = doc.createElement(QString::fromAscii(BODY_KEY));
        QList<RuleItem*> ritemList = pRule->ruleItemList();
        QList<RuleItem*>::const_iterator j;
        for(j = ritemList.constBegin(); j != ritemList.constEnd(); j++){
            RuleItem *rItem = *j;

            QDomElement regionElement =  doc.createElement(QString::fromAscii(REGION_TAG));

            regionElement.setAttribute(QString::fromAscii(ID_KEY),rItem->id());
            regionElement.setAttribute(QString::fromAscii(CLASS_KEY),rItem->headClass());
            regionElement.setAttribute(QString::fromAscii(PROPERTY_KEY),rItem->property());
            regionElement.setAttribute(QString::fromAscii(LEVEL_KEY),QString::number(rItem->segmentationLevel()));

            bodyElement.appendChild(regionElement);
        }

        ruleElement.appendChild(bodyElement);

        QDomElement headElement = doc.createElement(QString::fromAscii(HEAD_TAG));

        QDomElement regionOnHeadElement = doc.createElement(REGION_TAG);

        regionOnHeadElement.setAttribute(QString::fromAscii(CLASS_KEY),pRule->headClass());
        regionOnHeadElement.setAttribute(QString::fromAscii(CONFIDENCE_KEY),QString::number(pRule->confidence(),'f',2));

        headElement.appendChild(regionOnHeadElement);

        ruleElement.appendChild(headElement);

        rootElement.appendChild(ruleElement);
    }
}

//!
void RuleWriter::setNamespaces(const QString &nsXmlns, const QString &nsXmlBase){
    QDomNode rulesNode = doc.lastChild();
    if(rulesNode.isElement()){
        //
        QDomElement rulesElement = rulesNode.toElement();

        rulesElement.setAttribute(QString::fromAscii(KEY_NAMESPACEXMLNS),nsXmlns);
        rulesElement.setAttribute(QString::fromAscii(KEY_NAMESPACEXMLBASE),nsXmlBase);

    }else
        qDebug() << "rdf list is empty or no rules node found ..";
}

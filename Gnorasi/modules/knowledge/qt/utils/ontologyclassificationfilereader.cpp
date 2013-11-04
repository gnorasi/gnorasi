#include "ontologyclassificationfilereader.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

#include "ontologyclassificationmanager.h"
#include "../models/ontologyclass.h"
#include "../fuzzy/fuzzyrule.h"
#include "../fuzzy/fuzzyrulemanager.h"
#include "../fuzzy/fuzzyfunction.h"
#include "../fuzzy/fuzzyfunctionmanager.h"
#include "../fuzzy/fuzzyfunctionfactory.h"
#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"

OntologyClassificationFileReader::OntologyClassificationFileReader(QObject *parent) :
    QObject(parent)
{
    m_pFuzzyFunctionFactory = new FuzzyFunctionFactory(this);
}


int OntologyClassificationFileReader::getLevelOfClass(const QDomDocument &doc, const QString &name){

    QDomNode rootNode = doc.lastChild();
    QDomNode hierrarchyNode = rootNode.firstChild();
    QDomNode rdfXmlnsNode = hierrarchyNode.firstChild();

    QDomNode node = rdfXmlnsNode.firstChild();
    while(!node.isNull()){

        if(node.childNodes().count() == 3){

            QDomNode typeNode = node.firstChild();
            if(typeNode.isElement()){

                QDomElement typeElement = typeNode.toElement();
                QString resource = typeElement.attribute(QLatin1String("rdf:resource"));
                resource.remove(QLatin1String("#"));
                if(!resource.compare(name)){

                    QDomNode definedSegmentationLevelNode = node.lastChild();
                    if(definedSegmentationLevelNode.isElement()){

                        QDomElement definedSegmentationLevelElement = definedSegmentationLevelNode.toElement();
                        resource = definedSegmentationLevelElement.attribute(QLatin1String("rdf:resource"));

                        resource = resource.section(QLatin1String("_"),1,1);
                        return resource.toInt();
                    }
                }
            }
        }

        node = node.nextSibling();
    }

    return -1;
}


QString OntologyClassificationFileReader::getAttributeName(const QString &propertyname, const QString &tagname){

    QString name = propertyname;

    if(!tagname.compare(QLatin1String("attributeRule"))){

        name.remove(QLatin1String("Fuzzy"));
        name.insert(5,QLatin1String("::"));

        int idx = name.indexOf(QLatin1String("Band"));
        name.insert(idx+5,QLatin1String("::"));
    }else if(!tagname.compare(QLatin1String("shapeRule"))){

        name.remove(QLatin1String("Fuzzy"));
        name.insert(5,QLatin1String("::"));
    }

    return name;
}

void OntologyClassificationFileReader::parseRulesOfClass(OntologyClass *pClass, const QDomNode &node){

    QDomNode ruleNode = node.firstChild();
    while(!ruleNode.isNull()){

        if(ruleNode.isElement()){

            QDomElement ruleElement = ruleNode.toElement();
            QString propertyname = ruleElement.attribute(QLatin1String("property"));
            QString tagname = ruleElement.tagName();
            QString attribute = getAttributeName(propertyname,tagname);

            ObjectAttribute *pAttr = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(pClass->level(),attribute);
            if(!pAttr){

                qDebug() << "Could not create the fuzzy rule,could not find the attribute..";
                return;
            }

            QDomNode fuzzyFunctionNode = ruleNode.firstChild();
            if(fuzzyFunctionNode.isElement()){

                QDomElement fuzzyFunctionElement = fuzzyFunctionNode.toElement();
                QString ffname = fuzzyFunctionElement.attribute(QLatin1String("name"));
                FuzzyFunction *pFunction = m_pFuzzyFunctionFactory->createFuzzyFunction(ffname);
                int uid = 1;
                bool running = true;
                while(running){
                    if(FUZZYRULEMANAGER->fuzzyRuleById(uid)) // a fuzzy rule with the same id exists already
                        uid++;
                    else
                        running = false;
                }
                pFunction->setid(uid);

                int counter_ = 0;
                QDomNode parametersNode = fuzzyFunctionNode.firstChild();
                QDomNode parameterNode = parametersNode.firstChild();
                while(!parameterNode.isNull()){

                    if(parameterNode.isElement()){

                        QDomElement parameterElement = parameterNode.toElement();
                        double val = parameterElement.attribute(QLatin1String("value")).toDouble();
                        pFunction->setParameterValueForIndex(counter_,val);
                        counter_++;
                    }

                    parameterNode = parameterNode.nextSibling();
                }

                FUZZYFUNCTIONMANAGER->addFuzzyFunction(pFunction);
                qDebug() << "adding fuzzy function id : " << pFunction->name();

                uid = 1;
                running = true;
                while(running){

                    if(FUZZYRULEMANAGER->fuzzyRuleById(uid)) // a fuzzy rule with the same id exists already
                        uid++;
                    else
                        running = false;
                }

                FuzzyRule *pRule = new FuzzyRule(pFunction,FUZZYFUNCTIONMANAGER);
                pRule->setid(uid);
                pRule->setAttribute(pAttr->id());
                pRule->setlevelId(pClass->level());
                FUZZYRULEMANAGER->addFuzzyRule(pRule);
                pClass->addFuzzyRule(pClass->level(),pRule);

                qDebug() << "adding fuzzy rule attribute id : " << pRule->attribute();
            }
        }

        ruleNode = ruleNode.nextSibling();
    }
}


void OntologyClassificationFileReader::parseClass(OntologyClass *pClass, const QDomNode &node){

    QDomNode fuzzyRulesNode = node.firstChild();
    if(fuzzyRulesNode.isElement()){

        QDomElement fuzzyRulesElement = fuzzyRulesNode.toElement();
        QString operatorname = fuzzyRulesElement.attribute(QLatin1String("operator"));
        pClass->setopername(operatorname);
    }

    parseRulesOfClass(pClass,fuzzyRulesNode);

    QDomNode spatialRulesNode = node.lastChild();
    parseRulesOfClass(pClass,spatialRulesNode);
}


void OntologyClassificationFileReader::parseClasses(const QDomDocument &doc, const QDomNode &node){

    QDomNode classNode = node.firstChild();
    while(!classNode.isNull()){

        if(classNode.isElement()){
            QDomElement classElement = classNode.toElement();

            QString id = classElement.attribute(QLatin1String("id"));
            int level = getLevelOfClass(doc,id);
            if(level != -1){

                OntologyClass *pClass = new OntologyClass();
                pClass->setId(id);
                pClass->setName(id);
                pClass->setLevel(level);

                ONTOLOGYCLASSIFICATIONMANAGER->addOntologyClass(pClass);

                qDebug() << "adding class id : " << id;

                parseClass(pClass,classNode);
            }else{
                qDebug() << "level = -1";
            }
        }

        classNode = classNode.nextSibling();
    }
}

void OntologyClassificationFileReader::parsesHierrarchy(const QDomDocument &doc, const QDomNode &node){

    //! helper headers
    QString classTagName            = QLatin1String("owl:Class");
    QString idAttributeKey          = QLatin1String("rdf:ID");

    //! the second node is probably the node we need for iterating
    QDomNode snode = node.firstChild();

    QDomNode childNode = snode.firstChild();

    //! iterate through child items
    while(!childNode.isNull()){

        childNode = childNode.nextSibling();

        if(childNode.isElement()){

            QDomElement childElement = childNode.toElement();
            QString tagName = childElement.tagName();
            QString id = childElement.attribute(idAttributeKey);

            if(!tagName.compare(classTagName)){

                OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(id);
                if(!pClass){

                    qDebug() << "could not find the class with id : " << id;
                    continue;
                }

                QDomNode subClassNode = childNode.lastChild();
                QDomNode subClassOfClassNode = subClassNode.firstChild();
                if(subClassOfClassNode.isElement()){

                    QDomElement subClassOfClassElement = subClassOfClassNode.toElement();
                    if(subClassOfClassElement.hasAttribute(idAttributeKey)){

                        QString parentid = subClassOfClassElement.attribute(idAttributeKey);
                        OntologyClass *pParentClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(parentid);
                        if(!pClass){

                            qDebug() << "could not find the class with id : " << id;
                            continue;
                        }

                        pParentClass->addChild(pClass);
                        pClass->setParent(pParentClass);
                        pClass->setparentId(pParentClass->id());
                    }
                }
            }
        }

        childNode = childNode.nextSibling();
    }
}


//!
//! The general purpose of this function is to parse an owl and create the Ontology hierrarchy
//!
void OntologyClassificationFileReader::parse(const QString &path){

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "an error on opening the file occured..";
        file.close();
        return;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        qDebug() << "error on setting content of the file..";
        file.close();
        return;
    }

    file.close();

    QDomNode rootNode = doc.lastChild();

    QDomNode hierrarchyNode = rootNode.firstChild();
    QDomNode classesNode = rootNode.lastChild();

    parseClasses(doc,classesNode);
    parsesHierrarchy(doc,hierrarchyNode);
}

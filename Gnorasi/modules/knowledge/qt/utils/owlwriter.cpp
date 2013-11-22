#include "owlwriter.h"

#include <QDebug>
#include <QDateTime>

#include "../models/ontologyclass.h"
#include "../fuzzy/fuzzyrule.h"
#include "../fuzzy/fuzzyrulemanager.h"
#include "../fuzzy/fuzzyfunction.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../models/ontologyclass.h"
#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"
#include "../utils/objectlevel.h"
#include "../utils/objectlevelmanager.h"

#include <QtXml/QDomProcessingInstruction>

//
// Define keys tag names and values needed for creating the OWL document
//
// OWL related..
#define OWL_RDFTAGNAME          "rdf:RDF"
#define OWL_CLASSTAGNAME        "owl:Class"
#define OWL_SUBCLASSOFTAGNAME   "rdfs:subClassOf"
#define OWL_LABELTAGNAME        "rdfs:label"
#define OWL_COMMENTTAGNAME      "rdfs:comment"
#define OWL_RESOURCEKEY         "rdf:resource"
#define OWL_IDKEY               "rdf:ID"
#define OWL_ABOUTKEY            "rdf:about"
#define OWL_STRINGDATATYPEKEY   "rdf:datatype"
#define OWL_STRINGDATATYPEVAL   "http://www.w3.org/2001/XMLSchema#string"
#define OWL_INTDATATYPEKEY      "rdf:datatype"
#define OWL_INTDATATYPEVAL      "http://www.w3.org/2001/XMLSchema#int"
#define OWL_ONTOLOGYTAGNAME     "owl:Ontology"
#define OWL_DATAPROPERTYTAG     "owl:DatatypeProperty"
#define OWL_DPABOUTVAL          "http://www.gnorasi.gr/ontology#hasObjectClassID"
#define OWL_RDFSDOMAINTAG       "rdfs:domain"
#define OWL_RDFSDOMAINVAL       "http://www.gnorasi.gr/ontology#ObjectDepiction"
#define OWL_RDFRANGETAG         "rdfs:range"
#define OWL_RANGEVAL            "http://www.w3.org/2001/XMLSchema#int"
#define OWL_DESCRIPTIONKTAG     "rdf:Description"
#define OWL_TYPETAG             "rdf:type"
#define OWL_GNOHASOBJIDTAG      "gno:hasObjectClassID"


#define OBJECTDEPICTION_VALUE   "http://www.gnorasi.gr/ontology#ObjectDepiction"

// XML related..
#define XMLNS_RDFKEY            "xmlns:rdf"
#define XMLNS_RDFVALUE          "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define XMLNS_PROTEGEKEY        "xmlns:protege"
#define XMLNS_PROTEGEVALUE      "http://protege.stanford.edu/plugins/owl/protege#"
#define XMLNS_XSPKEY            "xmlns:xsp"
#define XMLNS_XSPVALUE          "http://www.owl-ontologies.com/2005/08/07/xsp.owl#"
#define XMLNS_OWLKEY            "xmlns:owl  "
#define XMLNS_OWLVALUE          "http://www.w3.org/2002/07/owl#"
#define XMLNS_KEY               "xmlns"
#define XMLNS_VALUE             "http://www.gnorasi.gr/ontology#"
#define XMLNS_XSDKEY            "xmlns:xsd"
#define XMLNS_XSDVALUE          "http://www.w3.org/2001/XMLSchema#"
#define XMLNS_SWRLKEY           "xmlns:swrl"
#define XMLNS_SWRLVALUE         "http://www.w3.org/2003/11/swrl#"
#define XMLNS_SWRLBKEY          "xmlns:swrlb"
#define XMLNS_SWRLBVALUE        "http://www.w3.org/2003/11/swrlb#"
#define XMLNS_RDFSKEY           "xmlns:rdfs"
#define XMLNS_RDFSVALUE         "http://www.w3.org/2000/01/rdf-schema#"
#define XMLNS_GEOKEY            "xmlns:geo"
#define XMLNS_GEOVALUE          "http://rdf.opensahara.com/type/geo/"
#define XMLNS_BASEKEY           "xml:base"
#define XMLNS_BASEVALUE         "http://www.gnorasi.gr/ontology"
#define XMLNS_GNOKEY            "xmlns:gno"
#define XMLNS_GNOVALUE          "http://www.gnorasi.gr/ontology#"

// a custom tag name
#define TAG_HASOBJECTID         "hasObjectClassID"

using namespace voreen;

OwlWriter::OwlWriter(QObject *parent)
    : QObject(parent)
{
//    helperCounter = 0;

    m_namespaceXmlns = "#";
}

QString OwlWriter::docToText(){
    return doc.toString(4);
}

void OwlWriter::createDocument(){
//    helperCounter = 0;

    doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    // create the root element
    owlrootElement = doc.createElement(QString::fromAscii(OWL_RDFTAGNAME));

    owlrootElement.setAttribute(QString::fromAscii(XMLNS_GEOKEY),QString::fromAscii(XMLNS_GEOVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_OWLKEY),QString::fromAscii(XMLNS_OWLVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_RDFKEY),QString::fromAscii(XMLNS_RDFVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_RDFSKEY),QString::fromAscii(XMLNS_RDFSVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_XSDKEY),QString::fromAscii(XMLNS_XSDVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_GNOKEY),QString::fromAscii(XMLNS_GNOVALUE));

    doc.appendChild(owlrootElement);

    //! create the Ontology class element
    QDomElement element = doc.createElement(QString::fromAscii(OWL_ONTOLOGYTAGNAME));
    element.setAttribute(QString::fromAscii(OWL_ABOUTKEY),QString(""));
    owlrootElement.appendChild(element);
}

void OwlWriter::createDocumentVersion2(){
//    helperCounter = 0;

    doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    QDomElement rootelement = doc.createElement(QLatin1String("root"));
    doc.appendChild(rootelement);

    //
    QDomElement hierrarchyElement = doc.createElement(QLatin1String("hierarchy"));
    rootelement.appendChild(hierrarchyElement);

    // create the element
    owlrootElement = doc.createElement(QString::fromAscii(OWL_RDFTAGNAME));

    QString cdt = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    owlrootElement.setAttribute(QLatin1String("xmlns"),QString::fromLatin1("http://www.gnorasi.gr/ontology/generated_%1#").arg(cdt));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_BASEKEY),QString::fromLatin1("http://www.gnorasi.gr/ontology/generated_%1").arg(cdt));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_GEOKEY),QString::fromAscii(XMLNS_GEOVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_OWLKEY),QString::fromAscii(XMLNS_OWLVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_RDFKEY),QString::fromAscii(XMLNS_RDFVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_RDFSKEY),QString::fromAscii(XMLNS_RDFSVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_XSDKEY),QString::fromAscii(XMLNS_XSDVALUE));
    owlrootElement.setAttribute(QString::fromAscii(XMLNS_GNOKEY),QString::fromAscii(XMLNS_GNOVALUE));

    hierrarchyElement.appendChild(owlrootElement);

    //! create the Ontology class element
    QDomElement element = doc.createElement(QString::fromAscii(OWL_ONTOLOGYTAGNAME));
    element.setAttribute(QString::fromAscii(OWL_ABOUTKEY),QString(""));
    owlrootElement.appendChild(element);
}


void OwlWriter::appendData(OntologyClass *item){
    QString label   = item->id();
//    QString comment = item->data(1).toString();
    QString id      = item->name();

    // create the class element
    QDomElement classElement = doc.createElement(QString::fromAscii(OWL_CLASSTAGNAME));
    classElement.setAttribute(QString::fromAscii(OWL_IDKEY),id);

    // create the label element
    QDomElement labelElement = doc.createElement(QString::fromAscii(OWL_LABELTAGNAME));
    labelElement.setAttribute(QString::fromAscii(OWL_STRINGDATATYPEKEY),QString::fromAscii(OWL_STRINGDATATYPEVAL));
    QDomText labelTextElement = doc.createTextNode(label);
    labelElement.appendChild(labelTextElement);
    classElement.appendChild(labelElement);

    // create the commnet element
//    QDomElement commentElement = doc.createElement(QString::fromAscii(OWL_COMMENTTAGNAME));
//    commentElement.setAttribute(QString::fromAscii(OWL_STRINGDATATYPEKEY),QString::fromAscii(OWL_STRINGDATATYPEVAL));
//    QDomText commentTextElement = doc.createTextNode(comment);
//    commentElement.appendChild(commentTextElement);
//    classElement.appendChild(commentElement);

    // create the subclass element
    QDomElement subclassofElement = doc.createElement(OWL_SUBCLASSOFTAGNAME);
    QDomElement childClassElement = doc.createElement(QString::fromAscii(OWL_CLASSTAGNAME));
    OntologyClass *parentItem = item->parent();
    if(parentItem){
        QString parentId = parentItem->id();
        if(!parentId.compare(QString::fromAscii(OBJECTDEPICTION_VALUE)))
            childClassElement.setAttribute(QString::fromAscii(OWL_ABOUTKEY),parentItem->id());
        else
            childClassElement.setAttribute(QString::fromAscii(OWL_IDKEY),parentItem->id());
    }else{
        childClassElement.setAttribute(QString::fromAscii(OWL_ABOUTKEY),QLatin1String("http://www.gnorasi.gr/ontology#ObjectDepiction"));
    }
    subclassofElement.appendChild(childClassElement);
    classElement.appendChild(subclassofElement);

    // append the class element to the root node
    owlrootElement.appendChild(classElement);

    // Create an individual element and append it to the root node.
    QDomElement individualElement = doc.createElement(QString::fromAscii(OWL_DESCRIPTIONKTAG));
    individualElement.setAttribute(QString::fromAscii(OWL_IDKEY),QString("ins_%1").arg(id.toLower()));
    // create the type element
    QDomElement typeElement = doc.createElement(QString::fromAscii(OWL_TYPETAG));
    typeElement.setAttribute(QString::fromAscii(OWL_RESOURCEKEY),QString("%1%2").arg(m_namespaceXmlns).arg(id));
    // create the hasObjectIdTExtElement
    QDomElement hasObjectIdElement = doc.createElement(QString::fromAscii(OWL_GNOHASOBJIDTAG));
    hasObjectIdElement.setAttribute(QString::fromAscii(OWL_STRINGDATATYPEKEY),QString::fromAscii(OWL_INTDATATYPEVAL));
//    QDomText hasObjectIDTextElement = doc.createTextNode(QString::number(helperCounter++));
    QDomText hasObjectIDTextElement = doc.createTextNode(QString::number(item->idx()));
    QDomElement segmentationLevelElement = doc.createElement(QLatin1String("gno:definedForSegmentationLevel"));
    segmentationLevelElement.setAttribute(QLatin1String("rdf:resource"),QString("#segmentationLevel_%1").arg(QString::number(item->level())));
    hasObjectIdElement.appendChild(hasObjectIDTextElement);
    individualElement.appendChild(typeElement);
    individualElement.appendChild(hasObjectIdElement);
    individualElement.appendChild(segmentationLevelElement);

    // append the individual element to the root Element
    owlrootElement.appendChild(individualElement);

    // now iterate children to create recursively the nodes
    QList<OntologyClass*> list = item->getChildItems();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClass *child = *i;

        appendData(child);
    }
}

//!
//! set the namespaces edited by the user
//!
//! vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//!
//! Take notice that this function must be called prior to the appendData() function..
//!
//! ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void OwlWriter::setupNamespaces(const QString &nsXmlns, const QString &nsXmlBase){
    //! settter
    m_namespaceXmlBase = nsXmlBase;
    m_namespaceXmlns = nsXmlns;

    QDomNodeList list = doc.elementsByTagName(QString::fromAscii(OWL_RDFTAGNAME));
    for(int i = 0; i < list.count(); i++){
        QDomNode node = list.at(i);

        if(node.isElement()){
            QDomElement element= node.toElement();

            element.setAttribute(QString::fromAscii(XMLNS_KEY),nsXmlns);
            element.setAttribute(QString::fromAscii(XMLNS_BASEKEY),nsXmlBase);
        }
    }

    if(list.isEmpty())
        qDebug() << "rdf list is empty no rdf node found ..";
}


void OwlWriter::appendRulesData(){

    QDomNode rootNode = doc.lastChild();
    if(rootNode.isNull())
        return;

    QDomElement classesElement = doc.createElement(QLatin1String("classes"));
    rootNode.appendChild(classesElement);

    QList<OntologyClass*> list = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassList();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClass *pClass = *i;

        QDomElement classElement = doc.createElement(QLatin1String("class"));
        classElement.setAttribute(QLatin1String("id"),pClass->id());
        classElement.setAttribute(QLatin1String("idx"),pClass->idx());
        classesElement.appendChild(classElement);

        QDomElement fuzzyRuleRootElement = doc.createElement(QLatin1String("fuzzyRules"));
        fuzzyRuleRootElement.setAttribute(QLatin1String("operator"),pClass->opername());
        classElement.appendChild(fuzzyRuleRootElement);

        QHash<int,FuzzyRule*> fuzzyrulehash = pClass->fuzzyRuleHash();
        QHash<int,FuzzyRule*>::const_iterator h;
        for(h = fuzzyrulehash.constBegin(); h != fuzzyrulehash.constEnd(); h++){
            int lid = h.key();
            FuzzyRule *pRule = h.value();

            QString attributeName = pRule->attribute();

            ObjectAttribute *pOA = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(lid,attributeName);
            if(!(pOA->otype() == 1 || pOA->otype() == 2))
                continue;

            attributeName = attributeName.remove(":");
            attributeName = QString("Fuzzy%1%2").arg(attributeName).arg(pRule->id());
            QDomElement element;

            if(pOA->otype()==1){

                element = doc.createElement(QLatin1String("attributeRule"));
                element.setAttribute(QLatin1String("id"),pRule->id());
                element.setAttribute(QLatin1String("property"),attributeName);
                fuzzyRuleRootElement.appendChild(element);
            }else{

                element = doc.createElement(QLatin1String("shapeRule"));
                element.setAttribute(QLatin1String("id"),pRule->id());
                element.setAttribute(QLatin1String("property"),attributeName);
                fuzzyRuleRootElement.appendChild(element);
            }

            FuzzyFunction *pfuzzyFunction = pRule->funzzyFunction();
            if(pfuzzyFunction){
                QDomElement functionElement = doc.createElement(QLatin1String("function"));
                functionElement.setAttribute(QLatin1String("name"),pfuzzyFunction->name());
                element.appendChild(functionElement);

                QDomElement parametersElement = doc.createElement(QLatin1String("parameters"));
                functionElement.appendChild(parametersElement);

                for(int i = 0; i < pfuzzyFunction->parametersCount(); i++){

                    int code = 97; // 'a'
                    code += i;

                    QString lstr = QString("%1").arg((char)code);
                    double val = pfuzzyFunction->parameterValueForIndex(i);

                    QDomElement parameterElement = doc.createElement(QLatin1String("parameter"));
                    parameterElement.setAttribute(QLatin1String("name"),lstr);
                    parameterElement.setAttribute(QLatin1String("value"),QString::number(val,'f',3));
                    parametersElement.appendChild(parameterElement);
                }
            }
        }
    }
}


void OwlWriter::appendSpatialData(){

    QDomNode rootNode = doc.lastChild();
    if(rootNode.isNull())
        return;

    QDomNode classesNode = rootNode.lastChild();

    QList<OntologyClass*> list = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassList();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClass *pClass = *i;

        QDomElement classElement;
        QString oid = pClass->id();
        QDomNode classNode = classesNode.firstChild();
        while(!classNode.isNull()){

            if(classNode.isElement()){

                classElement = classNode.toElement();
                QString ceaid = classElement.attribute(QLatin1String("id"),0);

                qDebug() << "ceaid : " << ceaid << ", oid : " << oid;

                if(!ceaid.compare(oid))
                    break;
            }

            classNode = classNode.nextSibling();
        }

        if(classElement.isNull()){
            qDebug() << "error, could not find the class , id : " << oid;
            continue;
        }

        QDomElement fuzzyRuleRootElement = doc.createElement(QLatin1String("spatialRules"));
        fuzzyRuleRootElement.setAttribute(QLatin1String("operator"),pClass->opername());
        classElement.appendChild(fuzzyRuleRootElement);

        QHash<int,FuzzyRule*> fuzzyrulehash = pClass->fuzzyRuleHash();
        QHash<int,FuzzyRule*>::const_iterator h;
        for(h = fuzzyrulehash.constBegin(); h != fuzzyrulehash.constEnd(); h++){
            int lid = h.key();
            FuzzyRule *pRule = h.value();

            QString attributeName = pRule->attribute();

            ObjectAttribute *pOA = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(lid,attributeName);

            Q_ASSERT(pOA);

            if(pOA->otype() != 3)
                continue;

            QDomElement element = doc.createElement(QLatin1String("spatialRule"));
            element.setAttribute(QLatin1String("id"),pRule->id());
            element.setAttribute(QLatin1String("property"),attributeName);

            FuzzyFunction *pfuzzyFunction = pRule->funzzyFunction();
            if(pfuzzyFunction){
                QDomElement functionElement = doc.createElement(QLatin1String("function"));
                functionElement.setAttribute(QLatin1String("name"),pfuzzyFunction->name());
                element.appendChild(functionElement);

                QDomElement parametersElement = doc.createElement(QLatin1String("parameters"));
                functionElement.appendChild(parametersElement);

                for(int i = 0; i < pfuzzyFunction->parametersCount(); i++){

                    int code = 97; // 'a'
                    code += i;

                    QString lstr = QString("%1").arg((char)code);
                    double val = pfuzzyFunction->parameterValueForIndex(i);

                    QDomElement parameterElement = doc.createElement(QLatin1String("parameter"));
                    parameterElement.setAttribute(QLatin1String("name"),lstr);
                    parameterElement.setAttribute(QLatin1String("value"),QString::number(val,'f',3));
                    parametersElement.appendChild(parameterElement);
                }
            }

            fuzzyRuleRootElement.appendChild(element);
        }
    }
}


void OwlWriter::processObjectLevel(ObjectLevel *level){
//    <rdf:Description rdf:ID="segmentationLevel_1">
//        <rdf:type rdf:resource="http://www.gnorasi.gr/ontology#SegmentationLevel"/>
//    </rdf:Description>
     QDomElement descriptionElement = doc.createElement(QLatin1String("rdf:Description"));
     descriptionElement.setAttribute(QLatin1String("rdf:ID"),QString("segmentationLevel_").append(QString::number(level->id())));
     owlrootElement.appendChild(descriptionElement);

     QDomElement typeElement = doc.createElement(QLatin1String("rdf:type"));
     typeElement.setAttribute(QLatin1String("rdf:resource"),QString("http://www.gnorasi.gr/ontology#SegmentationLevel"));
     descriptionElement.appendChild(typeElement);
}

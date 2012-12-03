#include "owlwriter.h"

#include <QDebug>

#include "../models/ontologyclassitem.h"

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

//<owl:DatatypeProperty rdf:about="http://www.gnorasi.gr/ontology#hasObjectClassID">
//    <rdfs:domain rdf:resource="http://www.gnorasi.gr/ontology#ObjectDepiction"/>
//    <rdfs:range rdf:resource="http://www.w3.org/2001/XMLSchema#int"/>
//  </owl:DatatypeProperty>

// a custom tag name
#define TAG_HASOBJECTID         "hasObjectClassID"

using namespace voreen;

OwlWriter::OwlWriter(QObject *parent)
    : QObject(parent)
{
    helperCounter = 0;
}

QString OwlWriter::docToText(){
    return doc.toString(4);
}

void OwlWriter::createDocument(){
    helperCounter = 0;

    doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    rootElement = doc.createElement(QString::fromAscii(OWL_RDFTAGNAME));

//    rootElement.setAttribute(QString::fromAscii(XMLNS_BASEKEY),QString::fromAscii(XMLNS_BASEVALUE));
    rootElement.setAttribute(QString::fromAscii(XMLNS_GEOKEY),QString::fromAscii(XMLNS_GEOVALUE));
//    rootElement.setAttribute(QString::fromAscii(XMLNS_KEY),QString::fromAscii(XMLNS_VALUE));
    rootElement.setAttribute(QString::fromAscii(XMLNS_OWLKEY),QString::fromAscii(XMLNS_OWLVALUE));
//    rootElement.setAttribute(QString::fromAscii(XMLNS_PROTEGEKEY),QString::fromAscii(XMLNS_PROTEGEVALUE));
    rootElement.setAttribute(QString::fromAscii(XMLNS_RDFKEY),QString::fromAscii(XMLNS_RDFVALUE));
    rootElement.setAttribute(QString::fromAscii(XMLNS_RDFSKEY),QString::fromAscii(XMLNS_RDFSVALUE));
//    rootElement.setAttribute(QString::fromAscii(XMLNS_SWRLBKEY),QString::fromAscii(XMLNS_SWRLVALUE));
//    rootElement.setAttribute(QString::fromAscii(XMLNS_SWRLBKEY),QString::fromAscii(XMLNS_SWRLBVALUE));
    rootElement.setAttribute(QString::fromAscii(XMLNS_XSDKEY),QString::fromAscii(XMLNS_XSDVALUE));
//    rootElement.setAttribute(QString::fromAscii(XMLNS_XSPKEY),QString::fromAscii(XMLNS_XSPVALUE));

    doc.appendChild(rootElement);

    //! create the Ontology class element
    QDomElement element = doc.createElement(QString::fromAscii(OWL_ONTOLOGYTAGNAME));
    element.setAttribute(QString::fromAscii(OWL_ABOUTKEY),QString(""));
    rootElement.appendChild(element);

    //! create the datatype tag element
    QDomElement dataPropertyElement = doc.createElement(QString::fromAscii(OWL_DATAPROPERTYTAG));
    dataPropertyElement.setAttribute(QString::fromAscii(OWL_ABOUTKEY),QString::fromAscii(OWL_DPABOUTVAL));
    QDomElement domainElement = doc.createElement(QString::fromAscii(OWL_RDFSDOMAINTAG));
    domainElement.setAttribute(QString::fromAscii(OWL_RESOURCEKEY),QString::fromAscii(OWL_RDFSDOMAINVAL));
    QDomElement rangeElement = doc.createElement(QString::fromAscii(OWL_RDFRANGETAG));
    rangeElement.setAttribute(QString::fromAscii(OWL_RESOURCEKEY),QString::fromAscii(OWL_RANGEVAL));
    dataPropertyElement.appendChild(domainElement);
    dataPropertyElement.appendChild(rangeElement);
    rootElement.appendChild(dataPropertyElement);
}

void OwlWriter::appendData(OntologyClassItem *item){
    QString label   = item->data(0).toString();
    QString comment = item->data(1).toString();
    QString id      = item->id();

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
    QDomElement commentElement = doc.createElement(QString::fromAscii(OWL_COMMENTTAGNAME));
    commentElement.setAttribute(QString::fromAscii(OWL_STRINGDATATYPEKEY),QString::fromAscii(OWL_STRINGDATATYPEVAL));
    QDomText commentTextElement = doc.createTextNode(comment);
    commentElement.appendChild(commentTextElement);
    classElement.appendChild(commentElement);

    // create the subclass element
    QDomElement subclassofElement = doc.createElement(OWL_SUBCLASSOFTAGNAME);
    QDomElement childClassElement = doc.createElement(QString::fromAscii(OWL_CLASSTAGNAME));
    OntologyClassItem *parentItem = item->parent();
    if(parentItem){
        QString parentId = parentItem->id();
        if(!parentId.compare(QString::fromAscii(OBJECTDEPICTION_VALUE)))
            childClassElement.setAttribute(QString::fromAscii(OWL_ABOUTKEY),parentItem->id());
        else
            childClassElement.setAttribute(QString::fromAscii(OWL_IDKEY),parentItem->id());
    }
    subclassofElement.appendChild(childClassElement);
    classElement.appendChild(subclassofElement);

    // append the class element to the root node
    rootElement.appendChild(classElement);

    // Create an individual element and append it to the root node.
    QDomElement individualElement = doc.createElement(id);
    individualElement.setAttribute(QString::fromAscii(OWL_IDKEY),QString("ins_%1").arg(id.toLower()));
    QDomElement hasObjectClassIDElement = doc.createElement(QString::fromAscii(TAG_HASOBJECTID));
    hasObjectClassIDElement.setAttribute(QString::fromAscii(OWL_INTDATATYPEKEY),QString::fromAscii(OWL_INTDATATYPEVAL));
    QDomText hasObjectIDTextElement = doc.createTextNode(QString::number(helperCounter++));
    hasObjectClassIDElement.appendChild(hasObjectIDTextElement);
    individualElement.appendChild(hasObjectClassIDElement);

    // append the individual element to the root Element
    rootElement.appendChild(individualElement);

    // now iterate children to create recursively the nodes
    QList<OntologyClassItem*> list = item->getChildItems();
    QList<OntologyClassItem*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClassItem *child = *i;

        appendData(child);
    }
}

//!
void OwlWriter::setNamespaces(const QString &nsXmlns, const QString &nsXmlBase){
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

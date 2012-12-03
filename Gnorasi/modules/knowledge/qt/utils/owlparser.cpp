#include "owlparser.h"

#include "owlhelperitem.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

#define OBJD_HEADER     "ObjectDepiction"
#define OBJECTDEPICTION_VALUE   "http://www.gnorasi.gr/ontology#ObjectDepiction"

#define RDF_TAG         "rdf:RDF"
#define XMLNS_KEY       "xmlns"
#define XMLNS_BASEKEY   "xml:base"


OWLParser::OWLParser(QObject *parent)
    : QObject(parent)
{
    m_pOWLHelperItem = NULL;
}

//!
//! The general purpose of this function is to parse an owl and create the Ontology hierrarchy
//!
//! This is an attempt to parse the XML content of the OWL document.
//! And also create the hierrarchy needed by the Ontology logic.
//! This function tries to locate the top level items inherit from ObjectDepiction class
//! and then iterate recursively through the OWLHelperItem functionality.
//!
void OWLParser::parse(const QString &path){

    if(m_pOWLHelperItem != NULL){
        delete m_pOWLHelperItem;
        m_pOWLHelperItem = NULL;
    }

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

    //! create a root item
    m_pOWLHelperItem = new OWLHelperItem(QString::fromAscii(OBJECTDEPICTION_VALUE),QString("ObjectDepiction"),QString("This is the root class"),this);

    //! then parse for children recursively
    m_pOWLHelperItem->parseChildren(doc);

    //! parse for setting up the namespace values
    setupNamespaces(doc);
}

//!
//! parses the content of a file
//!
void OWLParser::parseContent(const QString &text){

    if(m_pOWLHelperItem != NULL){
        delete m_pOWLHelperItem;
        m_pOWLHelperItem = NULL;
    }

    QDomDocument doc;
    QString error;
    int line = 0, col = 0;
    if(!doc.setContent(text,&error,&line,&col))
    {
        qDebug() << "error on setting content of the file..";
        qDebug() << "error : " << error << "\t line : " << line << "\t col :" << col;
        return;
    }

    // create a root item
    m_pOWLHelperItem = new OWLHelperItem(QString::fromAscii(OBJECTDEPICTION_VALUE),QString("ObjectDepiction"),QString("This is the root class"),this);

    // then parse for children recursively
    m_pOWLHelperItem->parseChildren(doc);

    //! parse for setting up the namespace values
    setupNamespaces(doc);
}

//! setup the namespaces
//! this function should be called after the document is parsed..
void OWLParser::setupNamespaces(const QDomDocument& doc){
    QDomNodeList list = doc.elementsByTagName(QString::fromAscii(RDF_TAG));
    for(int i = 0; i < list.count(); i++){
        QDomNode node = list.at(i);

        if(node.isElement()){
            QDomElement element= node.toElement();

            if(element.hasAttribute(QString::fromAscii(XMLNS_KEY)))
                m_namespace_xmlns = element.attribute(QString::fromAscii(XMLNS_KEY));

            if(element.hasAttribute(QString::fromAscii(XMLNS_BASEKEY)))
                m_namespace_xml_base = element.attribute(QString::fromAscii(XMLNS_BASEKEY));
        }
    }

    if(list.isEmpty())
        qDebug() << "rdf list is empty no rdf node found ..";
}

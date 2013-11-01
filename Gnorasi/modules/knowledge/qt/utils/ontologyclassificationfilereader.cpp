#include "ontologyclassificationfilereader.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

OntologyClassificationFileReader::OntologyClassificationFileReader(QObject *parent) :
    QObject(parent)
{
}


//!
//! The general purpose of this function is to parse an owl and create the Ontology hierrarchy
//!
//! This is an attempt to parse the XML content of the OWL document.
//! And also create the hierrarchy needed by the Ontology logic.
//! This function tries to locate the top level items inherit from ObjectDepiction class
//! and then iterate recursively through the OWLHelperItem functionality.
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

//    //! create a root item
//    m_pOWLHelperItem = new OWLHelperItem(QString::fromAscii(OBJECTDEPICTION_VALUE),QString("ObjectDepiction"),QString("This is the root class"),this);

//    //! then parse for children recursively
//    m_pOWLHelperItem->parseChildren(doc);

//    //! parse for setting up the namespace values
//    setupNamespaces(doc);
}

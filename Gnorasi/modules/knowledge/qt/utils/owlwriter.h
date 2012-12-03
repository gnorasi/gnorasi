#ifndef OWLWRITER_H
#define OWLWRITER_H

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "voreen/qt/voreenmoduleqt.h"

namespace voreen {

class OntologyClassItem;

class VRN_QT_API OwlWriter : public QObject{
    Q_OBJECT

public:

    OwlWriter(QObject *parent = 0);

    void createDocument();

    //! append the data from the OntologyClassItem
    void appendData(OntologyClassItem* );

    //! set the namespaces edited by the user
    void setNamespaces(const QString &nsXmlns, const QString &nsXmlBase);

    //! return the dom document data as text
    QString docToText();

    //! returns the QDomDocument
    QDomDocument domDocument() { return doc; }

private:

    int helperCounter;

    QDomDocument doc;

    QDomElement rootElement;
}; // OWLWRITER_H

}

#endif

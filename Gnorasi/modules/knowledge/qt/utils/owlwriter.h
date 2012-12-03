#ifndef OWLWRITER_H
#define OWLWRITER_H

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "voreen/qt/voreenmoduleqt.h"


class OntologyClassItem;

class /*VRN_QT_API*/ OwlWriter : public QObject{
    Q_OBJECT

public:

    OwlWriter(QObject *parent = 0);

    void createDocument();

    //! append the data from the OntologyClassItem
    void appendData(OntologyClassItem* );

    //!
    //! set the namespaces edited by the user
    //!
    //! vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //!
    //! Take notice that this function must be called prior to the appendData() function..
    //!
    //! ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    void setupNamespaces(const QString &nsXmlns, const QString &nsXmlBase);

    //! return the dom document data as text
    QString docToText();

    //! returns the QDomDocument
    QDomDocument domDocument() { return doc; }

private:

    //!
    QString m_namespaceXmlns;

    //!
    QString m_namespaceXmlBase;

    //!
    int helperCounter;

    QDomDocument doc;

    QDomElement rootElement;
}; // OWLWRITER_H


#endif

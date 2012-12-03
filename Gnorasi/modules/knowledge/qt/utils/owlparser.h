#ifndef OWLPARSER_H
#define OWLPARSER_H

#include <QtCore/QObject>

#include <QtXml/QDomDocument>


class OWLHelperItem;

//!
//! This class is a parse of the OWL document
//!
class /*VRN_QT_API*/ OWLParser : public QObject{
    Q_OBJECT

public:

    OWLParser(QObject *parent = 0);

    //! parse the file given by the QString parameter ..
    void parse(const QString & );

    //! parse the content given by the QString parameter
    void parseContent(const QString &);

    OWLHelperItem* rootOWLHelperItem() const { return m_pOWLHelperItem; }

    //!
    QString namespaceXmlns()const { return m_namespace_xmlns; }
    QString namespaceXmlBase() const { return m_namespace_xml_base; }

private:

    //! setup the namespaces
    //! this function should be called after the document is parsed..
    void setupNamespaces(const QDomDocument& );

    OWLHelperItem *m_pOWLHelperItem;

    //! These namespaces are set when a new Ontology hierrarchy is being created
    //! Should be set when a file is parsed
    QString m_namespace_xmlns;
    QString m_namespace_xml_base;

}; // OWLPARSER_H


#endif

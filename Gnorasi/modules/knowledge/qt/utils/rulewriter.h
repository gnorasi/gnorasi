#ifndef RULEWRITER_H
#define RULEWRITER_H

#include <QObject>

#include <QDomDocument>

namespace voreen{

class RuleItem;

class RuleWriter : public QObject
{
    Q_OBJECT
public:
    explicit RuleWriter(QObject *parent = 0);

    void createDocument();

    // append the data from the OntologyClassItem
    void appendData();

    // return a QString of the QDomDocument
    QString docToText();

    // self explanatory
    QDomDocument domDocument() { return doc; }

    // set the namespace values to the dom document
    void setNamespaces(const QString &nsXmlns, const QString &nsXmlBase);


private:

//    void parseRuleItem(RuleItem* );

    int helperCounter;

    QDomDocument doc;

    QDomElement rootElement;
    
};

}

#endif // RULEWRITER_H

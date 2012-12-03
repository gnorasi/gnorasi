#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <QObject>

#include <QDomDocument>


//!
//! Functionality, parses xml data related to rules
//! and adds them in memory.
//!
class RuleParser : public QObject
{
    Q_OBJECT
public:
    explicit RuleParser(QObject *parent = 0);

    //! parse the contents of the Xml file at the location given by the filePath
    void parseFile(const QString &filePath, QString *error = 0);
    
signals:
    
public slots:


private:
    //! namespace checking
    //! checks the rdf element namespaces attribute key values contained in the document passed as a parameter
    //! and the namespaces set in the RuleUtility instance while loading a valid OntologyClassModel given by
    //! the OntologyCreator processor
    bool validateNamespaces(const QDomDocument& );
    
};


#endif // RULEPARSER_H

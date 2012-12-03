#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <QObject>

#include <QDomDocument>

namespace voreen{

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
    bool validateNamespaces(const QDomDocument& );
    
};

}

#endif // RULEPARSER_H

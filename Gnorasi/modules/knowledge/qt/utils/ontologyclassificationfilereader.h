#ifndef ONTOLOGYCLASSIFICATIONFILEREADER_H
#define ONTOLOGYCLASSIFICATIONFILEREADER_H

#include <QObject>

#include <QtXml/QDomDocument>

class OntologyClass;
class FuzzyFunctionFactory;

class OntologyClassificationFileReader : public QObject
{
    Q_OBJECT
public:
    explicit OntologyClassificationFileReader(QObject *parent = 0);

    //! parse the file given by the QString parameter ..
    void parse(const QString & );

    //! parse the content given by the QString parameter
//    void parseContent(const QString &);

signals:

public slots:

private:

    int getLevelOfClass(const QDomDocument&, const QString &name);


    void parseClasses(const QDomDocument& , const QDomNode& );


    void parseHierrarchy(const QDomNode& );


    void parseClass(OntologyClass*, const QDomNode& );


    void parseRulesOfClass(OntologyClass*, const QDomNode& );


    QString getAttributeName(const QString& , const QString& );


    FuzzyFunctionFactory *m_pFuzzyFunctionFactory;


};

#endif // ONTOLOGYCLASSIFICATIONFILEREADER_H

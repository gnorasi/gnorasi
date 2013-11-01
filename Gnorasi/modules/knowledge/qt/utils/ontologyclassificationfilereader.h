#ifndef ONTOLOGYCLASSIFICATIONFILEREADER_H
#define ONTOLOGYCLASSIFICATIONFILEREADER_H

#include <QObject>

#include <QtXml/QDomDocument>

class OntologyClassificationFileReader : public QObject
{
    Q_OBJECT
public:
    explicit OntologyClassificationFileReader(QObject *parent = 0);

    //! parse the file given by the QString parameter ..
    void parse(const QString & );

    //! parse the content given by the QString parameter
    void parseContent(const QString &);

signals:

public slots:

private:


};

#endif // ONTOLOGYCLASSIFICATIONFILEREADER_H

#ifndef OWLHELPERITEM_H
#define OWLHELPERITEM_H

#include <QObject>

#include <QDomDocument>

#include "voreen/qt/voreenmoduleqt.h"

namespace voreen {

class VRN_QT_API OWLHelperItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id       READ id         WRITE setId         NOTIFY idChanged)
    Q_PROPERTY(QString label    READ label      WRITE setLabel      NOTIFY labelChanged)
    Q_PROPERTY(QString comment  READ comment    WRITE setComment    NOTIFY commentChanged)
public:
    explicit OWLHelperItem(const QString &id = QString(), const QString &lbl = QString(), const QString &cmt = QString(), QObject *parent = 0);

    QString id() const { return m_id; }
    void setId(const QString &s) { m_id = s; }

    QString label() const { return m_label; }
    void setLabel(const QString &s) { m_label = s; }

    QString comment() const { return m_comment; }
    void setComment(const QString &s) { m_comment = s; }

    QList<OWLHelperItem*> owlChildren() const { return m_owlchildren; }

    void parseChildren(QDomDocument );

signals:
    void labelChanged();
    void commentChanged();
    void idChanged();

private:
    QList<OWLHelperItem*> m_owlchildren;
    
    bool checkSiblings(const QString &);

    void parseSiblings(QDomDocument doc);

    QString m_label;
    QString m_comment;
    QString m_id;
};

}

#endif // OWLHELPERITEM_H

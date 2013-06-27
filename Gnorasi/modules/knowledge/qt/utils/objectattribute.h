#ifndef OBJECTATTRIBUTE_H
#define OBJECTATTRIBUTE_H

#include <QObject>

class ObjectAttribute : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString id   READ id   WRITE setId   NOTIFY idChanged)
public:
    explicit ObjectAttribute(QObject *parent = 0);
    
    QString name() const { return m_name; }
    void setName(const QString &n) { m_name = n; }

    QString id() const { return m_id;}
    void setId(const QString &i) { m_id = i; }


signals:
    
    void nameChanged();
    void idChanged();

public slots:

private:
    QString m_name;
    QString m_id;
    
};

#endif // OBJECTATTRIBUTE_H

#ifndef FUZZYOPERATOR_H
#define FUZZYOPERATOR_H

#include <QObject>

class FuzzyOperator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit FuzzyOperator(QObject *parent = 0);

    QString name() const { return m_name; }
    void setName(const QString &n) { m_name = n; }
    
signals:
    void nameChanged();
    
public slots:

private:
    QString m_name;
    
};

#endif // FUZZYOPERATOR_H

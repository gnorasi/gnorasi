#ifndef OBJECTLEVEL_H
#define OBJECTLEVEL_H

#include <QObject>


class ObjectLevel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
public:
    explicit ObjectLevel(QObject *parent = 0);
    
    int id() const {return m_id; }
    void setId(int i){ m_id = i; }

signals:
    void idChanged();
    
public slots:

private:
    int m_id;
    
};


#endif // OBJECTLEVEL_H

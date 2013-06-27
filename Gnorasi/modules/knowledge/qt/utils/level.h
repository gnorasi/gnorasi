#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>

class Level : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setid NOTIFY idChanged)
public:
    explicit Level(QObject *parent = 0);
    
    int id() const {return m_id; }
    void setId(int i){ m_id = i; }

signals:
    void idChanged();
    
public slots:

private:
    int m_id;
    
};

#endif // LEVEL_H

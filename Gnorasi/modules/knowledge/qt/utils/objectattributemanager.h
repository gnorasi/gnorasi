#ifndef OBJECTATTRIBUTEMANAGER_H
#define OBJECTATTRIBUTEMANAGER_H

#include <QObject>

class ObjectAttribute;

// a helper macro
#define OBJECTATTRIBUTEMANAGER ObjectAttributeManager::instance()

class ObjectAttributeManager : public QObject
{
    Q_OBJECT
public:

    static ObjectAttributeManager* instance();
    static void deleteInstance();
    
    QList<ObjectAttribute*> objectAttributeList() const {return m_objectAttributeList; }
    void setObjectAttributeList(const QList<ObjectAttribute*> &l) { m_objectAttributeList = l; }

    void addObjectAttribute(ObjectAttribute *l) { m_objectAttributeList.append(l); }
    void removeObjectAttribute(ObjectAttribute *l) { m_objectAttributeList.removeOne(l); }

    ObjectAttribute* objectAttributeById(const QString &idString) ;
    ObjectAttribute* objectAttributeById(int id) ;

    void clear() { qDeleteAll(m_objectAttributeList); m_objectAttributeList.clear(); }

signals:
    
public slots:



private:
    explicit ObjectAttributeManager(QObject *parent = 0);
    ~ObjectAttributeManager();

    static ObjectAttributeManager *m_pInstance;

    QList<ObjectAttribute*> m_objectAttributeList;
    
};

#endif // OBJECTATTRIBUTEMANAGER_H

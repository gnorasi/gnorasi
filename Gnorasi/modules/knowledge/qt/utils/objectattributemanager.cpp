#include "objectattributemanager.h"

#include "objectattribute.h"

ObjectAttributeManager* ObjectAttributeManager::m_pInstance = NULL;

ObjectAttributeManager::ObjectAttributeManager(QObject *parent) :
    QObject(parent)
{
}

ObjectAttributeManager* ObjectAttributeManager::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new ObjectAttributeManager();
    }

    return m_pInstance;
}

void ObjectAttributeManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


ObjectAttributeManager::~ObjectAttributeManager(){
    clear();
}

ObjectAttribute* ObjectAttributeManager::objectAttributeById(const QString &idString){
    QList<ObjectAttribute*>::const_iterator i;
    for(i = m_objectAttributeList.constBegin(); i != m_objectAttributeList.constEnd(); i++){
        ObjectAttribute *o = *i;

        if(!o->id().compare(idString))
            return o;
    }

    return 0;
}

ObjectAttribute* ObjectAttributeManager::objectAttributeById(int id){
    QString idString = QString::number(id);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = m_objectAttributeList.constBegin(); i != m_objectAttributeList.constEnd(); i++){
        ObjectAttribute *o = *i;

        if(!o->id().compare(idString))
            return o;
    }

    return 0;
}

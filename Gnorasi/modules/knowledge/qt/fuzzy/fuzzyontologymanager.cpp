#include "fuzzyontologymanager.h"
#include "fuzzyontologyclass.h"

FuzzyOntologyManager* FuzzyOntologyManager::m_pInstance = NULL;

FuzzyOntologyManager::FuzzyOntologyManager(QObject *parent) :
    QObject(parent)
{
}


FuzzyOntologyManager* FuzzyOntologyManager::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new FuzzyOntologyManager();

    return m_pInstance;
}


void FuzzyOntologyManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


FuzzyOntologyClass* FuzzyOntologyManager::fuzzyByOntologyClassByName(const QString &name){
    QList<FuzzyOntologyClass*>::const_iterator i;
    for(i = m_fuzzyOntolgyClassList.constBegin(); i != m_fuzzyOntolgyClassList.constEnd(); i++)
    {
        FuzzyOntologyClass *pClass = *i;
        QString className = pClass->className();
        if(!className.compare(name))
            return pClass;
    }

    return 0;
}


FuzzyOntologyManager::~FuzzyOntologyManager(){
    qDeleteAll(m_fuzzyOntolgyClassList);
    m_fuzzyOntolgyClassList.clear();
}

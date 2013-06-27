#include "fuzzyoperatormanager.h"

FuzzyOperatorManager* FuzzyOperatorManager::m_pInstance = NULL;

FuzzyOperatorManager::FuzzyOperatorManager(QObject *parent) :
    QObject(parent)
{
}

FuzzyOperatorManager::~FuzzyOperatorManager(){
    clear();
}

FuzzyOperatorManager* FuzzyOperatorManager::instance()
{
    if(m_pInstance == NULL)
        m_pInstance = new FuzzyOperatorManager();

    return m_pInstance;
}

void FuzzyOperatorManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

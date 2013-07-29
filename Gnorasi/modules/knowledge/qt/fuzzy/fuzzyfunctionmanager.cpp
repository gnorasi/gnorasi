#include "fuzzyfunctionmanager.h"

#include "fuzzyfunction.h"

FuzzyFunctionManager* FuzzyFunctionManager::m_pInstance = NULL;

FuzzyFunctionManager::FuzzyFunctionManager(QObject *parent) :
    QObject(parent)
{
}


FuzzyFunctionManager* FuzzyFunctionManager::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new FuzzyFunctionManager();

    return m_pInstance;
}

void FuzzyFunctionManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

FuzzyFunctionManager::~FuzzyFunctionManager(){
    clear();
}

FuzzyFunction* FuzzyFunctionManager::fuzzyFunctionById(int id){
    QList<FuzzyFunction*>::const_iterator i;
    for(i = m_fuzzyFunctionList.constBegin(); i!= m_fuzzyFunctionList.constEnd(); i++){
        FuzzyFunction *pFunction  = *i;

        if(pFunction->id() == id){
            return pFunction;
        }
    }

    return 0;
}

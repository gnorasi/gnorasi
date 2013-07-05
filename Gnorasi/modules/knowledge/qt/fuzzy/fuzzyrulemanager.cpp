#include "fuzzyrulemanager.h"

#include "fuzzyrule.h"

#include <QStringList>

FuzzyRuleManager* FuzzyRuleManager::m_pInstance = NULL;

FuzzyRuleManager::FuzzyRuleManager(QObject *parent) :
    QObject(parent)
{
}

FuzzyRuleManager::~FuzzyRuleManager(){
    clear();
}

FuzzyRuleManager* FuzzyRuleManager::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new FuzzyRuleManager();

    return m_pInstance;
}

void FuzzyRuleManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

FuzzyRule* FuzzyRuleManager::fuzzyRuleById(int id){
    QList<FuzzyRule*>::const_iterator i;
    for(i = m_fuzzyRuleList.constBegin(); i != m_fuzzyRuleList.constEnd(); i++){
        FuzzyRule *prule = *i;

        if(prule->id() == id)
            return prule;
    }

    return 0;
}

QStringList FuzzyRuleManager::fuzzyRulesNames(){
    QStringList list;
    QList<FuzzyRule*>::const_iterator i;
    for(i = m_fuzzyRuleList.constBegin(); i != m_fuzzyRuleList.constEnd(); i++){
        FuzzyRule *pRule = *i;

        QString name = pRule->name();

        if(!list.contains(name))
            list << name;
    }

    return list;
}

#include "itiotblevelutility.h"

#include "itiotblevel.h"

using namespace itiviewer;


//! initialize the unique instance to null
LevelUtility* LevelUtility::m_pInstance = NULL;

LevelUtility::LevelUtility(QObject *parent) :
    QObject(parent)
{
}


LevelUtility* LevelUtility::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new LevelUtility();

    return m_pInstance;
}


void LevelUtility::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance == NULL;
    }
}


Level* LevelUtility::levelById(int id){
    QList<Level*>::const_iterator i;
    for(i = m_levelList.constBegin(); i != m_levelList.constEnd(); i++){
        Level *pLevel = *i;

        if(pLevel->id() == id)
            return pLevel;
    }

    return 0;
}


void LevelUtility::appendLevel(Level *pLevel){
    m_levelList << pLevel;
}

void LevelUtility::deleteLevel(Level *pLevel){
    m_levelList.removeOne(pLevel);
}

void LevelUtility::clearLevels(){
    qDeleteAll(m_levelList);
    m_levelList.clear();
}

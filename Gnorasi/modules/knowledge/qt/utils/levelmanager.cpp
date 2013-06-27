#include "levelmanager.h"

LevelManager *LevelManager::m_pInstance = NULL;

LevelManager::LevelManager(QObject *parent) :
    QObject(parent)
{
}

LevelManager::~LevelManager(){
    clear();
}

LevelManager* LevelManager::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new LevelManager();

    return m_pInstance;
}

void LevelManager::deleteInstance()
{
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

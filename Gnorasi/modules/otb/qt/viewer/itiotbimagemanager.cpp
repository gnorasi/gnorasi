#include "itiotbimagemanager.h"

#include "otbImageFileReader.h"

using namespace itiviewer;

//! initialize the unique instance to null
ItiOtbImageManager* ItiOtbImageManager::m_pInstance = NULL;

//!
ItiOtbImageManager::ItiOtbImageManager()
{

}

//!
ItiOtbImageManager* ItiOtbImageManager::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new ItiOtbImageManager();
    }

    return m_pInstance;
}

//!
void ItiOtbImageManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;

        m_pInstance = NULL;
    }
}

//!
ItiOtbImageManager::~ItiOtbImageManager(){

}

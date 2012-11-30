#include "itiotbimagemanager.h"

#include "otbImageFileReader.h"

using namespace itiviewer;


template <class TImage> ItiOtbImageManager<TImage>* ItiOtbImageManager<TImage>::m_pInstance = NULL;

//!
template< class TImage>
ItiOtbImageManager<TImage>::ItiOtbImageManager()
{

}

//!
template< class TImage>
ItiOtbImageManager<TImage>* ItiOtbImageManager<TImage>::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new ItiOtbImageManager();
    }

    return m_pInstance;
}

//!
template< class TImage>
void ItiOtbImageManager<TImage>::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;

        m_pInstance = NULL;
    }
}

//!
template< class TImage>
ItiOtbImageManager<TImage>::~ItiOtbImageManager(){

}

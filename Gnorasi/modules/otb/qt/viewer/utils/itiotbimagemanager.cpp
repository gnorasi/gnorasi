#include "itiotbimagemanager.h"

#include "otbImageFileReader.h"

#include "../commands/command.h"

//#include "../../../ports/otbimageport.h"
//#include "../../../ports/otbvectorimageport.h"

using namespace itiviewer;
using namespace otb;
using namespace voreen;

//! initialize the unique instance to null
ItiOtbImageManager* ItiOtbImageManager::m_pInstance = NULL;

//!
ItiOtbImageManager::ItiOtbImageManager()
{
    filter = ImageToVectorImageCastFilterType::New();
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
void ItiOtbImageManager::setupImage(){

    //! type case checking
    if(dynamic_cast<OTBImagePort*>(m_pPort)){ // set here the raster image port
        //! cast it ot OTBImagePort
        OTBImagePort *pOtbImagePort = dynamic_cast<OTBImagePort*>(m_pPort);

        if(!pOtbImagePort)
            return;

        RasterImageType *rImgType = (RasterImageType*)pOtbImagePort->getData();
        if(!rImgType)
            return;

        filter->SetInput(rImgType);

        m_pImgType = filter->GetOutput();

        filter->Update();

    }
    else if(dynamic_cast<OTBVectorImagePort*>(m_pPort)){ // set here the vector image factory
        OTBVectorImagePort *pOtbVectorImagePort = dynamic_cast<OTBVectorImagePort*>(m_pPort);

        //! get the image
        VectorImageType *vImgType = (VectorImageType*)pOtbVectorImagePort->getData();
        if(!vImgType)
            return;

        m_pImgType = vImgType;
    }
}

//!
QString ItiOtbImageManager::imageFile() const {
    OTBImagePort *iPort = (OTBImagePort*)m_pPort;

    if(iPort){
        std::string text = iPort->imagePath();
        for(int i = 0;i < text.size(); i++){
            qDebug() << text[i];
        }
        return QString::fromStdString(text);
    }

    return QString();
}

//!
ItiOtbImageManager::~ItiOtbImageManager(){

}

#include "itiotbimagemanager.h"

#include "otbImageFileReader.h"

#include "../commands/command.h"

#include "../../../processors/ImageIO/otbvectorimagereaderprocessor.h"

#include "itiotblabelmapparser.h"
#include "itiotblabelimageparser.h"
#include "itiotblevel.h"
#include "itiotbregion.h"

#include "../../../ports/otblabelimageport.h"

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

    m_levelList.append(new Level(this));
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

        createRegions();

    }
    else if(dynamic_cast<OTBVectorImagePort*>(m_pPort)){ // set here the vector image factory
        OTBVectorImagePort *pOtbVectorImagePort = dynamic_cast<OTBVectorImagePort*>(m_pPort);

        //! get the image
        VectorImageType *vImgType = (VectorImageType*)pOtbVectorImagePort->getData();
        if(!vImgType)
            return;

        m_pImgType = vImgType;

        createRegions();
    }
}

//!
QString ItiOtbImageManager::imageFile() {

    QString path;

    std::vector<voreen::Processor*> list = nextConnectedProcessor(m_pPort);
    if(list.size()){
        Processor *proc = list.at(0);

        if(isReader(proc)){
            path = getPathFromReaderProcessor(proc);
        }else
            checkNext(proc, path);
    }

    return path;
}

void ItiOtbImageManager::checkNext(voreen::Processor* proc, QString &path ){
    std::vector<voreen::Port*> portlist = proc->getPorts();
    if(portlist.size()){
        voreen::Port *port = portlist.at(0);

        std::vector<voreen::Processor*> proclist = nextConnectedProcessor(port);

        for(int i = 0; i < proclist.size(); i++){
            proc = proclist.at(i);

            if(isReader(proc)){
                path = getPathFromReaderProcessor(proc);
                break;
            }
            else if( proc->getInports().size()){
                checkNext(proc, path);
            }
        }
    }
}

std::vector<voreen::Processor*> ItiOtbImageManager::nextConnectedProcessor(voreen::Port *port) const{
    std::vector<voreen::Processor*> proclist;

    const std::vector<const voreen::Port*> list = port->getConnected();
    for(int i = 0; i < list.size(); i++){
        const Port *port = list.at(i);
        Processor *proc = port->getProcessor();

        proclist.push_back(proc);
    }

    return proclist;
}


bool ItiOtbImageManager::isReader(voreen::Processor* proc) const{
    std::string text = proc->getClassName();
    QString className = QString::fromStdString(text);

    if(!className.compare(QLatin1String("Image File Reader")))
        return true;
    else if(!className.compare(QLatin1String("MultiBand Image File Reader")))
        return true;

    return false;
}

QString ItiOtbImageManager::getPathFromReaderProcessor(voreen::Processor *proc) const{
    const std::vector<voreen::Port*> list = proc->getPorts();

    for(int i = 0; i < list.size(); i++){
        voreen::Port* port = list.at(i);

        if(dynamic_cast<voreen::OTBImagePort*>(port)){
            OTBImagePort *iPort = dynamic_cast<voreen::OTBImagePort*>(port);
            return QString::fromStdString(iPort->imagePath());
        }

        if(dynamic_cast<voreen::OTBVectorImagePort*>(port)){
            OTBVectorImagePort *vPort = dynamic_cast<voreen::OTBVectorImagePort*>(port);
            return QString::fromStdString(vPort->imagePath());
        }
    }

    return QString();
}


void ItiOtbImageManager::createRegions(){

    qDeleteAll(m_levelList);
    m_levelList.clear();

    typedef otb::Image<unsigned long, 2> LabelImageType;

    m_levelList << new Level(this);

    voreen::Processor *proc = m_pPort->getProcessor();
    const std::vector<voreen::Port*> list = proc->getPorts();

    for(int i = 0; i < list.size(); i++){
        voreen::Port* port = list.at(i);

        QString className = QString::fromStdString(port->getName());

        if(!className.compare(QLatin1String("Label Image Port"))){

            OTBLabelImagePort *lblPort = dynamic_cast<OTBLabelImagePort*>(port);
            if(lblPort && lblPort->isConnected()){
                LabelImageType *mapT = (LabelImageType*)lblPort->getData();

                if(mapT){

//                    LabelMapParser *parser = new LabelMapParser(this);
                    LabelImageParser *parser = new LabelImageParser(this);

                    QList<Region*> regionList = parser->parse(mapT);

                    Level *pLevel = m_levelList.last();
                    pLevel->setRegions(regionList);
                }
            }

            break;
        }
    }
}

//!
ItiOtbImageManager::~ItiOtbImageManager(){
//    qDeleteAll(m_levelList);
//    m_levelList.clear();
}

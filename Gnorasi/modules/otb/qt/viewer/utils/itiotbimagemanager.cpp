#include "itiotbimagemanager.h"

#include "otbImageFileReader.h"

#include "../commands/command.h"

#include "../../../processors/ImageIO/otbvectorimagereaderprocessor.h"

#include "itiotblabelmapparser.h"
#include "itiotblabelimageparser.h"
#include "itiotblevel.h"
#include "itiotbregion.h"
#include "itiotblevelutility.h"

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

    clearLevels();

    voreen::Processor *proc = m_pPort->getProcessor();
    const std::vector<voreen::Port*> list = proc->getPorts();

    for(int i = 0; i < list.size(); i++){
        voreen::Port* port = list.at(i);

        QString className = QString::fromStdString(port->getName());

        if(!className.compare(QLatin1String("Label Map Port"))){

            OTBLabelMapPort *lblPort = dynamic_cast<OTBLabelMapPort*>(port);
            if(lblPort && lblPort->isConnected()){
                LabelMapParser::LabelMapType *mapT = (LabelMapParser::LabelMapType*)lblPort->getData();

                if(mapT){

                    LabelMapParser *parser = new LabelMapParser(this);

                    Level *pLevel = new Level(this);

                    QList<Region*> regionList = parser->parse(mapT);

                    pLevel->setRegions(regionList);

                    appendLevel(pLevel);

                    setupColors();
                }
            }

            break;
        }
    }
}

Level* ItiOtbImageManager::levelById(int id){
    QList<Level*>::const_iterator i;
    for(i = m_levelList.constBegin(); i != m_levelList.constEnd(); i++){
        Level *pLevel = *i;

        if(pLevel->id() == id)
            return pLevel;
    }

    return 0;
}


void ItiOtbImageManager::appendLevel(Level *pLevel){
    m_levelList << pLevel;
}

void ItiOtbImageManager::deleteLevel(Level *pLevel){
    m_levelList.removeOne(pLevel);
}

void ItiOtbImageManager::clearLevels(){
    qDeleteAll(m_levelList);
    m_levelList.clear();
}

//!
void ItiOtbImageManager::setupColors(){
    m_colorHash.clear();

    //! parse once to extract classfication and color information
    QList<Level*>::const_iterator i;
    for(i = m_levelList.constBegin(); i != m_levelList.constEnd(); i++){
        Level *pLevel = *i;

        QList<Region*> rList = pLevel->regions();

        QList<Region*>::const_iterator o;
        for(o = rList.constBegin(); o != rList.constEnd(); o++){
            Region *pRegion = *o;

            int clfId = pRegion->classificationId();

            if(!m_colorHash.contains(clfId)){
                int r = Region::randInt(0,255);
                int g = Region::randInt(0,255);
                int b = Region::randInt(0,255);
                m_colorHash[clfId] = QColor(r,g,b);
                m_classficationNamesIds[clfId] = QString::number(clfId);
            }
        }
    }


    //! parse a second time to set the color property to the regions
    QList<Level*>::const_iterator z;
    for(z = m_levelList.constBegin(); z != m_levelList.constEnd(); z++){
        Level *pLevel  = *z;

        QList<Region*> rList = pLevel->regions();
        QList<Region*>::const_iterator x;
        for(x = rList.constBegin(); x != rList.constEnd(); x++){
            Region *pRegion = *x;

            int clfId = pRegion->classificationId();

            pRegion->setColor(m_colorHash.value(clfId));
        }

        qDebug() << "Number of regions created : " << rList.size();
    }

    qDebug() << "Number of classification ids : " << m_colorHash.uniqueKeys().size();
}

//!
ItiOtbImageManager::~ItiOtbImageManager(){
//    qDeleteAll(m_levelList);
//    m_levelList.clear();
}

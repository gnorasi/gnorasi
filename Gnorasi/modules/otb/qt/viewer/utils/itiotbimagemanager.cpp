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

#include "../models/itiotbVectorImageModel.h"

using namespace itiviewer;
using namespace otb;
using namespace voreen;


//!
ItiOtbImageManager::ItiOtbImageManager(QObject *parent)
    : QObject(parent)
{
    filter = ImageToVectorImageCastFilterType::New();
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

        filter->Update();

        m_pImgType = filter->GetOutput();

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

bool ItiOtbImageManager::isPortEmpty(Port *port){
    //! type case checking
    if(dynamic_cast<OTBImagePort*>(port)){ // set here the raster image port
        //! cast it ot OTBImagePort
        OTBImagePort *pOtbImagePort = dynamic_cast<OTBImagePort*>(port);

        RasterImageType *rImgType = (RasterImageType*)pOtbImagePort->getData();
        if(!rImgType /*|| rImgType->GetLargestPossibleRegion().GetSize()[0] == 0 || rImgType->GetLargestPossibleRegion().GetSize()[1] == 0*/ )
            return true;

    }
    else if(dynamic_cast<OTBVectorImagePort*>(port)){ // set here the vector image factory
        OTBVectorImagePort *pOtbVectorImagePort = dynamic_cast<OTBVectorImagePort*>(port);

        //! get the image
        VectorImageType *vImgType = (VectorImageType*)pOtbVectorImagePort->getData();
        if(!vImgType || vImgType->GetLargestPossibleRegion().GetSize()[0] == 0 || vImgType->GetLargestPossibleRegion().GetSize()[1] == 0)
            return true;
    }

    return false;
}

//!
QString ItiOtbImageManager::imageFile(voreen::Port *port) {

    QString path;

    std::vector<voreen::Processor*> list;

    if(port){
        list = nextConnectedProcessor(port);
    }else{
        list = nextConnectedProcessor(m_pPort);
    }

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
                    parser->setManager(this);

                    Level *pLevel = new Level(this);

                    QList<Region*> regionList = parser->parse(mapT);
                    m_classficationNamesIds = parser->classLabelIdsNames();

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

    QHash<int,QString>::const_iterator i;
    for(i = m_classficationNamesIds.constBegin(); i != m_classficationNamesIds.constEnd(); i++){
        int clid = i.key();

        int r = Region::randInt(0,255);
        int g = Region::randInt(0,255);
        int b = Region::randInt(0,255);
        m_colorHash[clid] = QColor(r,g,b);
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
}


QString ItiOtbImageManager::constructInfoByIndex(ImageRegionType::IndexType idx, const QString &pixelInfo){
    QString text;

    ImageRegionType region = m_pImgType->GetLargestPossibleRegion();

    const VectorImageType::SpacingType sp = m_pImgType->GetSpacing();

    VectorImageType::PointType pt;

    m_pImgType->TransformIndexToPhysicalPoint(idx,pt);

    text += QString("Index :[%1, %2]").arg(QString::number(idx[0])).arg(QString::number(idx[1]));
    text += "\n";
    text += QString("Image Size : [%1, %2]").arg(QString::number(region.GetSize()[0])).arg(QString::number(region.GetSize()[1]));
    text += "\n";
    text += pixelInfo;
    text += QString("Ground spacing in m: (%1, %2)").arg(QString::number(sp[0])).arg(QString::number(sp[1]));
    text += "\n";
    text += QString("Lon: %1, Lat : %2").arg(QString::number(pt[0],'f',2)).arg(QString::number(pt[1],'f',2));

    return text;
}

QString ItiOtbImageManager::constructInfoByIndexAlt(ImageRegionType::IndexType idx){
    QString text;

    ImageRegionType region = m_pImgType->GetLargestPossibleRegion();

    const VectorImageType::SpacingType sp = m_pImgType->GetSpacing();

    VectorImageType::PointType pt;

    m_pImgType->TransformIndexToPhysicalPoint(idx,pt);

    text += QString("Index :[%1, %2]").arg(QString::number(idx[0])).arg(QString::number(idx[1]));
    text += "\n";
    text += QString("Image Size : [%1, %2]").arg(QString::number(region.GetSize()[0])).arg(QString::number(region.GetSize()[1]));
    text += "\n";
    text += QString("Ground spacing in m: (%1, %2)").arg(QString::number(sp[0])).arg(QString::number(sp[1]));
    text += "\n";
    text += QString("Lon: %1, Lat : %2").arg(QString::number(pt[0],'f',2)).arg(QString::number(pt[1],'f',2));

    return text;
}

//!
ItiOtbImageManager::~ItiOtbImageManager(){
//    qDeleteAll(m_levelList);
//    m_levelList.clear();
}

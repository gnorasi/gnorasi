#include "commandchangeclasscolor.h"

#include "../utils/itiotbimagemanager.h"
#include "../utils/itiotbregion.h"
#include "../utils/itiotblevel.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"
#include "../utils/itiotbimagemanager.h"

using namespace itiviewer;

CommandChangeClassColor::CommandChangeClassColor(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_pItiOtbVectorImageViewer(viewer), m_classLabelId(-1), Command(parent)
{
}

void CommandChangeClassColor::execute(){
    ItiOtbImageManager *manager = m_pItiOtbVectorImageViewer->manager();

    QList<Level*> levelList = manager->levels();

    QList<Level*>::const_iterator i;
    for(i = levelList.constBegin(); i != levelList.constEnd(); i++){
        Level *pLevel = *i;

        QList<Region*> rList = pLevel->regions();

        QList<Region*>::iterator o;
        for(o = rList.begin(); o != rList.end(); o++){
            Region *pRegion = *o;

            int clfId = pRegion->classificationId();

            if(clfId == m_classLabelId)
                pRegion->setColor(m_color);
        }
    }
}

#include "commandtoggleclassificationlabelvisibiltiy.h"

#include "../utils/itiotbimagemanager.h"
#include "../utils/itiotbregion.h"
#include "../utils/itiotblevel.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"
#include "../utils/itiotbimagemanager.h"

using namespace itiviewer;

CommandToggleClassificationLabelVisibiltiy::CommandToggleClassificationLabelVisibiltiy(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_pItiOtbVectorImageViewer(viewer), m_visible(true), m_classLabelId(-1), Command(parent)
{
}

void CommandToggleClassificationLabelVisibiltiy::execute(){
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
                pRegion->setVisible(m_visible);
        }
    }
}

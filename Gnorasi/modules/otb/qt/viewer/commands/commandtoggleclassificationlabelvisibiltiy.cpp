#include "commandtoggleclassificationlabelvisibiltiy.h"

#include "../utils/itiotbimagemanager.h"
#include "../utils/itiotbregion.h"
#include "../utils/itiotblevel.h"

using namespace itiviewer;

CommandToggleClassificationLabelVisibiltiy::CommandToggleClassificationLabelVisibiltiy(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandToggleClassificationLabelVisibiltiy::execute(){
    QList<Level*> levelList = ITIOTBIMAGEMANAGER->levels();

    QList<Level*>::const_iterator i;
    for(i = levelList.constBegin(); i != levelList.constEnd(); i++){
        Level *pLevel = *i;

        QList<Region*> rList = pLevel->regions();

        QList<Region*>::const_iterator o;
        for(o = rList.constBegin(); o != rList.constEnd(); o++){
            Region *pRegion = *o;

            int clfId = pRegion->classificationId();

            if(clfId == m_classLabelId)
                pRegion->setVisible(m_visible);
        }
    }
}

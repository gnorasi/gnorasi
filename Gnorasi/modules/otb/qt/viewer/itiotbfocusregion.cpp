#include "itiotbfocusregion.h"

#include "itiotbfocusregionobserver.h"

using namespace itiviewer;

ItiOtbFocusRegion::ItiOtbFocusRegion(QObject *parent) :
    QObject(parent)
{
}

//!
void ItiOtbFocusRegion::notifyObservers(){
    QList<ItiOtbFocusRegionObserver*>::const_iterator i;
    for(i = m_observerList.constBegin(); i != m_observerList.constEnd(); i++){
        ItiOtbFocusRegionObserver *observer = *i;
        observer->update(this);
    }
}

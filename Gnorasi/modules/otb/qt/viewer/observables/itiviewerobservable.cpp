#include "itiviewerobservable.h"

#include "../widgets/itiviewerobserver.h"

using namespace itiviewer;

//!
void ItiViewerObservable::notifyObservers(){
    QList<ItiViewerObserver*>::const_iterator i;
    for(i = m_observerList.constBegin(); i != m_observerList.constEnd(); i++){
        ItiViewerObserver *observer = *i;
        observer->updateObserver(this);
    }
}

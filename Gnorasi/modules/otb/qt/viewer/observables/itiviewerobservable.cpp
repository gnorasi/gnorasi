#include "itiviewerobservable.h"

#include "../widgets/itiviewerobserver.h"

using namespace itiviewer;

ItiViewerObservable::ItiViewerObservable(QObject *parent) :
    QObject(parent)
{
}

//!
void ItiViewerObservable::notifyObservers(){
    QList<ItiViewerObserver*>::const_iterator i;
    for(i = m_observerList.constBegin(); i != m_observerList.constEnd(); i++){
        ItiViewerObserver *observer = *i;
        observer->update(this);
    }
}

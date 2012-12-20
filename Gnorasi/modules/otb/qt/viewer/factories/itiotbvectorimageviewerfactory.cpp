#include "itiotbvectorimageviewerfactory.h"

#include "../widgets/itiotbvectorimageviewer.h"


using namespace itiviewer;

ItiOtbVectorImageViewerFactory::ItiOtbVectorImageViewerFactory(QObject *parent)
    : ItiOtbImageViewerFactory(parent)
{
}

//!
ItiOtbImageViewer* ItiOtbVectorImageViewerFactory::createViewer(){
    return new ItiOtbVectorImageViewer();
}

ItiOtbVectorImageViewerFactory::~ItiOtbVectorImageViewerFactory(){

}

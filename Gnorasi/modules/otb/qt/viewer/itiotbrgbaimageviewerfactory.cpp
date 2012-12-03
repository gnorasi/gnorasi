#include "itiotbrgbaimageviewerfactory.h"

#include "itiotbrgbaimageviewer.h"


using namespace itiviewer;

ItiOtbRgbaImageViewerFactory::ItiOtbRgbaImageViewerFactory(QObject *parent)
    : ItiOtbImageViewerFactory(parent)
{
}

//!
ItiOtbImageViewer* ItiOtbRgbaImageViewerFactory::createViewer(){
    return new ItiOtbRgbaImageViewer();
}

ItiOtbRgbaImageViewerFactory::~ItiOtbRgbaImageViewerFactory(){

}

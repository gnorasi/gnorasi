#include "itiotbrgbazoomablewidget.h"
#include "../observables/itiviewerobservableregion.h"
#include "../utils/itiotbimagemanager.h"
#include "itiotbrgbaqglwidget.h"

#include "../../../ports/otbimageport.h"

#include <QVBoxLayout>

using namespace itiviewer;


ItiOtbRgbaZoomableWidget::ItiOtbRgbaZoomableWidget(QWidget *parent)
    : ItiViewerObserver(parent)
{
    initialize();
}

//!
void ItiOtbRgbaZoomableWidget::initialize(){
    m_pOpenGlWidget = new ItiOtbRgbaQGLWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_pOpenGlWidget);

    setLayout(layout);
}

//!
void ItiOtbRgbaZoomableWidget::update(ItiViewerObservable *observable){

    ItiViewerObservable *rgbaRegion = qobject_cast<ItiViewerObservable*>(observable);
    if(!rgbaRegion)
        return;

    //! TODO
    //! add the functionality here
}

//!
void ItiOtbRgbaZoomableWidget::draw(){
    OTBImagePort *port = (OTBImagePort*)ITIOTBIMAGEMANAGER->port();

    if(!port)
        return;

    //!
    RasterImageType* imgType =  (RasterImageType*)port->getData();
    if(!imgType)
        return;

    RasterRegionType region = imgType->GetLargestPossibleRegion();

    //!
    m_pOpenGlWidget->ReadBuffer(imgType,region);
}

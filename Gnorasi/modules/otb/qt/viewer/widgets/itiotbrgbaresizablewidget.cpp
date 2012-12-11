#include "itiotbrgbaresizablewidget.h"
#include "../utils/itiotbimagemanager.h"
#include "../observables/itiviewerobservableregion.h"
#include "itiotbrgbaqglwidgetautoresize.h"

#include "../../../ports/otbimageport.h"

#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbRgbaResizableWidget::ItiOtbRgbaResizableWidget(QWidget *parent)
    : ItiViewerObserver(parent)
{
    initialize();
}

//!
void ItiOtbRgbaResizableWidget::initialize(){
    m_pOpenGlWidget = new ItiOtbRgbaQGLWidgetAutoResize(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_pOpenGlWidget);

    setLayout(layout);
}

//!
void ItiOtbRgbaResizableWidget::updateObserver(ItiViewerObservable *observable){

    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
    if(!region)
        return;

    //! TODO
    //! add the functionality here
    m_pOpenGlWidget->setVisibleRegion(region->region());
}

//!
void ItiOtbRgbaResizableWidget::draw(){
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

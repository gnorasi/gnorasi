#include "itiotbrgbascrollablewidget.h"
#include "../observables/itiotbrgbafocusregion.h"
#include "../utils/itiotbimagemanager.h"
#include "itiotbrgbaimagewidget.h"

#include "../../../ports/otbimageport.h"

#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbRgbaScrollableWidget::ItiOtbRgbaScrollableWidget(QWidget *parent) :
    ItiViewerObserver(parent)
{
    initialize();
}

//!
void ItiOtbRgbaScrollableWidget::initialize(){
    m_pOpenGlWidget = new ItiOtbRgbaImageWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_pOpenGlWidget);

    setLayout(layout);
}

//!
void ItiOtbRgbaScrollableWidget::update(ItiViewerObservable *observable){

    ItiOtbRgbaFocusRegion *rgbaRegion = qobject_cast<ItiOtbRgbaFocusRegion*>(observable);
    if(!rgbaRegion)
        return;

    //! TODO
    //! add the functionality here for drawing here
}

//!
void ItiOtbRgbaScrollableWidget::draw(){
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

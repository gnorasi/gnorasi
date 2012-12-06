#include "itiotbrgbazoomablewidget.h"
#include "itiotbrgbaimagewidget.h"
#include "../observables/itiotbrgbafocusregion.h"

#include <QVBoxLayout>

using namespace itiviewer;


ItiOtbRgbaZoomableWidget::ItiOtbRgbaZoomableWidget(QWidget *parent)
    : ItiViewerObserver(parent)
{
    initialize();
}

//!
void ItiOtbRgbaZoomableWidget::initialize(){
    m_pOpenGlWidget = new ItiOtbRgbaImageWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_pOpenGlWidget);

    setLayout(layout);
}

//!
void ItiOtbRgbaZoomableWidget::update(ItiViewerObservable *observable){

    ItiOtbRgbaFocusRegion *rgbaRegion = qobject_cast<ItiOtbRgbaFocusRegion*>(observable);
    if(!rgbaRegion)
        return;

    //! TODO
    //! add the functionality here
}

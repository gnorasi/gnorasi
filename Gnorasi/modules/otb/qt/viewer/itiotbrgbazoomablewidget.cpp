#include "itiotbrgbazoomablewidget.h"
#include "itiotbrgbaimagewidget.h"
#include "itiotbrgbafocusregion.h"

#include <QVBoxLayout>

using namespace itiviewer;


ItiOtbRgbaZoomableWidget::ItiOtbRgbaZoomableWidget(QWidget *parent)
    : ItiOtbFocusRegionObserver(parent)
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
void ItiOtbRgbaZoomableWidget::update(ItiOtbFocusRegion *region){

    ItiOtbRgbaFocusRegion *rgbaRegion = qobject_cast<ItiOtbRgbaFocusRegion*>(region);
    if(!rgbaRegion)
        return;

    //! TODO
    //! add the functionality here
}

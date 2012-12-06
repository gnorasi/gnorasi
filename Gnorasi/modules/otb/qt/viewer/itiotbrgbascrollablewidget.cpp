#include "itiotbrgbascrollablewidget.h"
#include "itiotbrgbafocusregion.h"
#include "itiotbrgbaimagewidget.h"

#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbRgbaScrollableWidget::ItiOtbRgbaScrollableWidget(QWidget *parent) :
    ItiOtbFocusRegionObserver(parent)
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
void ItiOtbRgbaScrollableWidget::update(ItiOtbFocusRegion *region){

    ItiOtbRgbaFocusRegion *rgbaRegion = qobject_cast<ItiOtbRgbaFocusRegion*>(region);
    if(!rgbaRegion)
        return;

    //! TODO
    //! add the functionality here for drawing here
}

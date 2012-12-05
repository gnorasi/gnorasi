#include "itiotbrgbafullwidget.h"
#include "itiotbrgbafocusregion.h"
#include "itiotbrgbaimagewidget.h"

#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbRgbaFullWidget::ItiOtbRgbaFullWidget(QWidget *parent)
    : ItiOtbFocusRegionObserver(parent)
{
    initialize();
}

//!
void ItiOtbRgbaFullWidget::initialize(){
    m_pOpenGlWidget = new ItiOtbRgbaImageWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_pOpenGlWidget);

    setLayout(layout);
}

//!
void ItiOtbRgbaFullWidget::update(ItiOtbFocusRegion *region){

    ItiOtbRgbaFocusRegion *rgbaRegion = qobject_cast<ItiOtbRgbaFocusRegion*>(region);
    if(!rgbaRegion)
        return;

    //! TODO
    //! add the functionality here
}

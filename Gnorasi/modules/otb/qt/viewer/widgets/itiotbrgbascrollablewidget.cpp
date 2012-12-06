#include "itiotbrgbascrollablewidget.h"
#include "../observables/itiotbrgbafocusregion.h"
#include "itiotbrgbaimagewidget.h"

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

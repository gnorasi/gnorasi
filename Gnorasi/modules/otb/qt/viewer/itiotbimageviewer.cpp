#include "itiotbimageviewer.h"

using namespace itiviewer;

ItiOtbImageViewer::ItiOtbImageViewer(QWidget *parent)
    : QWidget(parent)
{
    // initialize instances
    m_pLabelFullResolution  = new QLabel(tr("Full Resolution View"),this);
    m_pLabelNavigationView  = new QLabel(tr("Navigation View"),this);
    m_pLabelZoomView        = new QLabel(tr("Zoom View"),this);

    //! setup the resize policy
    m_pLabelFullResolution->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelNavigationView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelZoomView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
}

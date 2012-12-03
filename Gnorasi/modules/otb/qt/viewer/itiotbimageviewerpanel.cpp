#include "itiotbimageviewerpanel.h"

#include "itiotbimageviewerpaneldatatab.h"
#include "itiotbimageviewerpanelsetuptab.h"
#include "itiotbimageviewerpanelhistogramtab.h"
#include "itiotbimageviewerpanelpixeldescriptiontab.h"

#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbImageViewerPanel::ItiOtbImageViewerPanel(QWidget *parent) :
    QWidget(parent)
{
    initialize();
}

//!
void ItiOtbImageViewerPanel::initialize(){
    //! the data tab
    m_pDataTab = new ItiOtbImageViewerPanelDataTab(this);

    //! the setu tab
    m_pSetupTab = new ItiOtbImageViewerPanelSetupTab(this);

    //! the histogram data tab
    m_pHistogramTab = new ItiOtbImageViewerPanelHistogramTab(this);

    //! the pixel description tab
    m_pPixelDescriptionTab = new ItiOtbImageViewerPanelPixelDescriptionTab(this);

    //! the tab widget
    m_pTabWidget = new QTabWidget(this);

    m_pTabWidget->addTab(m_pDataTab,m_pDataTab->windowTitle());
    m_pTabWidget->addTab(m_pSetupTab,m_pSetupTab->windowTitle());
    m_pTabWidget->addTab(m_pHistogramTab,m_pHistogramTab->windowTitle());
    m_pTabWidget->addTab(m_pPixelDescriptionTab,m_pPixelDescriptionTab->windowTitle());

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_pTabWidget);

    setLayout(layout);
}

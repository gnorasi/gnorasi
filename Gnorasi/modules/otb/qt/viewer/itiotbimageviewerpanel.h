#ifndef ITIOTBIMAGEVIEWERPANEL_H
#define ITIOTBIMAGEVIEWERPANEL_H

#include <QWidget>
#include <QTabWidget>

namespace itiviewer{

class ItiOtbImageViewerPanelDataTab;
class ItiOtbImageViewerPanelSetupTab;
class ItiOtbImageViewerPanelHistogramTab;
class ItiOtbImageViewerPanelPixelDescriptionTab;

/*!
 * \brief The ItiOtbImageViewerPanel class
 *  This is the widget handling the various parameters of the
 *  ItiOtbImageViewer.
 */
class ItiOtbImageViewerPanel : public QWidget
{
    Q_OBJECT
public:
    //!
    explicit ItiOtbImageViewerPanel(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    //! inititializing stuff
    void initialize();

    //! the main tool box
    QTabWidget *m_pTabWidget;

    //! tab widgets
    ItiOtbImageViewerPanelDataTab *m_pDataTab;
    ItiOtbImageViewerPanelSetupTab *m_pSetupTab;
    ItiOtbImageViewerPanelHistogramTab *m_pHistogramTab;
    ItiOtbImageViewerPanelPixelDescriptionTab *m_pPixelDescriptionTab;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVIEWERPANEL_H

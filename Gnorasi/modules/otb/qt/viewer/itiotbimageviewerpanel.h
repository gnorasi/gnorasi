#ifndef ITIOTBIMAGEVIEWERPANEL_H
#define ITIOTBIMAGEVIEWERPANEL_H

#include <QWidget>
#include <QTabWidget>

namespace itiviewer{

//! class declarations
class ItiOtbImageViewerPanelDataTab;
class ItiOtbImageViewerPanelSetupTab;
class ItiOtbImageViewerPanelHistogramTab;
class ItiOtbImageViewerPanelPixelDescriptionTab;

/*!
 * \brief The ItiOtbImageViewerPanel class
 *  This is the widget handling the various parameters of the
 *  ItiOtbImageViewer.
 *  The tab widget will have 4 tabs , as the Monteverdi application has..
 */
class ItiOtbImageViewerPanel : public QWidget
{
    Q_OBJECT
public:
    //! ctor
    explicit ItiOtbImageViewerPanel(QWidget *parent = 0);
    
    //! dtor
    virtual ~ItiOtbImageViewerPanel() {}

signals:
    
public slots:

private:
    //! inititializing stuff
    void initialize();

    //! the main tool box
    QTabWidget *m_pTabWidget;

    //! tab widget for setting up data properties
//    ItiOtbImageViewerPanelDataTab *m_pDataTab;

    //! tab widget for setting up setup properties
    ItiOtbImageViewerPanelSetupTab *m_pSetupTab;

    //! tab widget for setting up histogram properties
//    ItiOtbImageViewerPanelHistogramTab *m_pHistogramTab;

    //! tab widget for setting up pixel description properties
//    ItiOtbImageViewerPanelPixelDescriptionTab *m_pPixelDescriptionTab;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVIEWERPANEL_H

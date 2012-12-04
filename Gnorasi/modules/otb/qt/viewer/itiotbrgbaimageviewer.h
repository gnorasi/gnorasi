#ifndef ITIOTBRGBAIMAGEVIEWER_H
#define ITIOTBRGBAIMAGEVIEWER_H

#include <QtCore>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>

#include "itiotbimageviewer.h"
#include "itiotbimagemanager.h"


namespace itiviewer{

class ItiOtbRgbaImageWidget;

/** \class ITIOTBImageViewer
 *   \brief This class implements a standard visualization tool to be
 *   plugged at the end of a pipeline.
 *
 *  \ingroup Visualization
 *
 */
class ItiOtbRgbaImageViewer : public ItiOtbImageViewer
{
    Q_OBJECT
public:
    explicit ItiOtbRgbaImageViewer(QWidget *parent = 0);

    /** Set center*/
//    void setCenter(IndexType index);

    //! implementation
    void disassembleWidgets();

    //! implementation
    void assembleWidgets();

    //! implementation
    void draw();

signals:
    
public slots:

private:
    void setupLayout();

    //! The Scrollable QGLWidget
    ItiOtbRgbaImageWidget *m_pItiOtbRgbaImageWidgetScroll;

    //! The Full QGLWidget
    ItiOtbRgbaImageWidget *m_pItiOtbRgbaImageWidgetFull;

    //! The Zoomable QGLWidget
    ItiOtbRgbaImageWidget *m_pItiOtbRgbaImageWidgetZoom;

    //! The left vertical layout, needed for assembiling and disassembliing purposes
    QVBoxLayout *m_pvBoxLayoutLeft;

    //! The right vertical layout, needed for assembiling and disassembliing purposes
    QVBoxLayout *m_pVboxLayoutRight;

    //! The main layout, needed for assembiling and disassembliing purposes
    QVBoxLayout *m_pMainLayout;

    //! The horizontal splitter;
    QSplitter *m_pSplitter;

    //! the following labels were taken from the monteverdi viewer
    //! a label showing a title for the full resolution view
    QLabel *m_pLabelFullResolution;

    //! a label showing a title for the navigation view
    QLabel *m_pLabelNavigationView;

    //! a label showing a title for the zoom view
    QLabel *m_pLabelZoomView;

    //! a label showing a title for the metadata view
    QLabel *m_pLabelMetadataView;

    //! Container widget
    QWidget *m_pMetadataWidget;

    
};

} // end of namespace itiviewer

#endif // ITIOTBRGBAIMAGEVIEWER_H

#ifndef ITIOTBRGBAIMAGEVIEWER_H
#define ITIOTBRGBAIMAGEVIEWER_H

#include <QWidget>

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

    //! Container widget
    QWidget *m_pHistogramPlaceHolderWidget;

    
};

} // end of namespace itiviewer

#endif // ITIOTBRGBAIMAGEVIEWER_H

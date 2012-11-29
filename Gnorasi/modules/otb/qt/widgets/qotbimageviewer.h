#ifndef QOTBIMAGEVIEWER_H
#define QOTBIMAGEVIEWER_H

#include <QWidget>

#include "qotbimagewidget.h"

#include "../utils/qotbimagemanager.h"


/** \class QOTBImageViewer
 *   \brief This class implements a standard visualization tool to be
 *   plugged at the end of a pipeline.
 *
 *  \ingroup Visualization
 *
 */
class QOTBImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit QOTBImageViewer(QWidget *parent = 0);

    /** Set center*/
    void setCenter(IndexType index);

    /** Update and show the widget (you should call Fl::run() to hold to
    * display */
    void update();

signals:
    
public slots:

private:
    void setupLayout();

    //! The Scrollable QGLWidget
    QOTBImageWidget *m_pQOTBImageWidgetScroll;

    //! The Full QGLWidget
    QOTBImageWidget *m_pQOTBImageWidgetFull;

    //! The Zoomable QGLWidget
    QOTBImageWidget *m_pQOTBImageWidgetZoom;

    //! Container widget
    QWidget *m_pHistogramPlaceHolderWidget;

    /** The image layer */
    ImageLayerType::Pointer m_pImageLayer;

    /** StandardRenderingFunction */
    StandardRenderingFunctionType::Pointer m_RenderingFunction;
    
};

#endif // QOTBIMAGEVIEWER_H

#ifndef VRN_QGLOTBIMAGEVIEWERWIDGET_H
#define VRN_QGLOTBIMAGEVIEWERWIDGET_H

#include "modules/otb/processors/Visualization/otbimageviewerprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"
#include "../../ports/otbimageport.h"

#include "otbQGLImageWidget.h"

#include <QLabel>
#include <QString>
#include <QtGui>


using namespace otb;

namespace voreen {

//!
//! This is the main widget of the GeospatialClassificationProcessor
//!
class VRN_QT_API QGLOtbImageViewerWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    QGLOtbImageViewerWidget(QWidget*, OTBImageViewerProcessor* );

    virtual ~QGLOtbImageViewerWidget() { }

    void initialize();

    virtual void updateFromProcessor();
    
protected:
//    void mouseMoveEvent(QMouseEvent *);

signals:
    
public slots:
    //!


private slots:
    //!


private:
    static const std::string loggerCat_;

//    QString constructMetadataInfo();

//    ImageType *m_pImageType;

    QGLImageWidget *m_pQGLImageWidget;
};

}

#endif // VRN_QGLOTBIMAGEVIEWERWIDGET_H

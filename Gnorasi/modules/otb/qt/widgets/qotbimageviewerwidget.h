#ifndef VRN_QGLOTBIMAGEVIEWERWIDGET_H
#define VRN_QGLOTBIMAGEVIEWERWIDGET_H

#include <QLabel>
#include <QString>
#include <QtGui>

#include "modules/otb/processors/Visualization/otbimageviewerprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"
#include "../../ports/otbimageport.h"


namespace itiviewer{
    class ItiOtbImageViewer;
    class ItiOtbImageViewerFactory;
    class ItiOtbImageViewerPanel;
}

using namespace otb;

namespace voreen {

/*!
 * \brief The QGLOtbImageViewerWidget class
 *  This is the main widget of the GeospatialClassificationProcessor
 *  This widget accepts data from the processors which are connected with the
 *  OtbImageViewerProcessor. It has two key member variables : ItiOtbImageViewer and ItiOtbImageViewerFactory
 *  The ItiOtbImageViewer is the GUI class which hadles all the visualization stuff.
 *  The ItiOtbImageViewerFactory is the core class which is responsible for creating the
 *  ItiOtbImageViewer instances. Depending on the port a new ItiOtbImageViewerFactory instance
 *  is created.
 */
class VRN_QT_API QGLOtbImageViewerWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    QGLOtbImageViewerWidget(QWidget*, OTBImageViewerProcessor* );

    virtual ~QGLOtbImageViewerWidget();

    void initialize();

    virtual void updateFromProcessor();
    
protected:
    void keyPressEvent(QKeyEvent *);

signals:
    
public slots:
    //!


private slots:
    //!


private:
    //! this function creates and assembles all the widgets into one single widget
    void assembleWidgets();

    //! this function splits the layout
    void disassembleWidgets();

    static const std::string loggerCat_;

    //! this is the main widget of this viewer
    itiviewer::ItiOtbImageViewer *m_pItiOtbImageViewer;

    //! a factory responsible for creating ItiOtbImageViewer instances..
    itiviewer::ItiOtbImageViewerFactory *m_pItiOtbImageFactory;

    //! a panel widget for handling viewer parameters
    itiviewer::ItiOtbImageViewerPanel *m_pItiOtbImageViewerPanel;

    QSplitter *m_pvSplitter;
};

}

#endif // VRN_QGLOTBIMAGEVIEWERWIDGET_H

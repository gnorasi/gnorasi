#ifndef QGLOTBIMAGEMANAGER_H
#define QGLOTBIMAGEMANAGER_H

#include <QObject>

#include "otbImageLayer.h"
#include "otbImageLayerRenderingModel.h"
#include "otbRegionGlComponent.h"
#include "otbListenerBase.h"


#include "../widgets/qotbImageWidget.h"

using namespace otb;


/** Rendering model type */
typedef ImageLayerRenderingModel<ImageType> RenderingModelType;

/** Region gl component typedef */
typedef RegionGlComponent                       RegionGlComponentType;

/*!
 *
 *  This class is created in order to take over the functionality
 *  previously handled by the ImageView class declared in the
 *  otbImageView header file contained in the OTB library.
 *
 *  This is a singleton class in order the unique instance of this class to be
 *  used as an interface - console item , accessible from everywhere in
 *  the project.
 *
 *  This class has widgets as member variables that will be used in the main
 *  interface of the app.
 *
 */

class QGLotbImageManager : public QObject, public ListenerBase
{
public:
    //!
    static QGLotbImageManager* instance();

    //!
    static void deleteInstance();

    /**
     * This method unregister with previous model if any, and
     * register with the new one.
     */
    void setModel(RenderingModelType * model);

    /**
     * Manually trigger a refresh
     */
    void update();

    //! self explanatory
    QOtbImageWidget* scrollWidget() { return m_pScrollWidget; }

    //! self explanatory
    QOtbImageWidget* zoomWidget() { return m_pZoomWidget; }

    //! self explanatory
    QOtbImageWidget* fullWidget() { return m_pFullWidget; }

    /** Handle notification from the viewer */
    virtual void Notify();

protected:
    /** Update Scroll widget */
    void updateScrollWidget();

    /** Update Full widget */
    void updateFullWidget();

    /** Update Zoom widget */
    void updateZoomWidget();

private:
    QGLotbImageManager();

    ~QGLotbImageManager();

    //!
    static QGLotbImageManager* m_pInstance;

    /** The three classical widgets */
    QOtbImageWidget* m_pScrollWidget;
    QOtbImageWidget* m_pFullWidget;
    QOtbImageWidget* m_pZoomWidget;

    /** Viewed region gl components */
    RegionGlComponentType::Pointer m_pExtractRegionGlComponent;
    RegionGlComponentType::Pointer m_pScaledExtractRegionGlComponent;

    /** Model pointer */
    RenderingModelType *m_pModel;
};

#endif // QGLOTBIMAGEMANAGER_H

#ifndef QOTBIMAGEMANAGER_H
#define QOTBIMAGEMANAGER_H

#include <QObject>

#include "otbImageLayer.h"
#include "otbImageLayerRenderingModel.h"
#include "otbImageLayerGenerator.h"
#include "otbRegionGlComponent.h"
#include "otbListenerBase.h"
#include "otbHistogramCurve.h"
#include "otbPixelDescriptionModel.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataGlComponent.h"
#include "otbVectorDataExtractROI.h"
#include "otbRemoteSensingRegion.h"

#include "../widgets/qotbimagewidget.h"


using namespace otb;


/** Rendering model type */
typedef ImageLayerRenderingModel<ImageType>     RenderingModelType;

/** Region gl component typedef */
typedef RegionGlComponent                       RegionGlComponentType;

/** VectorData typedef */
typedef VectorData<double>                      VectorDataType;


/** Pixel description */
typedef PixelDescriptionModel<ImageType>        PixelDescriptionModelType;

/** VectorData overlay */
typedef VectorDataProjectionFilter
<VectorDataType, VectorDataType>                VectorDataProjectionFilterType;
typedef VectorDataExtractROI<VectorDataType>    VectorDataExtractROIType;
typedef VectorDataGlComponent<VectorDataType>   VectorDataGlComponentType;
typedef VectorDataFileReader<VectorDataType>    VectorDataFileReaderType;

/** Image layer type */
typedef ImageLayer<ImageType, ImageType>        ImageLayerType;

/** Image layer generator type */
typedef ImageLayerGenerator<ImageLayerType>       ImageLayerGeneratorType;


typedef HistogramCurve<ImageLayerType::HistogramType>           HistogramCurveType;

/** Rendering function */
typedef ImageLayerGeneratorType::RenderingFunctionType StandardRenderingFunctionType;

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
 *  This class has widgets as member variables, that will be used in the main
 *  user interface of the viewer's widget.
 *
 */

class QOTBImageManager : public QObject, public ListenerBase
{
public:
    //!
    static QOTBImageManager* instance();

    //!
    static void deleteInstance();

    /**
     * This method unregister with previous model if any, and
     * register with the new one.
     */
    void setModel(RenderingModelType * model);
    RenderingModelType* model() const { return m_pModel; }

    /**
     * This method sets the controller and passes it to the widgets.
     */
    void setController(ControllerType * controller);

    /**
     * Manually trigger a refresh
     */
    void update();

    //! self explanatory
    QOTBImageWidget* scrollWidget() { return m_pQOTBImageWidgetScroll; }

    //! self explanatory
    QOTBImageWidget* fullWidget() { return m_pQOTBImageWidgetFull; }

    //! self explanatory
    QOTBImageWidget* zoomWidget() { return m_pQOTBImageWidgetZoom; }


    /** Handle notification from the viewer */
    virtual void Notify();

    //! Loads an otbImage by parsing data given a file path parameter
    void load(const QString &path);

    //! Loads an ImageType image
    void load(ImageType* image);


    /** Set/Get the image to render */
    void setImage(ImageType *image) { m_pImage = image; }
    ImageType::Pointer image() const { return m_pImage; }

    /** Set/Get the VectorData to render */
    void setVectorData(VectorDataType *vd) { m_pVectorData = vd; }
    VectorDataType::Pointer vectorData() const { return m_pVectorData; }

    /** Set/Get the DEM directory */
    void setDemDirectory(const QString &d) { m_DEMDirectory = d; }
    QString demDirectory() const { return m_DEMDirectory; }

    /** Set/Get the DEM directory */
    void setGeoidFile(const QString &fp) { m_GeoidFile = fp; }
    QString geoidFile() const { return m_GeoidFile; }

protected:
    /** Update Scroll widget */
    void updateScrollWidget();

    /** Update Full widget */
    void updateFullWidget();

    /** Update Zoom widget */
    void updateZoomWidget();

private:
    QOTBImageManager();

    ~QOTBImageManager();

    //! this is the unique instance
    static QOTBImageManager* m_pInstance;

    /** The three classical widgets */
    QOTBImageWidget* m_pQOTBImageWidgetScroll;
    QOTBImageWidget* m_pQOTBImageWidgetFull;
    QOTBImageWidget* m_pQOTBImageWidgetZoom;

    /** Viewed region gl components */
    RegionGlComponentType::Pointer m_pExtractRegionGlComponent;
    RegionGlComponentType::Pointer m_pScaledExtractRegionGlComponent;

    /** Model pointer */
    RenderingModelType::Pointer m_pModel;

    /** Controller pointer */
    ControllerType::Pointer m_pController;

    /** Pointer to the image */
    ImageType::Pointer m_pImage;

    /** Pointer to the VectorData */
    VectorDataType::Pointer m_pVectorData;

    /** The pixel description model */
    PixelDescriptionModelType::Pointer m_pPixelDescriptionModel;

    /** Path to the DEMDirectory (used if a VectorData is rendered */
    QString m_DEMDirectory;

    /** GeoidFile filename (used if a VectorData is rendered */
    QString m_GeoidFile;
};

#endif // QOTBIMAGEMANAGER_H

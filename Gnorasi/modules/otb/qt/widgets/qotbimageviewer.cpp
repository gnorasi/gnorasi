#include "qotbimageviewer.h"

#include "qotbimagewidget.h"

#include <QFileDialog>
#include <QDebug>

#include <QSplitter>
#include <QVBoxLayout>

QOTBImageViewer::QOTBImageViewer(QWidget *parent) :
    m_pImageLayer(), QWidget(parent)
{
    setupLayout();
}

void QOTBImageViewer::setupLayout(){
    QOTBImageManager *manager = QOTBImageManager::instance();

    //!
    m_pQOTBImageWidgetScroll = manager->scrollWidget();
    m_pQOTBImageWidgetFull = manager->fullWidget();
    m_pQOTBImageWidgetZoom = manager->zoomWidget();

    m_pHistogramPlaceHolderWidget = new QWidget(this);
    m_pHistogramPlaceHolderWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QWidget *pWidgetLeft = new QWidget(this);

    QSplitter *verticalSplitterLeft = new QSplitter(Qt::Vertical,this);
    verticalSplitterLeft->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    verticalSplitterLeft->addWidget(m_pQOTBImageWidgetFull);
    verticalSplitterLeft->addWidget(m_pQOTBImageWidgetZoom);

    QVBoxLayout *vBoxLayoutLeft = new QVBoxLayout();
    vBoxLayoutLeft->addWidget(verticalSplitterLeft);
    vBoxLayoutLeft->addWidget(m_pHistogramPlaceHolderWidget);

    pWidgetLeft->setLayout(vBoxLayoutLeft);

    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal,this);

    horizontalSplitter->addWidget(pWidgetLeft);
    horizontalSplitter->addWidget(m_pQOTBImageWidgetScroll);

    QVBoxLayout *vboxlayout = new QVBoxLayout();

    vboxlayout->addWidget(horizontalSplitter);

    setLayout(vboxlayout);
}


void QOTBImageViewer::setCenter(IndexType index){
    QOTBImageManager *mgr = QOTBImageManager::instance();

    mgr->model()->SetExtractRegionCenter(index);
    mgr->model()->SetScaledExtractRegionCenter(index);
    mgr->model()->Update();
}


void QOTBImageViewer::update(){
    QOTBImageManager *mgr = QOTBImageManager::instance();

    ImageType::Pointer image = mgr->image();

    VectorDataType::Pointer vectorData = mgr->vectorData();

    RenderingModelType* model = mgr->model();

    // First check if there is actually an input image
    if (image.IsNull() || !model || vectorData.IsNull())
    {
//      itkExceptionMacro(
//        << "The image pointer is null, there is nothing to display. You probably forget to set the image.");
        return;
    }

    // Update image info for further use
    image->UpdateOutputInformation();

    VectorDataProjectionFilterType::Pointer vproj;
    VectorDataExtractROIType::Pointer       vdextract;

    // Colors
    HistogramCurveType::ColorType red, green, blue, gray;
    red.Fill(0);
    red[0] = 1.;
    red[3] = 0.5;

    green.Fill(0);
    green[1] = 1.;
    green[3] = 0.5;

    blue.Fill(0);
    blue[2] = 1.;
    blue[3] = 0.5;

    gray.Fill(0.6);

    // If there is a VectorData
    if (vectorData.IsNotNull())
    {
        // Extract The part of the VectorData that actually overlaps with
        // the image extent
        vdextract = VectorDataExtractROIType::New();
        vdextract->SetInput(vectorData);

        // Find the geographic region of interest

        // Ge the index of the corner of the image
        IndexType ul, ur, ll, lr;
        ImageType::PointType pul, pur, pll, plr;
        ul = image->GetLargestPossibleRegion().GetIndex();
        ur = ul;
        ll = ul;
        lr = ul;
        ur[0] += image->GetLargestPossibleRegion().GetSize()[0];
        lr[0] += image->GetLargestPossibleRegion().GetSize()[0];
        lr[1] += image->GetLargestPossibleRegion().GetSize()[1];
        ll[1] += image->GetLargestPossibleRegion().GetSize()[1];

        // Transform to physical point
        image->TransformIndexToPhysicalPoint(ul, pul);
        image->TransformIndexToPhysicalPoint(ur, pur);
        image->TransformIndexToPhysicalPoint(ll, pll);
        image->TransformIndexToPhysicalPoint(lr, plr);

        // Build the cartographic region
        VectorDataExtractROIType::RegionType rsRegion;
        VectorDataExtractROIType::RegionType::IndexType rsOrigin;
        VectorDataExtractROIType::RegionType::SizeType  rsSize;
        rsOrigin[0] = std::min(pul[0], plr[0]);
        rsOrigin[1] = std::min(pul[1], plr[1]);
        rsSize[0] = vcl_abs(pul[0] - plr[0]);
        rsSize[1] = vcl_abs(pul[1] - plr[1]);

        rsRegion.SetOrigin(rsOrigin);
        rsRegion.SetSize(rsSize);
        rsRegion.SetRegionProjection(image->GetProjectionRef());
        rsRegion.SetKeywordList(image->GetImageKeywordlist());

        // Set the cartographic region to the extract roi filter
        vdextract->SetRegion(rsRegion);
        vdextract->SetDEMDirectory(mgr->demDirectory().toStdString());
        vdextract->SetGeoidFile(mgr->geoidFile().toStdString());

        // Reproject VectorData in image projection
        vproj = VectorDataProjectionFilterType::New();
        vproj->SetInput(vdextract->GetOutput());
        vproj->SetInputProjectionRef(vectorData->GetProjectionRef());
        vproj->SetOutputKeywordList(image->GetImageKeywordlist());
        vproj->SetOutputProjectionRef(image->GetProjectionRef());
        vproj->SetOutputOrigin(image->GetOrigin());
        vproj->SetOutputSpacing(image->GetSpacing());
        vproj->SetDEMDirectory(mgr->demDirectory().toStdString());
        vproj->SetGeoidFile(mgr->geoidFile().toStdString());
        vproj->Update();

        // Create a VectorData gl component
        VectorDataGlComponentType::Pointer vgl = VectorDataGlComponentType::New();
        vgl->SetVectorData(vproj->GetOutput());
        vgl->SetColor(blue);
        // Add it to the image view
        mgr->scrollWidget()->AddGlComponent(vgl);
        mgr->fullWidget()->AddGlComponent(vgl);
        mgr->zoomWidget()->AddGlComponent(vgl);
    }

    // Generate the layer
//    QImageLayerGeneratorType::Pointer generator = QImageLayerGeneratorType::New();
//    generator->SetImage(image);
//    FltkFilterWatcher qlwatcher(generator->GetProgressSource(), 0, 0, 200, 20, "Generating QuickLook ...");
//    generator->GenerateLayer();
//    m_pImageLayer = generator->GetLayer();
//    m_RenderingFunction = generator->GetRenderingFunction();

    // Set the window and layer label
//    m_DisplayWindow->SetLabel(m_Label.c_str());
//    m_pImageLayer->SetName(m_Label);

    // Add the generated layer to the rendering model
//    model->AddLayer(generator->GetLayer());

    // Show everything
//    m_DisplayWindow->Show();

    // Update the rendering model
    model->Update();

    // adding histograms rendering
//    unsigned int listSize = m_ImageLayer->GetHistogramList()->Size();

//    if (listSize > 0)
//    {
//        if (listSize == 1)
//        {
//            HistogramCurveType::Pointer grayhistogram = HistogramCurveType::New();
//            grayhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(0));
//            grayhistogram->SetHistogramColor(gray);
//            grayhistogram->SetLabelColor(gray);
//            m_CurveWidget->AddCurve(grayhistogram);
//        }
//        else
//        {
//            HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
//            rhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(0));
//            rhistogram->SetHistogramColor(red);
//            rhistogram->SetLabelColor(red);
//            m_CurveWidget->AddCurve(rhistogram);
//        }
//        if (listSize > 1)
//        {
//            HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
//            ghistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(1));
//            ghistogram->SetHistogramColor(green);
//            ghistogram->SetLabelColor(green);
//            m_CurveWidget->AddCurve(ghistogram);
//        }
//        if (listSize > 2)
//        {
//            HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
//            bhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(2));
//            bhistogram->SetHistogramColor(blue);
//            bhistogram->SetLabelColor(blue);
//            m_CurveWidget->AddCurve(bhistogram);
//        }
//    }

//    m_CurveWidget->SetXAxisLabel("Pixels");
//    m_CurveWidget->SetYAxisLabel("Frequency");
}

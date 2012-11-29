#include "qotbimagemanager.h"

#include "otbImageFileReader.h"

using namespace otb;

typedef otb::ImageFileReader<ImageType>   ReaderType;

QOTBImageManager* QOTBImageManager::m_pInstance = NULL;

QOTBImageManager::QOTBImageManager()
    : m_pModel(), m_pVectorData(), m_pController(), m_pExtractRegionGlComponent(), m_pScaledExtractRegionGlComponent()
{
    // Build a new rendering model
    m_pModel = RenderingModelType::New();
    m_pPixelDescriptionModel = PixelDescriptionModelType::New();
    m_pPixelDescriptionModel->SetLayers(m_pModel->GetLayers());

    m_pVectorData = VectorDataType::New();

    // create the views
    m_pQOTBImageWidgetScroll = new QOTBImageWidget();
    m_pQOTBImageWidgetFull = new QOTBImageWidget();
    m_pQOTBImageWidgetZoom = new QOTBImageWidget();

    // Extract regions gl components
    m_pExtractRegionGlComponent = RegionGlComponentType::New();
    m_pScaledExtractRegionGlComponent = RegionGlComponentType::New();
    m_pExtractRegionGlComponent->SetVisible(false);
    m_pScaledExtractRegionGlComponent->SetVisible(false);
    m_pQOTBImageWidgetScroll->AddGlComponent(m_pExtractRegionGlComponent);
    m_pQOTBImageWidgetFull->AddGlComponent(m_pScaledExtractRegionGlComponent);

    m_pQOTBImageWidgetScroll->setWindowTitle(tr("Scroll"));
    m_pQOTBImageWidgetFull->setWindowTitle(tr("Full"));
    m_pQOTBImageWidgetZoom->setWindowTitle(tr("Zoom"));
}

QOTBImageManager* QOTBImageManager::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new QOTBImageManager();
    }

    return m_pInstance;
}

void QOTBImageManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;

        m_pInstance = NULL;
    }
}

void QOTBImageManager::load(const QString &path){

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(path.toStdString());
    reader->Update();
    reader->GenerateOutputInformation();

    ImageType::Pointer image = reader->GetOutput();
    setImage(image);

    RegionType region = image->GetLargestPossibleRegion();


    m_pQOTBImageWidgetScroll->setIsotropicZoom(1);
    m_pQOTBImageWidgetFull->setIsotropicZoom(0.5);
    m_pQOTBImageWidgetScroll->ReadBuffer(image, region);
    m_pQOTBImageWidgetFull->ReadBuffer(image,region);
}


void QOTBImageManager::load(ImageType *image){
    setImage(image);

    // get the region
    RegionType region = image->GetLargestPossibleRegion();

    m_pQOTBImageWidgetScroll->setIsotropicZoom(1);
    m_pQOTBImageWidgetFull->setIsotropicZoom(0.5);
    m_pQOTBImageWidgetScroll->ReadBuffer(image,region);
    m_pQOTBImageWidgetFull->ReadBuffer(image,region);
}


void QOTBImageManager::Notify()
{
  this->update();
}


void QOTBImageManager::setModel(RenderingModelType * model)
{
  // Unregister from previous model if nay
  if (m_pModel.IsNotNull())
  {
    m_pModel->UnRegisterListener(this);
  }

  // Set and register with new model
  m_pModel = model;
  m_pModel->RegisterListener(this);
}


void QOTBImageManager::setController(ControllerType * controller)
{
  m_pController = controller;
  m_pQOTBImageWidgetScroll->setController(m_pController);
  m_pQOTBImageWidgetFull->setController(m_pController);
  m_pQOTBImageWidgetZoom->setController(m_pController);
}


void QOTBImageManager::update()
{
  this->updateScrollWidget();
  this->updateFullWidget();
  this->updateZoomWidget();
}


void QOTBImageManager::updateScrollWidget()
{
  // If the model has a quicklook
  if (m_pModel->GetHasQuicklook())
    {
//    otbMsgDevMacro(<< "ImageView::UpdateScrollWidget(): redrawing scroll widget");

    // Read the buffer
    const RegionType qlRegion =  m_pModel->GetRasterizedQuicklook()
                                ->GetLargestPossibleRegion();
    m_pQOTBImageWidgetScroll->ReadBuffer(m_pModel->GetRasterizedQuicklook(), qlRegion);

    // Compute the appropriate scale
    const double wratio = static_cast<double>(m_pQOTBImageWidgetScroll->width()) / static_cast<double>(qlRegion.GetSize()[0]);
    const double hratio = static_cast<double>(m_pQOTBImageWidgetScroll->height()) / static_cast<double>(qlRegion.GetSize()[1]);
    const double scale = std::min(wratio, hratio);
    m_pQOTBImageWidgetScroll->setIsotropicZoom(scale);
    m_pQOTBImageWidgetScroll->setSubSamplingRate(m_pModel->GetSubsamplingRate());

    // Setting widget label
//    std::string label = m_pQOTBImageWidgetScroll->GetIdentifier();
//    label += (" - ");
//    label += m_pModel->GetName();
//    m_pQOTBImageWidgetScroll->label(label.c_str());

    // display the zoom rectangle if necessary
    if (m_pModel->GetHasExtract())
      {
      m_pExtractRegionGlComponent->SetVisible(true);
      m_pExtractRegionGlComponent->SetRegion(m_pModel->GetExtractRegion());
      }
    else
      {
      m_pExtractRegionGlComponent->SetVisible(false);
      }
    }
  else
    {
    // Else, there is nothing to display
    // Ensure to reset buffer
    m_pQOTBImageWidgetScroll->ClearBuffer();
    }

  // Redraw
  m_pQOTBImageWidgetScroll->updateGL();
}


void QOTBImageManager::updateFullWidget()
{
  if (m_pModel->GetHasExtract())
    {
    otbMsgDevMacro(<< "ImageView::UpdateFullWidget(): redrawing full widget");
    m_pQOTBImageWidgetFull->ReadBuffer(m_pModel->GetRasterizedExtract(), m_pModel->GetRasterizedExtract()
                             ->GetLargestPossibleRegion());

    // Setting widget label
//    std::string label = m_pQOTBImageWidgetFull->GetIdentifier();
//    label += (" - ");
//    label += m_pModel->GetName();
//    m_pQOTBImageWidgetFull->label(label.c_str());

    // display the zoom rectangle if necessary
    if (m_pModel->GetHasScaledExtract())
      {
      m_pScaledExtractRegionGlComponent->SetVisible(true);
      m_pScaledExtractRegionGlComponent->SetRegion(m_pModel->GetScaledExtractRegion());
      }
    else
      {
      m_pScaledExtractRegionGlComponent->SetVisible(false);
      }
    }
  else
    {
    // Else, there is nothing to display
    // Ensure to reset buffer
    m_pQOTBImageWidgetFull->ClearBuffer();
    }

  // redraw the widget
  // Redraw
  m_pQOTBImageWidgetFull->updateGL();
}

//!
void QOTBImageManager::updateZoomWidget()
{
  if (m_pModel->GetHasScaledExtract())
    {
//    otbMsgDevMacro(<< "ImageView::UpdateZoomWidget(): redrawing zoom widget");
    m_pQOTBImageWidgetZoom->ReadBuffer(m_pModel->GetRasterizedScaledExtract(), m_pModel->GetRasterizedScaledExtract()
                             ->GetLargestPossibleRegion());

    // Setting widget label
//    std::string label = m_pQOTBImageWidgetZoom->GetIdentifier();
//    label += (" - ");
//    label += m_pModel->GetName();
//    m_pQOTBImageWidgetZoom->label(label.c_str());
    }
  else
    {
    // Else, there is nothing to display
    // Ensure to reset buffer
    m_pQOTBImageWidgetZoom->ClearBuffer();
    }

  // Redraw
  //
  m_pQOTBImageWidgetZoom->updateGL();
}


QOTBImageManager::~QOTBImageManager(){
    if(m_pQOTBImageWidgetFull)
        delete m_pQOTBImageWidgetFull;
    if(m_pQOTBImageWidgetScroll)
        delete m_pQOTBImageWidgetScroll;
    if(m_pQOTBImageWidgetZoom)
        delete m_pQOTBImageWidgetZoom;

    //
    //
    // TODO
    // check that the regions pointers are also deleted
    // could not delete them beacause the destructor is protected
    //
}

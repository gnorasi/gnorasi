#include "qglotbimagemanager.h"

//#include "otbImageView.h"

using namespace otb;

QGLotbImageManager* QGLotbImageManager::m_pInstance = NULL;

QGLotbImageManager::QGLotbImageManager()
{
    m_pScrollWidget = new QOtbImageWidget();
    m_pFullWidget = new QOtbImageWidget();
    m_pZoomWidget = new QOtbImageWidget();

    // Extract regions gl components
    m_pExtractRegionGlComponent = RegionGlComponentType::New();
    m_pScaledExtractRegionGlComponent = RegionGlComponentType::New();
    m_pExtractRegionGlComponent->SetVisible(false);
    m_pScaledExtractRegionGlComponent->SetVisible(false);
    m_pScrollWidget->AddGlComponent(m_pExtractRegionGlComponent);
    m_pFullWidget->AddGlComponent(m_pScaledExtractRegionGlComponent);
}

QGLotbImageManager* QGLotbImageManager::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new QGLotbImageManager();
    }

    return m_pInstance;
}

void QGLotbImageManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;

        m_pInstance = NULL;
    }
}


void
QGLotbImageManager::Notify()
{
  this->update();
}


void
QGLotbImageManager::setModel(RenderingModelType * model)
{
  // Unregister from previous model if nay
  if (m_pModel)
  {
    m_pModel->UnRegisterListener(this);
  }

  // Set and register with new model
  m_pModel = model;
  m_pModel->RegisterListener(this);
}


void
QGLotbImageManager::update()
{
  this->updateScrollWidget();
  this->updateFullWidget();
  this->updateZoomWidget();
}


void
QGLotbImageManager::updateScrollWidget()
{
  // If the model has a quicklook
  if (m_pModel->GetHasQuicklook())
    {
//    otbMsgDevMacro(<< "ImageView::UpdateScrollWidget(): redrawing scroll widget");

    // Read the buffer
    const RegionType qlRegion =  m_pModel->GetRasterizedQuicklook()
                                ->GetLargestPossibleRegion();
    m_pScrollWidget->ReadBuffer(m_pModel->GetRasterizedQuicklook(), qlRegion);

    // Compute the appropriate scale
    const double wratio = static_cast<double>(m_pScrollWidget->width()) / static_cast<double>(qlRegion.GetSize()[0]);
    const double hratio = static_cast<double>(m_pScrollWidget->height()) / static_cast<double>(qlRegion.GetSize()[1]);
    const double scale = std::min(wratio, hratio);
    m_pScrollWidget->setIsotropicZoom(scale);
    m_pScrollWidget->setSubSamplingRate(m_pModel->GetSubsamplingRate());

    // Setting widget label
//    std::string label = m_pScrollWidget->GetIdentifier();
//    label += (" - ");
//    label += m_pModel->GetName();
//    m_pScrollWidget->label(label.c_str());

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
    m_pScrollWidget->ClearBuffer();
    }

  // Redraw
  //
  //
  // TODO
  //
  // REDRAW in Qt openGL
  //
  //
//  m_pScrollWidget->redraw();
}


void
QGLotbImageManager::updateFullWidget()
{
  if (m_pModel->GetHasExtract())
    {
    otbMsgDevMacro(<< "ImageView::UpdateFullWidget(): redrawing full widget");
    m_pFullWidget->ReadBuffer(m_pModel->GetRasterizedExtract(), m_pModel->GetRasterizedExtract()
                             ->GetLargestPossibleRegion());

    // Setting widget label
//    std::string label = m_pFullWidget->GetIdentifier();
//    label += (" - ");
//    label += m_pModel->GetName();
//    m_pFullWidget->label(label.c_str());

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
    m_pFullWidget->ClearBuffer();
    }

  // redraw the widget
  // Redraw
  //
  //
  // TODO
  //
  // REDRAW in Qt openGL
  //
  //
//  m_pFullWidget->redraw();
}

//!
void
QGLotbImageManager::updateZoomWidget()
{
  if (m_pModel->GetHasScaledExtract())
    {
//    otbMsgDevMacro(<< "ImageView::UpdateZoomWidget(): redrawing zoom widget");
    m_pZoomWidget->ReadBuffer(m_pModel->GetRasterizedScaledExtract(), m_pModel->GetRasterizedScaledExtract()
                             ->GetLargestPossibleRegion());

    // Setting widget label
//    std::string label = m_pZoomWidget->GetIdentifier();
//    label += (" - ");
//    label += m_pModel->GetName();
//    m_pZoomWidget->label(label.c_str());
    }
  else
    {
    // Else, there is nothing to display
    // Ensure to reset buffer
    m_pZoomWidget->ClearBuffer();
    }

  // Redraw
  //
  //
  // TODO
  //
  // REDRAW in Qt openGL
  //
  //
//  m_pZoomWidget->redraw();
}


QGLotbImageManager::~QGLotbImageManager(){
    if(m_pFullWidget)
        delete m_pFullWidget;
    if(m_pScrollWidget)
        delete m_pScrollWidget;
    if(m_pZoomWidget)
        delete m_pZoomWidget;

    //
    //
    // TODO
    // check that the regions pointers are also deleted
    // could not delete them beacause the destructor is protected
    //
}

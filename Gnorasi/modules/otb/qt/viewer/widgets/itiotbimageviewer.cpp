#include "itiotbimageviewer.h"

#include "../models/itiotbAbstractImageModel.h"

#include "../utils/itiotbimagemanager.h"

using namespace itiviewer;

ItiOtbImageViewer::ItiOtbImageViewer(QWidget *parent)
    : QWidget(parent)
{
    m_pManager = new ItiOtbImageManager(this);
}

void ItiOtbImageViewer::setModel( AbstractModel* model )
{
//    if(m_pAbstractImageModel)
//        delete m_pAbstractImageModel;

    m_pAbstractImageModel = model;
    connect(m_pAbstractImageModel,SIGNAL(ready()),this,SIGNAL(ready()));
    m_pAbstractImageModel ->setParent( this );
}

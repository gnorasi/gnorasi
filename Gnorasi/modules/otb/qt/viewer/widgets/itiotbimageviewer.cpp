#include "itiotbimageviewer.h"

#include "../models/itiotbAbstractImageModel.h"

using namespace itiviewer;

void ItiOtbImageViewer::setModel( AbstractModel* model )
{
//    if(m_pAbstractImageModel)
//        delete m_pAbstractImageModel;

    m_pAbstractImageModel = model;
    m_pAbstractImageModel ->setParent( this );
}

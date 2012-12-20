#include "itiotbimagergbachannelprovider.h"

using namespace itiviewer;

ItiOtbImageRgbaChannelProvider::ItiOtbImageRgbaChannelProvider(RasterImageType *imgType, QObject *parent) :
    m_pRasterImageType(imgType), ItiOtbImageChannelProvider(parent)
{
}

//!
void ItiOtbImageRgbaChannelProvider::parseImage(){
    //! TODO parse the image
}

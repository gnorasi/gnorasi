#include "itiotbimagevectorchannelprovider.h"

using namespace itiviewer;

ItiOtbImageVectorChannelProvider::ItiOtbImageVectorChannelProvider(VectorImageType *imgType, QObject *parent) :
    m_pVectorImageType(imgType), ItiOtbImageChannelProvider(parent)
{
}

//!
void ItiOtbImageVectorChannelProvider::parseImage(){
    //! TODO parse the image
}

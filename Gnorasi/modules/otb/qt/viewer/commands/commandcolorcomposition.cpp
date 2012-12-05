#include "commandcolorcomposition.h"

#include "../itiotbimageviewer.h"

using namespace itiviewer;

CommandColorComposition::CommandColorComposition(ItiOtbImageViewer *viewer, QObject *parent)
    : m_pItiOtbImageViewer(viewer), Command(parent)
{
    m_isGreyScale   = true;
    m_greyChannel   = 1;
    m_redChannel    = 1;
    m_greenChannel  = 2;
    m_blueChannel   = 3;
}

//!
void CommandColorComposition::setGreyScaleMethod(int channel){
    m_isGreyScale = true;
    m_greyChannel = channel;
}

//!
void CommandColorComposition::setRGBMethod(int red, int green, int blue){
    m_isGreyScale = false;
    m_redChannel = red;
    m_greenChannel = green;
    m_blueChannel = blue;
}

//!
void CommandColorComposition::execute(){
    if(m_isGreyScale)
        m_pItiOtbImageViewer->applyGreyScaleColorMode(m_greyChannel);
    else
        m_pItiOtbImageViewer->applyRGBColorMode(m_redChannel,m_greenChannel,m_blueChannel);
}

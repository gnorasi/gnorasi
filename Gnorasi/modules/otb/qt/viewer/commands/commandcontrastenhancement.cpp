#include "commandcontrastenhancement.h"

using namespace itiviewer;

//!
CommandContrastEnhancement::CommandContrastEnhancement(ItiOtbImageViewer *viewer, QObject *parent)
    : m_pItiOtbImageViewer(viewer), Command(parent)
{
    m_standardDeviation = 1.0;
    m_upperQuantile     = 2.0;
    m_lowerQuantile     = 2.0;
}

//!
void CommandContrastEnhancement::setContrastEnhancementMethod(int method, double aval, double bval){
    m_method = (METHOD)method;
}

//!
void CommandContrastEnhancement::execute(){

}

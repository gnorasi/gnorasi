#include "commandcontrastenhancementlinearxperc.h"

using namespace itiviewer;

CommandContrastEnhancementLinearXPerc::CommandContrastEnhancementLinearXPerc(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_upperQuantile(2.0), m_lowerQuantile(2.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementLinearXPerc::execute(){
    //! TODO
    //! apply the filter here
}

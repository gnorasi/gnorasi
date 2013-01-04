#include "commandcontrastenhancementsquareroot.h"

using namespace itiviewer;

CommandContrastEnhancementSquareRoot::CommandContrastEnhancementSquareRoot(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_upperQuantile(2.0), m_lowerQuantile(2.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementSquareRoot::execute(){
    //! TODO
    //! apply the filter here
}

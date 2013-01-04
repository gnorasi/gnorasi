#include "commandcontrastenhancementgaussian.h"

using namespace itiviewer;

CommandContrastEnhancementGaussian::CommandContrastEnhancementGaussian(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_deviation(1.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementGaussian::execute(){
    //! TODO
    //! apply the filter here
}

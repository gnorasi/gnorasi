#include <QtCore>
#include <QtGui>

#include "commandcontrastenhancementsquareroot.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"
#include "../models/itiotbVectorImageModel.h"
#include "../vector_globaldefs.h"

using namespace itiviewer;
using namespace otb;

CommandContrastEnhancementSquareRoot::CommandContrastEnhancementSquareRoot(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_upperQuantile(2.0), m_lowerQuantile(2.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementSquareRoot::execute(){
    VectorImageModel *vModel = qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());
    if(!vModel)
        return;

    RenderingFilterType *filter = vModel->filter();

    RenderingFunctionType *renderer = dynamic_cast<RenderingFunctionType*>(SquareRootRenderingFunctionType::New().GetPointer());

    if(filter)
        filter->SetRenderingFunction(renderer);

    m_pItiOtbVectorImageViewer->draw();
}

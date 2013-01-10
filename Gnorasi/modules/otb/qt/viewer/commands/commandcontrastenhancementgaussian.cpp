#include <QtCore>
#include <QtGui>

#include "commandcontrastenhancementgaussian.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"
#include "../models/itiotbVectorImageModel.h"
#include "../vector_globaldefs.h"

#include "otbGaussianRenderingFunction.h"


using namespace itiviewer;
using namespace otb;

CommandContrastEnhancementGaussian::CommandContrastEnhancementGaussian(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_deviation(1.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementGaussian::execute(){

    VectorImageModel *vModel = qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());
    if(!vModel)
        return;

    RenderingFilterType *filter = vModel->filter();

    RenderingFunctionType *renderer = dynamic_cast<RenderingFunctionType*>(GaussianRenderingFunctionType::New().GetPointer());

    if(filter)
        filter->SetRenderingFunction(renderer);

    m_pItiOtbVectorImageViewer->draw();
}

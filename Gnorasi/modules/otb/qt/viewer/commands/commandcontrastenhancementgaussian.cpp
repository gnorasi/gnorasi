#include "commandcontrastenhancementgaussian.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"

#include "itkImageRegionIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkGaussianOperator.h"

#include "otbVectorImageToImageListFilter.h"

#include "../utils/itiotbimagemanager.h"

#include "otbImageFileReader.h"

using namespace itiviewer;

CommandContrastEnhancementGaussian::CommandContrastEnhancementGaussian(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_deviation(1.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementGaussian::execute(){

    //!
//    VectorImageType* imgType =  (VectorImageType*)ITIOTBIMAGEMANAGER->image();
//    if(!imgType)
//        return;

//    VectorImageToImageListType::Pointer imageList = VectorImageToImageListType::New();
//    imageList->SetInput(imgType);

//    imageList->UpdateOutputInformation();

//    const unsigned int nbBands = imgType->GetNumberOfComponentsPerPixel();

//    for(unsigned int j = 0; j < nbBands; ++j){
//        RasterImageType *input = (RasterImageType*)imageList->GetOutput()->GetNthElement(j);

    voreen::OTBImagePort *pPort = dynamic_cast<voreen::OTBImagePort*>(ITIOTBIMAGEMANAGER->port());

    if(!pPort)
        return;

    RasterImageType *input = (RasterImageType*)pPort->getData();
    if(!input)
        return;

    RasterImageType::Pointer output = RasterImageType::New();
    output->SetRegions(input->GetRequestedRegion());
    output->Allocate();

    itk::NeighborhoodInnerProduct<RasterImageType> innerProduct;

    FaceCalculatorType                         faceCalculator;
    FaceCalculatorType::FaceListType           faceList;
    FaceCalculatorType::FaceListType::iterator fit;

    IteratorType             out;
    NeighborhoodIteratorType it;

    itk::GaussianOperator<PixelType, Dimension> gaussianOperator;
    gaussianOperator.SetVariance(m_deviation);

    for (unsigned int i = 0; i < RasterImageType::ImageDimension; ++i)
    {
        gaussianOperator.SetDirection(i);
        gaussianOperator.CreateDirectional();

        faceList = faceCalculator(input, output->GetRequestedRegion(),
                                gaussianOperator.GetRadius());

        for (fit = faceList.begin(); fit != faceList.end(); ++fit)
        {
            it = NeighborhoodIteratorType(gaussianOperator.GetRadius(),
                                          input, *fit);

            out = IteratorType(output, *fit);

            for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
            {
                out.Set(innerProduct(it, gaussianOperator));
            }
        }

        // Swap the input and output buffers
        if (i != RasterImageType::ImageDimension - 1)
        {
            RasterImageType::Pointer tmp = input;
            input = output;
            output = tmp;
        }
    }

    ImageToVectorImageCastFilterType::Pointer filter = ImageToVectorImageCastFilterType::New();

    filter->SetInput(output);

    VectorImageType* vImage = filter->GetOutput();

    filter->Update();

    ITIOTBIMAGEMANAGER->setImage(vImage);
//    }

    m_pItiOtbVectorImageViewer->draw();
}

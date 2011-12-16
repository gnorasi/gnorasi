/*=========================================================================

  Program:   GNORASI NDWI Calculator Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkMacro.h"
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "otbVectorImageToImageListFilter.h"


#include "otbBandMathImageFilter.h"

int main( int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile outputImageFile8bit";
    return EXIT_FAILURE;
  }

  typedef double                                                          PixelType;
  typedef otb::VectorImage<PixelType, 2>                                  InputImageType;
  typedef otb::Image<PixelType, 2>                                        OutputImageType;
  typedef otb::ImageList<OutputImageType>                                 ImageListType;
  typedef OutputImageType::PixelType                                      VPixelType;
  typedef otb::VectorImageToImageListFilter<InputImageType, ImageListType>
  VectorImageToImageListType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                           WriterType;

  typedef otb::BandMathImageFilter<OutputImageType>   FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  FilterType::Pointer filter = FilterType::New();

  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->UpdateOutputInformation();

  VectorImageToImageListType::Pointer imageList = VectorImageToImageListType::New();
  imageList->SetInput(reader->GetOutput());

  imageList->UpdateOutputInformation();

  const unsigned int nbBands = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  for(unsigned int j = 0; j < nbBands; ++j)
  {
    filter->SetNthInput(j, imageList->GetOutput()->GetNthElement(j));
  }

  filter->SetExpression("if( (b4 != 0), (b4-b5)/(b4+b5), 0)");

  writer->Update();
  
  return EXIT_SUCCESS;
}

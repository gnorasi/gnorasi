/*=========================================================================

  Program:   GNORASI Two Images Band Math Application
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
  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile1 ";
    std::cerr << " inputImageFile2 outputImageFile outputPrettyImageFile ";
    std::cerr << " MathExpression " << std::endl;
    std::cerr << "e.g. \"(b4-b3)\"" << std::endl;
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

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  FilterType::Pointer filter = FilterType::New();

  writer->SetInput(filter->GetOutput());
  reader1->SetFileName(argv[1]);
  reader2->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);

  reader1->UpdateOutputInformation();
  reader2->UpdateOutputInformation();

  VectorImageToImageListType::Pointer imageList1 = VectorImageToImageListType::New();
  imageList1->SetInput(reader1->GetOutput());

  imageList1->UpdateOutputInformation();

  const unsigned int nbBands1 = reader1->GetOutput()->GetNumberOfComponentsPerPixel();

  for(unsigned int j = 0; j < nbBands1; ++j)
  {
    filter->SetNthInput(j, imageList1->GetOutput()->GetNthElement(j));
  }
  
  VectorImageToImageListType::Pointer imageList2 = VectorImageToImageListType::New();
  imageList2->SetInput(reader2->GetOutput());

  imageList2->UpdateOutputInformation();

  const unsigned int nbBands2 = reader2->GetOutput()->GetNumberOfComponentsPerPixel();
  int k = nbBands1;

  for(unsigned int j = 0; j < nbBands2; ++j)
  {
    filter->SetNthInput(k, imageList2->GetOutput()->GetNthElement(j));
    k++;
  }
  
  filter->SetExpression(argv[5]);

  writer->Update();
  
  typedef otb::Image<unsigned char, 2>                                      OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>            PrettyImageFileWriterType;
  typedef itk::CastImageFilter<OutputImageType, OutputPrettyImageType> CastImageFilterType;
  
  PrettyImageFileWriterType::Pointer prettyWriter = PrettyImageFileWriterType::New();
  CastImageFilterType::Pointer caster = CastImageFilterType::New();
  caster->SetInput(filter->GetOutput());

  prettyWriter->SetInput(caster->GetOutput());
  prettyWriter->SetFileName(argv[4]);
  
  prettyWriter->Update();

  return EXIT_SUCCESS;
}

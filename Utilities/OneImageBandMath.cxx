/*=========================================================================

  Program:   GNORASI One Image Band Math Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  
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
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile ";
    std::cerr << " outputPrettyImageFile MathExpression" << std::endl;
    std::cerr << "e.g. \"if((b4-b3)/(b4+b3) > 0.4, 255, 0)\"" << std::endl;
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

  //filter->SetExpression("if((b4-b3)/(b4+b3) > 0.4, 255, 0)");
  filter->SetExpression(argv[4]);
  
  writer->Update();

  typedef otb::Image<unsigned char, 2>                                      OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>            PrettyImageFileWriterType;
  typedef itk::CastImageFilter<OutputImageType, OutputPrettyImageType> CastImageFilterType;
  
  PrettyImageFileWriterType::Pointer prettyWriter = PrettyImageFileWriterType::New();
  CastImageFilterType::Pointer caster = CastImageFilterType::New();
  caster->SetInput(filter->GetOutput());

  prettyWriter->SetInput(caster->GetOutput());
  prettyWriter->SetFileName(argv[3]);
  
  prettyWriter->Update();
  
  return EXIT_SUCCESS;
}

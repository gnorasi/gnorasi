/*=========================================================================

  Program:   GNORASI Binary Oppening Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

int main(int argc, char * argv[])
{
  if (argc < 3 || argc > 4)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  ";
    std::cerr << " outputImageFileOpenning [radius]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;


  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<
      InputPixelType,
      Dimension>             StructuringElementType;

  typedef itk::BinaryErodeImageFilter<
      InputImageType,
      OutputImageType,
      StructuringElementType>  ErodeFilterType;

  typedef itk::BinaryDilateImageFilter<
      InputImageType,
      OutputImageType,
      StructuringElementType>  DilateFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  ErodeFilterType::Pointer  binaryErode  = ErodeFilterType::New();
  DilateFilterType::Pointer binaryDilate = DilateFilterType::New();

  StructuringElementType structuringElement;
  
  int rad = 1;
  
  if(argc == 4)
  {
    rad = atoi(argv[3]);
  }

  structuringElement.SetRadius(rad);    // 3x3 structuring element

  structuringElement.CreateStructuringElement();

  binaryErode->SetKernel(structuringElement);
  binaryDilate->SetKernel(structuringElement);


  reader->SetFileName(argv[1]);

  writer->SetFileName(argv[2]);


  InputPixelType foreground = 255;

  binaryErode->SetInput(reader->GetOutput());
  binaryDilate->SetInput(binaryErode->GetOutput());

  binaryErode->SetErodeValue(foreground);
  binaryDilate->SetDilateValue(foreground);

  writer->SetInput(binaryDilate->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

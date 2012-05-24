/*=========================================================================

  Program:   GNORASI Canny Edge Detection Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkCannyEdgeDetectionImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [variance]" << std::endl;
    return EXIT_FAILURE;
  }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  float        variance = 2.0;

  if (argc > 3)
  {
    variance = atof(argv[3]);
  }

  typedef unsigned char CharPixelType;     
  typedef double        RealPixelType;     
  const unsigned int Dimension = 2;

  typedef otb::Image<CharPixelType, Dimension> CharImageType;
  typedef otb::Image<RealPixelType, Dimension> RealImageType;

  typedef otb::ImageFileReader<RealImageType> ReaderType;
  typedef otb::ImageFileWriter<CharImageType> WriterType;

  typedef itk::RescaleIntensityImageFilter<RealImageType,
      CharImageType> RescaleFilter;

  typedef itk::CannyEdgeDetectionImageFilter<RealImageType,
      RealImageType> CannyFilter;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  RescaleFilter::Pointer rescale = RescaleFilter::New();


  CannyFilter::Pointer cannyFilter = CannyFilter::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  rescale->SetOutputMinimum(0);
  rescale->SetOutputMaximum(255);

  cannyFilter->SetInput(reader->GetOutput());
  cannyFilter->SetVariance(variance);
  rescale->SetInput(cannyFilter->GetOutput());
  writer->SetInput(rescale->GetOutput());

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}

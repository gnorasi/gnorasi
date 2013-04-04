/*=========================================================================

  Program:   GNORASI Adaptive Histogram Equalization Application
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
#include "otbPerBandVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

#include "itkAdaptiveHistogramEqualizationImageFilter.h"

int main( int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " alpha beta inputImageFile ";
    std::cerr << " outputImageFile ";
    return EXIT_FAILURE;
    }

  typedef double                                                          PixelType;
  typedef otb::VectorImage<PixelType, 2>                                  InputVectorImageType;
  typedef otb::Image<PixelType, 2>					  InputImageType;
  typedef otb::VectorImage<PixelType, 2>                            	  OutputImageType;
  typedef otb::ImageFileReader<InputVectorImageType>                      ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                           WriterType;
  
  typedef itk::AdaptiveHistogramEqualizationImageFilter<InputImageType> FilterType;
  typedef otb::PerBandVectorImageFilter<InputVectorImageType, OutputImageType, FilterType> VectorFilterType;
  
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  VectorFilterType::Pointer vfilter = VectorFilterType::New();
  FilterType::Pointer filter = FilterType::New();
  
  const double alpha = atof(argv[1]);
  const double beta = atof(argv[2]);
  //const unsigned long radius = atoi(argv[3]);
  

  writer->SetInput(vfilter->GetOutput());
  reader->SetFileName(argv[3]);
  writer->SetFileName(argv[4]);

  reader->UpdateOutputInformation();

  //filter->SetRadius(radius);
  filter->SetAlpha(alpha);
  filter->SetBeta(beta);
  vfilter->SetFilter(filter);
  vfilter->SetInput(reader->GetOutput());
  
  writer->Update();

  return EXIT_SUCCESS;
}

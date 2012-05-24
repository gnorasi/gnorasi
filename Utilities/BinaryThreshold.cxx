/*=========================================================================

  Program:   GNORASI Binary Thresholding Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkBinaryThresholdImageFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
  if (argc < 7)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFile ";
    std::cerr << " lowerThreshold upperThreshold ";
    std::cerr << " outsideValue insideValue   "  << std::endl;
    return EXIT_FAILURE;
    }

  typedef  double InputPixelType;
  typedef  double OutputPixelType;
  typedef otb::Image<InputPixelType,  2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;
  typedef itk::BinaryThresholdImageFilter<
      InputImageType, OutputImageType>  FilterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);

  filter->SetInput(reader->GetOutput());

  const OutputPixelType outsideValue = atoi(argv[5]);
  const OutputPixelType insideValue  = atoi(argv[6]);
  
  filter->SetOutsideValue(outsideValue);
  filter->SetInsideValue(insideValue);

  const OutputPixelType lowerThreshold = atof(argv[3]);
  const OutputPixelType upperThreshold = atof(argv[4]);
  
  filter->SetLowerThreshold(lowerThreshold);
  filter->SetUpperThreshold(upperThreshold);

  filter->Update();

  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}

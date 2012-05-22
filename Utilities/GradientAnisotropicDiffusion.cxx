/*=========================================================================

  Program:   GNORASI Gradient Anisotropic Diffusion Application
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
#include "itkRescaleIntensityImageFilter.h"

#include "itkGradientAnisotropicDiffusionImageFilter.h"

int main(int argc, char * argv[])
{
  if (argc < 6)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile ";
    std::cerr << "numberOfIterations  timeStep  conductance" << std::endl;
    return EXIT_FAILURE;
  }

  typedef    float InputPixelType;
  typedef    float OutputPixelType;

  typedef otb::Image<InputPixelType,  2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  typedef itk::GradientAnisotropicDiffusionImageFilter<
      InputImageType, OutputImageType>  FilterType;
  FilterType::Pointer filter = FilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  filter->SetInput(reader->GetOutput());

  const unsigned int numberOfIterations = atoi(argv[3]);

  const double timeStep = atof(argv[4]);

  const double conductance = atof(argv[5]);

  filter->SetNumberOfIterations(numberOfIterations);
  filter->SetTimeStep(timeStep);
  filter->SetConductanceParameter(conductance);

  filter->Update();

  typedef unsigned char                 WritePixelType;
  typedef otb::Image<WritePixelType, 2> WriteImageType;
  typedef itk::RescaleIntensityImageFilter<
      OutputImageType, WriteImageType> RescaleFilterType;

  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  typedef otb::ImageFileWriter<WriteImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);

  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

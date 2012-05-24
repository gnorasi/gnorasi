/*=========================================================================

  Program:   GNORASI Watershed Segmentation Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  INPUTS: {ROI_QB_MUL_3.tif}
//  OUTPUTS: {WatershedSegmentation1Output1.png}
//  2 10 0 0.05 1
//  INPUTS: {ROI_QB_MUL_3.tif}
//  OUTPUTS: {WatershedSegmentation1Output2.png}
//  2 10 0.001 0.15 0
#include <iostream>

#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVectorCastImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

int main(int argc, char *argv[])
{
  if (argc < 8)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputImage outputImage conductanceTerm diffusionIterations lowerThreshold outputScaleLevel gradientMode "
              << std::endl;
    return 1;
  }

  typedef itk::RGBPixel<unsigned char>   RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>    RGBImageType;
  typedef itk::Vector<float, 3>          VectorPixelType;
  typedef itk::Image<VectorPixelType, 2> VectorImageType;
  typedef itk::Image<unsigned long, 2>   LabeledImageType;
  typedef itk::Image<float, 2>           ScalarImageType;


  typedef otb::ImageFileReader<RGBImageType> FileReaderType;
  typedef itk::VectorCastImageFilter<RGBImageType, VectorImageType>
  CastFilterType;
  typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType,
      VectorImageType>
  DiffusionFilterType;
  typedef itk::VectorGradientMagnitudeImageFilter<VectorImageType, float,
      ScalarImageType>
  GradientMagnitudeFilterType;
  typedef itk::WatershedImageFilter<ScalarImageType> WatershedFilterType;

  typedef otb::ImageFileWriter<RGBImageType> FileWriterType;

  FileReaderType::Pointer reader = FileReaderType::New();
  reader->SetFileName(argv[1]);

  CastFilterType::Pointer caster = CastFilterType::New();


  DiffusionFilterType::Pointer diffusion = DiffusionFilterType::New();
  diffusion->SetNumberOfIterations(atoi(argv[4]));
  diffusion->SetConductanceParameter(atof(argv[3]));
  diffusion->SetTimeStep(0.125);

  GradientMagnitudeFilterType::Pointer
    gradient = GradientMagnitudeFilterType::New();
  gradient->SetUsePrincipleComponents(atoi(argv[7]));


  WatershedFilterType::Pointer watershed = WatershedFilterType::New();
  watershed->SetLevel(atof(argv[6]));
  watershed->SetThreshold(atof(argv[5]));

  typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>
  ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<LabeledImageType,
      RGBImageType,
      ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();

  FileWriterType::Pointer writer = FileWriterType::New();
  writer->SetFileName(argv[2]);

  caster->SetInput(reader->GetOutput());
  diffusion->SetInput(caster->GetOutput());
  gradient->SetInput(diffusion->GetOutput());
  watershed->SetInput(gradient->GetOutput());
  colormapper->SetInput(watershed->GetOutput());
  writer->SetInput(colormapper->GetOutput());

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
  }

  return EXIT_SUCCESS;
}

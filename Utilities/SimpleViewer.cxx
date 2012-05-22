/*=========================================================================

  Program:   GNORASI Simple Viewer Application
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
//#include "itkGradientMagnitudeImageFilter.h"
#include "otbStandardImageViewer.h"

int main(int argc, char * argv[])
{

  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0] << " <inputImageFile>" << std::endl;
    return EXIT_FAILURE;
    }

  typedef double                   PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  //typedef itk::GradientMagnitudeImageFilter
  //<ImageType, ImageType> FilterType;
  //FilterType::Pointer filter = FilterType::New();

  typedef otb::StandardImageViewer<ImageType> ViewerType;
  ViewerType::Pointer viewer = ViewerType::New();

  //filter->SetInput(reader->GetOutput());
  //viewer->SetImage(filter->GetOutput());
  viewer->SetImage(reader->GetOutput());
  viewer->SetLabel(argv[1]);

  viewer->Update();

  Fl::run();

  return EXIT_SUCCESS;
}


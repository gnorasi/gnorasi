/*=========================================================================

  Program:   GNORASI Filtering with Convolution Application
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

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"

int main(int argc, char *argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
              << " inputImageFile outputImageFile"
              << std::endl;
    return -1;
    }

  typedef float                           PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef itk::ConstNeighborhoodIterator<ImageType> NeighborhoodIteratorType;
  typedef itk::ImageRegionIterator<ImageType>       IteratorType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
    }

  NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType it(radius, reader->GetOutput(),
                              reader->GetOutput()->GetRequestedRegion());

  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetRequestedRegion());
  output->Allocate();

  IteratorType out(output, reader->GetOutput()->GetRequestedRegion());

  NeighborhoodIteratorType::OffsetType offset1 = {{-1, -1}};
  NeighborhoodIteratorType::OffsetType offset2 = {{-1, 0 }};
  NeighborhoodIteratorType::OffsetType offset3 = {{-1, 1 }};
  NeighborhoodIteratorType::OffsetType offset4 = {{ 0, -1}};
  NeighborhoodIteratorType::OffsetType offset5 = {{ 0, 0 }};
  NeighborhoodIteratorType::OffsetType offset6 = {{ 0, 1 }};
  NeighborhoodIteratorType::OffsetType offset7 = {{ 1, -1}};
  NeighborhoodIteratorType::OffsetType offset8 = {{ 1, 0 }};
  NeighborhoodIteratorType::OffsetType offset9 = {{ 1, 1 }};

  for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
    {
    float sum;
    sum = it.GetPixel(offset2) - it.GetPixel(offset1);
    sum += 2.0 * it.GetPixel(offset4) - 2.0 * it.GetPixel(offset3);
    sum += it.GetPixel(offset6) - it.GetPixel(offset5);
    out.Set(sum);
    }

  typedef unsigned char                        WritePixelType;
  typedef otb::Image<WritePixelType, 2>        WriteImageType;
  typedef otb::ImageFileWriter<WriteImageType> WriterType;

  typedef itk::RescaleIntensityImageFilter<
      ImageType, WriteImageType> RescaleFilterType;

  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(output);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(rescaler->GetOutput());
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
    }

  return EXIT_SUCCESS;
}

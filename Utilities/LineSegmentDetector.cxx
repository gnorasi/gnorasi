/*=========================================================================

  Program:   GNORASI Local Corelation Change Detection Application
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
#include "otbVectorData.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataToLabelImageFilter.h"
#include "otbLineSegmentDetector.h"

int main(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImage outputImage" << std::endl;
    return EXIT_FAILURE;
  }
  
  const char * infname  = argv[1];
  const char * outfname  = argv[2];
  
  typedef unsigned char InputPixelType;
  //typedef unsigned int OutputPixelType;
  typedef double        PrecisionType;
  const unsigned int Dimension = 2;
  
  typedef otb::Image<InputPixelType,  Dimension> ImageType;
  //typedef otb::Image<OutputPixelType,  Dimension> OutImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  
  ReaderType::Pointer reader = ReaderType::New();
  
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  typedef otb::LineSegmentDetector<ImageType,
      PrecisionType> LsdFilterType;

  LsdFilterType::Pointer lsdFilter = LsdFilterType::New();

  typedef otb::VectorData<PrecisionType> VectorDataType;
  
  typedef otb::VectorDataToLabelImageFilter<VectorDataType,
      ImageType> VectorDataRendererType;
  VectorDataRendererType::Pointer vectorDataRenderer = VectorDataRendererType::New();

  
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  
  lsdFilter->SetInput(reader->GetOutput());
  reader->GenerateOutputInformation();
  vectorDataRenderer->AddVectorData(lsdFilter->GetOutput());
  vectorDataRenderer->SetOutputParametersFromImage(reader->GetOutput());
  writer->SetInput(vectorDataRenderer->GetOutput());
  reader->GenerateOutputInformation();
  writer->Update();

  return EXIT_SUCCESS;
}

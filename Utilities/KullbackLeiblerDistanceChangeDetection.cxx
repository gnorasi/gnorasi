/*=========================================================================

  Program:   GNORASI Kullback Leibler Distance Change Detection Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbKullbackLeiblerDistanceImageFilter.h"

int main(int argc, char * argv[])
{
  try
  {
    if (argc != 5)
    {
      std::cerr <<
      "Change detection through a Kullback-Leibler measure (which is a distance between local distributions)\n";
      std::cerr <<
      "Kullback-Leibler measure is optimized by a Edgeworth series expansion\n";
      std::cerr << argv[0] << " imgAv imgAp imgResu winSize\n";
      return 1;
    }

    char * fileName1 = argv[1];
    char * fileName2 = argv[2];
    char * fileNameOut = argv[3];
    int    winSize = atoi(argv[4]);

    const unsigned int Dimension = 2;
    typedef double        PixelType;
    typedef unsigned char OutputPixelType;

    typedef otb::Image<PixelType, Dimension>       ImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;


    typedef otb::KullbackLeiblerDistanceImageFilter<ImageType,
        ImageType,
        ImageType> FilterType;

    typedef otb::ImageFileReader<ImageType>       ReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(fileName1);

    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(fileName2);


    FilterType::Pointer filter = FilterType::New();
    filter->SetRadius((winSize - 1) / 2);

    filter->SetInput1(reader1->GetOutput());
    filter->SetInput2(reader2->GetOutput());

    typedef itk::RescaleIntensityImageFilter<ImageType,
        OutputImageType> RescaleFilterType;
    RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

    rescaler->SetInput(filter->GetOutput());
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(fileNameOut);
    writer->SetInput(rescaler->GetOutput());
    writer->Update();

  }

  catch (itk::ExceptionObject& err)
    {
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }

  catch (...)
    {
    std::cout << "Unknown exception thrown !" << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}

/*=========================================================================

  Program:   GNORASI Mean Shift Segmentation Application
  Language:  C++

  Copyright (c) National Technical University of Athens. All rights reserved.
  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  
  Portions of this code are covered under the OTB copyright.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//    INPUTS: {ROI_QB_MUL_1.png}
//    OUTPUTS: {MSFilteredOutput.tif}, {MSClusteredOutput.tif}, {MSLabeledOutput.tif}, {MSBoundariesOutput.tif}, {MSFilteredOutput-pretty.jpg}, {MSClusteredOutput-pretty.jpg}
//  7 30 20 1.0

#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"

#include "otbMeanShiftVectorImageFilter.h"
int main(int argc, char * argv[])
{
  if (argc != 12)
  {
    std::cerr << "Usage: " << argv[0]
              << " infname filteredfname clusteredfname labeledclusteredfname "
              << "clusterboundariesfname filteredPretty clusteredPretty "
              << "spatialRadius rangeRadius minregionsize scale"
              << std::endl;
    return EXIT_FAILURE;
  }

  const char *       infname                = argv[1];
  const char *       filteredfname          = argv[2];
  const char *       clusteredfname         = argv[3];
  const char *       labeledclusteredfname  = argv[4];
  const char *       clusterboundariesfname = argv[5];
  const char *       filteredPretty         = argv[6];
  const char *       clusteredPretty        = argv[7];
  const unsigned int spatialRadius          = atoi(argv[8]);
  const double       rangeRadius            = atof(argv[9]);
  const unsigned int minRegionSize          = atoi(argv[10]);
  const double       scale                  = atoi(argv[11]);

  const unsigned int Dimension = 2;
  typedef float                                    PixelType;
  typedef otb::VectorImage<PixelType, Dimension>   ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;


  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::LabeledOutputType
  LabeledImageType;
  typedef otb::StreamingImageFileWriter<LabeledImageType>
  LabeledWriterType;

  FilterType::Pointer        filter = FilterType::New();
  ReaderType::Pointer        reader = ReaderType::New();
  WriterType::Pointer        writer1 = WriterType::New();
  WriterType::Pointer        writer2 = WriterType::New();
  LabeledWriterType::Pointer writer3 = LabeledWriterType::New();
  LabeledWriterType::Pointer writer4 = LabeledWriterType::New();


  reader->SetFileName(infname);
  writer1->SetFileName(filteredfname);
  writer2->SetFileName(clusteredfname);
  writer3->SetFileName(labeledclusteredfname);
  writer4->SetFileName(clusterboundariesfname);


  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);

  filter->SetScale(scale);

  filter->SetInput(reader->GetOutput());
  writer1->SetInput(filter->GetOutput());
  writer2->SetInput(filter->GetClusteredOutput());
  writer3->SetInput(filter->GetLabeledClusteredOutput());
  writer4->SetInput(filter->GetClusterBoundariesOutput());

  writer1->Update();
  writer2->Update();
  writer3->Update();
  writer4->Update();

  typedef otb::PrintableImageFilter<ImageType> PrintableFilterType;
  PrintableFilterType::Pointer printableImageFilter = PrintableFilterType::New();

  printableImageFilter->SetChannel(1);
  printableImageFilter->SetChannel(2);
  printableImageFilter->SetChannel(3);

  typedef PrintableFilterType::OutputImageType           OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>          PrettyWriterType;

  PrettyWriterType::Pointer prettyWriter = PrettyWriterType::New();

  printableImageFilter->SetInput(filter->GetOutput());
  prettyWriter->SetFileName(filteredPretty);
  prettyWriter->SetInput(printableImageFilter->GetOutput());
  prettyWriter->Update();

  printableImageFilter->SetInput(filter->GetClusteredOutput());
  prettyWriter->SetFileName(clusteredPretty);
  prettyWriter->SetInput(printableImageFilter->GetOutput());
  prettyWriter->Update();

  return EXIT_SUCCESS;
}

/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegionConstIteratorWithIndex.h"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "itiotbVectorImageModel.h"

#include "../vector_globaldefs.h"


#include "itkImageRegionConstIteratorWithIndex.h"


using namespace otb;


using namespace itiviewer;
/*
  TRANSLATOR mvd::VectorImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
VectorImageModel
::VectorImageModel( ItiOtbImageManager *manager, QObject* parent ) :
  AbstractImageModel( manager, parent ),
  m_RasterizedBuffer( NULL ),
  m_PreviousRegion(ImageRegionType())
{
    m_ExtractFilter = ExtractFilterType::New();
    m_RenderingFilter = RenderingFilterType::New();
    m_RenderingFilter->GetRenderingFunction()->SetAutoMinMax(false);
}

/*******************************************************************************/
VectorImageModel
::~VectorImageModel()
{
  this->ClearBuffer();
}

/*******************************************************************************/
void
VectorImageModel
::loadFile( const QString& filename )
{
  DefaultImageFileReaderType::Pointer imageFileReader(
    DefaultImageFileReaderType::New()
  );

  m_lastPath = filename;

  imageFileReader->SetFileName( filename.toLatin1().data() );
  imageFileReader->UpdateOutputInformation();

  m_ImageFileReader = imageFileReader;

  // Before doing anything, check if region is inside the buffered
  // region of image
  unsigned int currentIndex = 0;
  const DefaultImageType* image =  this->GetOutput(currentIndex);
  if(!image)
      return;

  // initialize the channel list for the rendering needs following the
  // input image
  // TODO : See if if needs to be moved somewhere else
  // TODO : use the default display
//  if (m_ImageFileReader->GetOutput()->GetNumberOfComponentsPerPixel()  < 3)
  if(image->GetNumberOfComponentsPerPixel() < 3)
    {
    m_Channels.resize(1);
    m_Channels[0]  = 0;
    }
  else
    {
    m_Channels.resize(3);
    m_Channels[0]  = 0;
    m_Channels[1]  = 1;
    m_Channels[2]  = 2;
    }

  m_RenderingFilter->GetRenderingFunction()->SetChannelList(m_Channels);

  emit changed();
}

void VectorImageModel::resetData(){

    // Clear the previous buffer
    this->ClearBuffer();

    // Allocate new memory
    unsigned int nbPixels = 3 * m_Region.GetNumberOfPixels();
    m_RasterizedBuffer = new unsigned char[nbPixels];

    DumpImagePixelsWithinRegionIntoBuffer(m_Region);
}

void
VectorImageModel
::ClearBuffer()
{
  // Delete previous buffer if needed
  if (m_RasterizedBuffer != NULL)
    {
    delete[] m_RasterizedBuffer;
    m_RasterizedBuffer = NULL;
    }
}

unsigned char *
VectorImageModel
::RasterizeRegion( const ImageRegionType& region)
{
  m_Region = region;
  // Don't do anything if the region did not changed
  // moveMoveEvent (Drag region).
  if ( m_PreviousRegion != region )
    {
    // check the current and the previous region have some pixels in
    // common 
    ImageRegionType  tempPreviousRegionRegion = m_PreviousRegion;
    bool res =  tempPreviousRegionRegion.Crop(region);

    // if the first time, image must be read
    if ( m_PreviousRegion != ImageRegionType() && res)
      {
      // Compute loaeded region, and region not loaded yet within the
      // new requested region
      this->ComputeRegionsToLoad(m_Region);

      // Copy the previous buffer into a temporary buf to get the
      // previously loaded data
      unsigned char * previousRasterizedBuffer =  
        new unsigned char[3 * m_PreviousRegion.GetNumberOfPixels()];

      std::memcpy(previousRasterizedBuffer, m_RasterizedBuffer, 3 * m_PreviousRegion.GetNumberOfPixels());

      // Clear the previous buffer
      this->ClearBuffer();

      // Allocate new memory
      unsigned int nbPixels = 3 * region.GetNumberOfPixels();
      m_RasterizedBuffer = new unsigned char[nbPixels];
    
      // Copy the already loaded pixels into the m_RasterizedBuffer
      unsigned int previousNbPixels = m_PreviousRegion.GetNumberOfPixels();
      
      for (unsigned int idx = 0; idx < previousNbPixels; idx++)
        {
        // compose the image index from the buffer index
        IndexType imageIndex;
        imageIndex = ComputeImageIndexFromFlippedBuffer(idx, m_PreviousRegion);

        if (m_AlreadyLoadedRegion.IsInside(imageIndex))
          {
          // Get the buffer index relative to the imageIndex in the
          // new requested region 
          unsigned int newBufferindex = 0;
          newBufferindex = ComputeXAxisFlippedBufferIndex(imageIndex, m_Region);
          
          // Copy the already loaded values into the new buffer
          m_RasterizedBuffer[newBufferindex]     = previousRasterizedBuffer[3*idx];
          m_RasterizedBuffer[newBufferindex + 1] = previousRasterizedBuffer[3*idx + 1];
          m_RasterizedBuffer[newBufferindex + 2] = previousRasterizedBuffer[3*idx + 2];
          }
        }
      
      // Get the image pixels within not loaded region and add them to
      // the buffer
      for (unsigned int idx = 0; idx < m_RegionsToLoadVector.size() ; idx++)
        {
        this->DumpImagePixelsWithinRegionIntoBuffer(m_RegionsToLoadVector[idx]);
        }

      // free the previous buffer memory (copied one)
      if (previousRasterizedBuffer != NULL)
        {
        delete[] previousRasterizedBuffer;
        previousRasterizedBuffer = NULL;
        }
      }
    else
      {
      // Delete previous buffer if needed
      this->ClearBuffer();
 
      // Allocate new memory
      m_RasterizedBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

      // rasterize the region
      this->DumpImagePixelsWithinRegionIntoBuffer(region);
      }
    }

  // Store the region
  m_PreviousRegion = region;
  
  // if ok return the  buffer
  return m_RasterizedBuffer;
}


void
VectorImageModel
::DumpImagePixelsWithinRegionIntoBuffer(const ImageRegionType& region)
{

//    QTime time;
//    qDebug() << "Start of DumpImagePixelsWithinRegionIntoBuffer...";
//    time.restart();

  // Before doing anything, check if region is inside the buffered
  // region of image
  unsigned int currentIndex = 0;

  // TODO : add some checking
  const DefaultImageType* image =  this->GetOutput(currentIndex);
  if(!image)
      return;

  // some checking
  if (!image->GetBufferedRegion().IsInside(region))
    {
    //itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
    }


  //test to strech image in 8bit
  ByteRescalerFilterType::Pointer  byterescaler;
  byterescaler = ByteRescalerFilterType::New();
  //image->UpdateOutputInformation();
  ByteImageType::PixelType minimum, maximum;
  int bands = image->GetNumberOfComponentsPerPixel();
  minimum.SetSize(bands);
  maximum.SetSize(bands);
  minimum.Fill(0);
  maximum.Fill(255);
  byterescaler->SetInput(image);
  byterescaler->SetOutputMinimum(minimum);
  byterescaler->SetOutputMaximum(maximum);
  byterescaler->SetClampThreshold(0.01);
  ByteImageType::Pointer image8;
  image8 = byterescaler->GetOutput();
//  byterescaler->Update();
  //end test

  // Extract the region of interest in the image
  m_ExtractFilter->SetInput(image8);
  m_ExtractFilter->SetExtractionRegion(region);

  // Use the rendering filter to get
  m_RenderingFilter->SetInput(m_ExtractFilter->GetOutput());
  m_RenderingFilter->GetOutput()->SetRequestedRegion(region);
  m_RenderingFilter->Update();

//  qDebug() << "End of m_RenderingFilter \nmilliseconds elapsed : " << time.elapsed();


  // Declare the iterator
  itk::ImageRegionConstIteratorWithIndex< RenderingFilterType::OutputImageType >
    it(m_RenderingFilter->GetOutput(), region);

  // Go to begin
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    // Fill the buffer
    unsigned int index = 0;
    index = ComputeXAxisFlippedBufferIndex(it.GetIndex(), m_Region);

    // Fill the buffer
    m_RasterizedBuffer[index]  = it.Get()[0];
    m_RasterizedBuffer[index + 1] = it.Get()[1];
    m_RasterizedBuffer[index + 2] = it.Get()[2];
    ++it;
    }


//    qDebug() << "End of DumpImagePixelsWithinRegionIntoBuffer \nmilliseconds elapsed : " << time.elapsed();
}


void
VectorImageModel
::ComputeRegionsToLoad(const ImageRegionType& region)
{
  // Initialize the region and clear vector
  m_AlreadyLoadedRegion = m_PreviousRegion;
  m_RegionsToLoadVector.clear();

  // 4 regions to compute
  ImageRegionType upperRegion;
  ImageRegionType lowerRegion;
  ImageRegionType leftRegion;
  ImageRegionType rightRegion;

  // local variables
  IndexType index;
  ImageRegionType::SizeType size;

  // Compute the already loaded region as a simple Crop
  bool res =  m_AlreadyLoadedRegion.Crop(region);

  // ------ upper region
  upperRegion.SetIndex(region.GetIndex());
  size[0] = region.GetSize()[0];
  size[1] = vcl_abs( region.GetIndex()[1] - m_AlreadyLoadedRegion.GetIndex()[1]);
  upperRegion.SetSize(size);

  // ------ lower region
  index[0] = region.GetIndex()[0];
  index[1] = m_AlreadyLoadedRegion.GetSize()[1] + m_AlreadyLoadedRegion.GetIndex()[1];
  lowerRegion.SetIndex(index);

  size[0] = region.GetSize()[0];
  size[1] = vcl_abs( (int)region.GetIndex()[1] + (int)region.GetSize()[1]
                     - (int)m_AlreadyLoadedRegion.GetIndex()[1] - (int)m_AlreadyLoadedRegion.GetSize()[1] );
  lowerRegion.SetSize(size);

  // ------- left region
  index[0] = region.GetIndex()[0];
  index[1] = m_AlreadyLoadedRegion.GetIndex()[1];
  leftRegion.SetIndex(index);

  size[0] = vcl_abs(region.GetIndex(0) - m_AlreadyLoadedRegion.GetIndex()[0]);
  size[1] = m_AlreadyLoadedRegion.GetSize()[1];
  leftRegion.SetSize(size);

  // -------- right region
  index[0] = m_AlreadyLoadedRegion.GetIndex()[0] + m_AlreadyLoadedRegion.GetSize()[0];
  index[1] = m_AlreadyLoadedRegion.GetIndex()[1];
  rightRegion.SetIndex(index);

  size[0] = vcl_abs((int)region.GetSize()[0] + (int)region.GetIndex()[0]
                    - (int)m_AlreadyLoadedRegion.GetIndex()[0] - (int)m_AlreadyLoadedRegion.GetSize()[0]);
  size[1] = m_AlreadyLoadedRegion.GetSize()[1]; 
  rightRegion.SetSize(size);

  // add the upper region if any pixel
  if ( upperRegion .GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(upperRegion);

  // add the lower region if any pixel
  if ( lowerRegion.GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(lowerRegion);

  // add the left region if any pixel
  if ( leftRegion.GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(leftRegion);
  
  // add the right region if any pixel
  if ( rightRegion.GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(rightRegion);
}


ImageRegionType VectorImageModel::GetLargestPossibleRegion() const{
    ImageRegionType region;
    if(!lastPath().isEmpty())
        region = m_ImageFileReader->GetOutput()->GetLargestPossibleRegion();

    return region;
}


void VectorImageModel::setRenderingFuction(RenderingFunctionType *rType){

    m_RenderingFilter->SetRenderingFunction(rType);
}


/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/


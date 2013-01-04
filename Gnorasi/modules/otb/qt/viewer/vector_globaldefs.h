#ifndef VECTOR_GLOBALDEFS_H
#define VECTOR_GLOBALDEFS_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itkRGBAPixel.h"
#include "itkFixedArray.h"
#include "otbObjectList.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbImageFileReader.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"

#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"


#include "rgba_globaldefs.h"

using namespace otb;

const unsigned int VDimension = 2;

typedef double                                                              VPixelType;
typedef otb::VectorImage<VPixelType, VDimension>                            VectorImageType;
typedef VectorImageType::RegionType                                         VectorRegionType;
typedef VectorImageType::IndexType                                          VectorIndexType;
typedef VectorImageType::SizeType                                           VectorSizeType;

typedef itk::ConstNeighborhoodIterator<RasterImageType>                     NeighborhoodIteratorType;
typedef itk::ImageRegionIterator<RasterImageType>                           IteratorType;

typedef itk::NeighborhoodAlgorithm
::ImageBoundaryFacesCalculator<RasterImageType>                             FaceCalculatorType;

typedef otb::ImageToVectorImageCastFilter<RasterImageType, VectorImageType> ImageToVectorImageCastFilterType;
typedef otb::ImageFileReader<VectorImageType> ReaderType;

typedef otb::ImageList<RasterImageType>                                     ImageListType;
typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType>   VectorImageToImageListType;

#endif // VECTOR_GLOBALDEFS_H

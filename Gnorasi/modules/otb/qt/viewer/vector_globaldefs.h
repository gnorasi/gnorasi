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
#include "otbImageToVectorImageCastFilter.h"
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

typedef otb::ImageToVectorImageCastFilter<RasterImageType, VectorImageType> ImageToVectorImageCastFilterType;

#endif // VECTOR_GLOBALDEFS_H

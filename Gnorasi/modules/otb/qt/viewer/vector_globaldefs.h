#ifndef VECTOR_GLOBALDEFS_H
#define VECTOR_GLOBALDEFS_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itkRGBAPixel.h"
#include "itkFixedArray.h"
#include "otbObjectList.h"

#include "../../ports/otbvectorimageport.h"

using namespace otb;

const unsigned int VDimension = 2;

typedef itk::RGBAPixel<unsigned char>               PixelType;
typedef otb::VectorImage<PixelType, VDimension>     VectorImageType;
typedef VectorImageType::RegionType                 VectorRegionType;
typedef VectorImageType::IndexType                  VectorIndexType;
typedef VectorImageType::SizeType                   VectorSizeType;

#endif // VECTOR_GLOBALDEFS_H

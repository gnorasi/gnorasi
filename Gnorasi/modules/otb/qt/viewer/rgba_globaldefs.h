#ifndef RGBA_GLOBALDEFS_H
#define RGBA_GLOBALDEFS_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itkRGBAPixel.h"
#include "itkFixedArray.h"
#include "otbObjectList.h"

#include "../../ports/otbimageport.h"

using namespace otb;

const unsigned int Dimension = 2;

typedef double                                  PixelType;
typedef otb::Image<PixelType, Dimension>        RasterImageType;
typedef otb::Image<unsigned char,Dimension>     RasterByteImageType;
typedef RasterImageType::RegionType             RasterRegionType;
typedef RasterImageType::IndexType              RasterIndexType;
typedef RasterImageType::SizeType               RasterSizeType;

#endif // RGBA_GLOBALDEFS_H

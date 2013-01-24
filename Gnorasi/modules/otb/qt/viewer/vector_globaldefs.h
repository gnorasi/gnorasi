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
#include "otbRenderingImageFilter.h"
#include "otbStandardRenderingFunction.h"
#include "otbNoStretchRenderingFunction.h"
#include "otbSquareRootRenderingFunction.h"
#include "otbGaussianRenderingFunction.h"
#include "otbVectorRescaleIntensityImageFilter.h"


#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"



#include "rgba_globaldefs.h"

using namespace otb;

const double ZOOM_VALUE = 0.125;

const unsigned int VDimension = 2;

typedef double                                                              VPixelType;
typedef otb::VectorImage<VPixelType, VDimension>                            VectorImageType;

/**
 */
typedef itk::RGBAPixel< unsigned char >                                     RGBAPixelType;
/**
 */
typedef otb::Image< RGBAPixelType, VDimension >                             RGBAImageType;

/**
 */
typedef itk::ImageRegion<VDimension >                                       ImageRegionType;

/**
 */
typedef itk::Index< VDimension >                                            IndexType;

typedef VectorImageType                                                     DefaultImageType;
typedef VectorImageType::RegionType                                         VectorRegionType;
typedef VectorImageType::IndexType                                          VectorIndexType;
typedef VectorImageType::SizeType                                           VectorSizeType;

typedef unsigned char                                                       BytePixelType;
typedef otb::VectorImage<BytePixelType, 2>                                  ByteImageType;

typedef itk::ConstNeighborhoodIterator<RasterImageType>                     NeighborhoodIteratorType;
typedef itk::ImageRegionIterator<RasterImageType>                           IteratorType;

typedef itk::NeighborhoodAlgorithm
::ImageBoundaryFacesCalculator<RasterImageType>                             FaceCalculatorType;

typedef otb::ImageToVectorImageCastFilter<RasterImageType,
VectorImageType>                                                            ImageToVectorImageCastFilterType;

typedef otb::VectorImage<RGBAImageType::PixelType,VDimension>               ByteVectorImageType;

typedef otb::ImageToVectorImageCastFilter<RGBAImageType,
ByteVectorImageType>                                                        ByteImageToVectorImageCastFilterType;

typedef otb::ImageFileReader<VectorImageType>                               ReaderType;


//typedef itk::Statistics::ListSample<VectorImageType::PixelType>             SampleListType;
typedef itk::Statistics::ListSample<ByteImageType::PixelType>               SampleListType;

typedef otb::ImageList<RasterImageType>                                     ImageListType;
typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType>   VectorImageToImageListType;

/**
 */
typedef otb::ImageFileReader< VectorImageType >                             VectorImageFileReaderType;

/** Rendering filter */
/** Rendering part */
//typedef RenderingImageFilter<VectorImageType, RGBAImageType>                RenderingFilterType;
typedef RenderingImageFilter<ByteImageType, RGBAImageType>                  RenderingFilterType;


/** Rendering function */
//typedef Function::RenderingFunction<VectorImageType::PixelType, RGBAPixelType> RenderingFunctionType;
typedef Function::RenderingFunction<ByteImageType::PixelType, RGBAPixelType> RenderingFunctionType;

//typedef Function::StandardRenderingFunction<VectorImageType::PixelType,
//                                            RGBAPixelType> StandardRenderingFunctionType;
//typedef StandardRenderingFunctionType::ChannelListType    ChannelListType;
//typedef StandardRenderingFunctionType::ParametersType     ParametersType;

//typedef Function::NoStretchRenderingFunction<VectorImageType::PixelType,
//                                             RGBAPixelType> NoStretchRenderingFunctionType;

//typedef Function::SquareRootRenderingFunction<VectorImageType::PixelType,
//                                              RGBAPixelType> SquareRootRenderingFunctionType;

//typedef Function::GaussianRenderingFunction<VectorImageType::PixelType,
//                                            RGBAPixelType> GaussianRenderingFunctionType;
typedef Function::StandardRenderingFunction<ByteImageType::PixelType,
                                            RGBAPixelType> StandardRenderingFunctionType;
typedef StandardRenderingFunctionType::ChannelListType    ChannelListType;
typedef StandardRenderingFunctionType::ParametersType     ParametersType;

typedef Function::NoStretchRenderingFunction<ByteImageType::PixelType,
                                             RGBAPixelType> NoStretchRenderingFunctionType;

typedef Function::SquareRootRenderingFunction<ByteImageType::PixelType,
                                              RGBAPixelType> SquareRootRenderingFunctionType;

typedef Function::GaussianRenderingFunction<ByteImageType::PixelType,
                                            RGBAPixelType> GaussianRenderingFunctionType;

/**
 */
typedef VectorImageFileReaderType                                           DefaultImageFileReaderType;


typedef otb::VectorRescaleIntensityImageFilter<
DefaultImageType, ByteImageType>                                            ByteRescalerFilterType;


/**
*/
typedef std::vector< std::string > StringVector;



#endif // VECTOR_GLOBALDEFS_H

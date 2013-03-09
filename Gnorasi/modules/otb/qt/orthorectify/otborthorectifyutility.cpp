/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#include "otborthorectifyutility.h"

#include "otbGeoInformationConversion.h"

#include "otbStreamingShrinkImageFilter.h"

#include "otbImageFileWriter.h"

const char *egsa = "PROJCS[\"GGRS87 / Greek Grid\",GEOGCS[\"GGRS87\",DATUM[\"Greek_Geodetic_Reference_System_1987\",SPHEROID[\"GRS 1980\",6378137,298.257222101,AUTHORITY[\"EPSG\",\"7019\"]],TOWGS84[-199.87,74.79,246.62,0,0,0,0],AUTHORITY[\"EPSG\",\"6121\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4121\"]],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",24],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],AUTHORITY[\"EPSG\",\"2100\"],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]";

OTBOrthoUtility::OTBOrthoUtility(QObject *parent) :
    m_utmZone(0),
    m_tmScale(1.0),
    m_useDEM(false),
    QObject(parent)
{
    m_Transform                     = TransformType::New();
    m_InverseTransform              = TransformType::New();
    m_Resampler                     = ResampleFilterType::New();

    m_outputImage                   = NULL;

    m_WholeOutputOrigin[0]          = 0.0;
    m_WholeOutputOrigin[1]          = 0.0;
    m_WholeOutputSize[0]            = 0;
    m_WholeOutputSize[1]            = 0;
    m_WholeOutputSpacing[0]         = 0.0;
    m_WholeOutputSpacing[1]         = 0.0;
}


void OTBOrthoUtility::updateTmScale(double sc){
    setTmScale(sc);

//    VectorImageType::Pointer vImgType = inputImage();
//    if(vImgType.IsNull())
//        return;

//    tranverseMercatorProjectionType::Pointer transMercatorProjection = tranverseMercatorProjectionType::New();
//    transMercatorProjection->SetParameters(m_easting, m_northing, m_tmScale);
//    std::string tmRef = transMercatorProjection->GetWkt();

//    // Report projection ref (not done by the resample filter)
//    itk::MetaDataDictionary& dict = vImgType->GetMetaDataDictionary();
//    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, tmRef);

//    // Up To date the transform
//    m_Transform->SetInputProjectionRef(tmRef);
//    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
//    m_Transform->InstanciateTransform();
}


void OTBOrthoUtility::setMapProjection(MAPPROJECTION mp){
    m_mapProjection                 = mp;
}

void OTBOrthoUtility::setHemisphere(int hm){
    m_hemisphere = hm;
}


void OTBOrthoUtility::setUtmZone(int z){
    m_utmZone = z;
}

void OTBOrthoUtility::setDEMPath(const QString &p){
    m_demPath = p;

    if (m_useDEM)
      {
      // Update the transform and the inverse one
          m_Transform->SetDEMDirectory(m_demPath.toStdString());
          m_Transform->InstanciateTransform();
          m_Transform->GetInverse(m_InverseTransform);
      }
    else
      {
          // Update the transform and the inverse one
          m_Transform->SetDEMDirectory("");
          m_Transform->InstanciateTransform();
          m_Transform->GetInverse(m_InverseTransform);
      }
}


void OTBOrthoUtility::setImageFile(const QString &s){
    m_imageFile = s;
}


void OTBOrthoUtility::setEpsg(const QString &e){
    m_epsg = e;
}


VectorImageType::Pointer OTBOrthoUtility::inputImage(){
    if(m_imageFile.isEmpty())
        return 0;

    VectorImageFileReaderType::Pointer reader = VectorImageFileReaderType::New();
    reader->SetFileName(m_imageFile.toLatin1().data());
    reader->GenerateOutputInformation();

    return reader->GetOutput();
}


void OTBOrthoUtility::updateOutputParameters(){
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    // Ccompute the output parameters stuff
    typedef otb::ImageToGenericRSOutputParameters<VectorImageType> OutputParamEstimatorType;
    OutputParamEstimatorType::Pointer estimator = OutputParamEstimatorType::New();

    estimator->SetInput(vImgType);
    estimator->SetOutputProjectionRef(m_OutputProjectionRef);
    estimator->Compute();

    // Keep the max absolute spacing value
    double maxAbsSpVal = std::max( std::abs(estimator->GetOutputSpacing()[0]),
                                   std::abs(estimator->GetOutputSpacing()[1]));

    // Edit the output image parmaters
    m_OutputOrigin = estimator->GetOutputOrigin();
    // Force the spacing to be samein X and Y
    m_OutputSpacing[0] = estimator->GetOutputSpacing()[0] > 0 ? maxAbsSpVal : -maxAbsSpVal;
    m_OutputSpacing[1] = estimator->GetOutputSpacing()[1] > 0 ? maxAbsSpVal : -maxAbsSpVal;
    m_OutputSize = estimator->GetOutputSize();

    // Keep a copy of the origin of the whole projected image
    m_WholeOutputOrigin = estimator->GetOutputOrigin();
    m_WholeOutputSpacing = estimator->GetOutputSpacing();
    // Force the spacing to be samein X and Y
    m_WholeOutputSpacing[0] = estimator->GetOutputSpacing()[0] > 0 ? maxAbsSpVal : -maxAbsSpVal;
    m_WholeOutputSpacing[1] = estimator->GetOutputSpacing()[1] > 0 ? maxAbsSpVal : -maxAbsSpVal;
    m_WholeOutputSize = estimator->GetOutputSize();

    emit transformChanged();
}


bool OTBOrthoUtility::validateEpsgCode(int rsidNumber){

    std::string projRef = otb::GeoInformationConversion::ToWKT(rsidNumber);
    if (projRef.empty()){
        QString output = QString::fromStdString(projRef);
        qDebug() << "ProjRef : " << output;

        emit nonValidEpsgCode();
        return false;
    }

    qDebug() << QString::fromStdString(projRef) ;

    return true;
}


void OTBOrthoUtility::updateEPSGTransform(int rsidNumber){
    m_epsg = QString::number(rsidNumber);

    if(!validateEpsgCode(rsidNumber))
        return;

    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    m_OutputProjectionRef = otb::GeoInformationConversion::ToWKT(rsidNumber);

//    m_OutputProjectionRef = std::string(egsa);

    // Build the Generic RS transform
    m_Transform->SetInputProjectionRef(vImgType->GetProjectionRef());
    m_Transform->SetInputDictionary(vImgType->GetMetaDataDictionary());

    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    // Get the inverse
    m_Transform->GetInverse(m_InverseTransform);

    updateOutputParameters();
}

/**
 *
 */
void OTBOrthoUtility::UpdateUTMTransform(int zone, bool hemisphere)
{
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    typedef UtmInverseProjection UtmProjectionType;
    UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();

    std::string hem = "N";
    if (!hemisphere) hem = "S";

    utmProjection->SetHemisphere(hem[0]);
    utmProjection->SetZone(zone);
    m_OutputProjectionRef = utmProjection->GetWkt();

    // Build the Generic RS transform
    m_Transform->SetInputProjectionRef(vImgType->GetProjectionRef());
    m_Transform->SetInputDictionary(vImgType->GetMetaDataDictionary());

    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    // Get the transform
    m_Transform->GetInverse(m_InverseTransform);

    // Update the output image parameters
    updateOutputParameters();

    //Notify the view that the transform has changed
    emit transformChanged();
}


/**
 * Initialize the Transmercator Projection and Transform
 */
void OTBOrthoUtility::UpdateTMTransform(double scale, double falseEasting, double falseNorthing)
{
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
      return;


    typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
    TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();

    transMercatorProjection->SetParameters(falseEasting, falseNorthing, scale);
    m_OutputProjectionRef = transMercatorProjection->GetWkt();

    // Build the Generic RS transform
    m_Transform->SetInputProjectionRef(vImgType->GetProjectionRef());
    m_Transform->SetInputDictionary(vImgType->GetMetaDataDictionary());

    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    // Instanciate the inverse RS transform
    m_Transform->GetInverse(m_InverseTransform);

    updateOutputParameters();

    emit transformChanged();
}



std::string OTBOrthoUtility::projectionRef(){
    std::string t;

    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return t;

    t = vImgType->GetProjectionRef();

    return t;
}

/**
 *
 */
void OTBOrthoUtility::projectRegion(unsigned int sizeX, unsigned int sizeY, double spacingX, double spacingY,
                                    double originX, double originY)
{
  // Edit the size
  m_OutputSize[0] = sizeX;
  m_OutputSize[1] = sizeY;

  // Edit the spacing
  m_OutputSpacing[0] = spacingX;
  m_OutputSpacing[1] = spacingY;

  // Edit the origin in the cartographic projection
  OutputPointType geoPoint;
  geoPoint[0] = originX;
  geoPoint[1] = originY;
  m_OutputOrigin = m_Transform->GetTransform()->GetSecondTransform()->TransformPoint(geoPoint);
}


/**
 *
 */
void OTBOrthoUtility::UpdateWGS84Transform()
{
    // Build the Output Projection Ref
    // WGS84
    char * wgs84Ref;
    OGRSpatialReference oSRS;
    oSRS.SetWellKnownGeogCS("WGS84");
    oSRS.exportToWkt(&wgs84Ref);
    m_OutputProjectionRef = wgs84Ref;

    VectorImageType::Pointer vImg  = inputImage();
    if(vImg.IsNull())
      return;

    // Build the Generic RS transform
    m_Transform->SetInputProjectionRef(vImg->GetProjectionRef());
    m_Transform->SetInputDictionary(vImg->GetMetaDataDictionary());

    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    // Get the transform
    m_Transform->GetInverse(m_InverseTransform);

    // Update the output image parameters
    updateOutputParameters();

    //Notify the view that the transform has changed
    emit transformChanged();
}



void OTBOrthoUtility::applyOrthoRectification(){

    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

//    // Fill the resampler parameters
//    m_Resampler->SetInput(vImgType);
//    m_Resampler->SetOutputSize(m_OutputSize);
//    m_Resampler->SetOutputSpacing(m_OutputSpacing);
//    m_Resampler->SetOutputOrigin(m_OutputOrigin);
////    m_Resampler->SetOutputProjectionRef(m_Transform->GetOutputProjectionRef());
//    m_Resampler->SetOutputProjectionRef(m_OutputProjectionRef);

//    // Estimate Input RPC Model
////    if (m_EstimateInputRPCModel)
////      {
////      m_Resampler->EstimateInputRpcModelOn();
////      m_Resampler->SetInputRpcGridSize(20);
////      }

//    // Use the DEM
//    if (m_useDEM)
//    {
//        m_Resampler->SetDEMDirectory(m_Transform->GetDEMDirectory());
//    }

//    // Default padding value
//    VectorImageType::PixelType defaultValue;
//    unsigned int nmcpp = vImgType->GetNumberOfComponentsPerPixel();
//    itk::PixelBuilder<VectorImageType::PixelType>::Zero(defaultValue, nmcpp);
//    m_Resampler->SetEdgePaddingValue(defaultValue);

//    m_Resampler->Update();

//    typedef otb::ImageFileWriter<VectorImageType> WriterType;
//    WriterType::Pointer writer = WriterType::New();
//    QString path = QFileDialog::getSaveFileName(0,tr("Save"),QDir::homePath());

//    writer->SetFileName(path.toStdString());
//    writer->SetInput(m_Resampler->GetOutput());
//    //        writer->SetAutomaticTiledStreaming();
//    writer->Update();

//    m_outputImage = m_Resampler->GetOutput();



    //----------------------------------------------------------------------------------
    //
    // TEST
    //
    //

    //    switch ((int)m_mapProjection){
    //    case (int)MP_UTM:
    //        break;
    //    case (int)MP_LAMBERT2:
    //        break;
    //    case (int)MP_WGS84:
    //        break;
    //    case (int)MP_EPSG:{
            typedef otb::GenericMapProjection<otb::TransformDirection::FORWARD> GMapProjection;
            GMapProjection::Pointer genericMapProjection = GMapProjection::New();
            genericMapProjection->SetWkt(egsa);
//            genericMapProjection->SetWkt(m_OutputProjectionRef);


//            tranverseMercatorProjectionType::Pointer tmMapProjection = tranverseMercatorProjectionType::New();
//            tmMapProjection->SetParameters(500000.0, 0.0, 0.9996);
//            tmMapProjection->SetParameter("OriginX", "24.0"); //TODO check where this is coming from
//            tmMapProjection->SetParameter("OriginY", "0.0");


            typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, GMapProjection> OrthoRectifFilterType;
//            typedef otb::OrthoRectificationFilter<VectorImageType, VectorImageType, tranverseMercatorProjectionType> OrthoRectifFilterType;
            OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
            orthoRectifFilter->SetMapProjection(genericMapProjection);
//            orthoRectifFilter->SetMapProjection(tmMapProjection);

            orthoRectifFilter->SetInput(vImgType);
            VectorImageType::IndexType start;
            start[0] = 0;
            start[1] = 0;
            orthoRectifFilter->SetOutputStartIndex(start);
            orthoRectifFilter->SetOutputSize(m_OutputSize);
            orthoRectifFilter->SetOutputSpacing(m_OutputSpacing);
            VectorImageType::SpacingType gridSpacing;
            gridSpacing[0] = 20.0;
            gridSpacing[1] = 20.0;
            orthoRectifFilter->SetDeformationFieldSpacing(gridSpacing);
            VectorImageType::PixelType defaultValue;
            unsigned int nmcpp = vImgType->GetNumberOfComponentsPerPixel();
            itk::PixelBuilder<VectorImageType::PixelType>::Zero(defaultValue, nmcpp);
            orthoRectifFilter->SetEdgePaddingValue(defaultValue);
            orthoRectifFilter->SetOutputOrigin(m_OutputOrigin);
            orthoRectifFilter->Update();

            typedef otb::ImageFileWriter<VectorImageType> WriterType;
            WriterType::Pointer writer = WriterType::New();
            QString path = QFileDialog::getSaveFileName(0,tr("Save"),QDir::homePath());

            writer->SetFileName(path.toStdString());
            writer->SetInput(orthoRectifFilter->GetOutput());
    //        writer->SetAutomaticTiledStreaming();
            writer->Update();



    //        m_outputImage = orthoRectifFilter->GetOutput();

    //        break;
    //    }
    //    case (int)MP_TM:
    //        break;
    //    default:
    //        break;
    //    }

    //
    //
    // TEST
    //
    //----------------------------------------------------------------------------------


    emit transformChanged();
}

/**
 * Update the input Projection Ref
 */
void OTBOrthoUtility::UpdateInputUTMTransform(int zone, bool north)
{
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    typedef UtmInverseProjection UtmProjectionType;
    UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();

    std::string hem = "N";
    if (!north) hem = "S";

    utmProjection->SetHemisphere(hem[0]);
    utmProjection->SetZone(zone);
    std::string utmRef = utmProjection->GetWkt();

    // Report projection ref (not done by the resample filter)
    itk::MetaDataDictionary& dict = vImgType->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, utmRef);

    // Up To date the transform
    m_Transform->SetInputProjectionRef(utmRef);
    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    emit transformChanged();
}


void OTBOrthoUtility::UpdateInputEPSGTransform(int rsidNumber)
{
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    std::string epsgRef = otb::GeoInformationConversion::ToWKT(rsidNumber);

//    std::string epsgRef = std::string(egsa);

    // Report projection ref (not done by the resample filter)
    itk::MetaDataDictionary& dict = vImgType->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, epsgRef);

    // Up To date the transform
    m_Transform->SetInputProjectionRef(epsgRef);
    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    updateOutputParameters();

    emit transformChanged();

}


/**
 * Update the input Projection LambertII Projection Ref
 */
void OTBOrthoUtility::InitializeInputLambertIITransform()
{
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    typedef Lambert2EtenduForwardProjection Lambert2Type;
    Lambert2Type::Pointer lambert2Projection = Lambert2Type::New();
    std::string lambertRef = lambert2Projection->GetWkt();

    // Report projection ref (not done by the resample filter)
    itk::MetaDataDictionary& dict = vImgType->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, lambertRef);

    // Update the transform
    m_Transform->SetInputProjectionRef(lambertRef);
    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    emit transformChanged();
}

/**
 * Initialize the Transmercator Projection and Transform
 */
void OTBOrthoUtility::UpdateInputTMTransform(double scale, double falseEasting, double falseNorthing)
{
    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
    TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();
    transMercatorProjection->SetParameters(falseEasting, falseNorthing, scale);
    std::string tmRef = transMercatorProjection->GetWkt();

    // Report projection ref (not done by the resample filter)
    itk::MetaDataDictionary& dict = vImgType->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, tmRef);

    // Up To date the transform
    m_Transform->SetInputProjectionRef(tmRef);
    m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
    m_Transform->InstanciateTransform();

    emit transformChanged();
}

void OTBOrthoUtility::setInterpolator(INTERPOLATOR ir){
    int currentIndex = (int)ir;

    switch (currentIndex){
    case (int)INTERPOLATOR_LINEAR:
    {
        typedef itk::LinearInterpolateImageFunction<VectorImageType, double> LinearType;
        LinearType::Pointer interp = LinearType::New();
        m_Resampler->SetInterpolator(interp);
        break;
    }
    case (int)INTERPOLATOR_NEAREST:
    {
        typedef itk::NearestNeighborInterpolateImageFunction<VectorImageType, double> NearestType;
        NearestType::Pointer interp = NearestType::New();
        m_Resampler->SetInterpolator(interp);
        break;
    }
    case (int)INTERPOLATOR_BCO:{
        //        typedef BCOInterpolateImageFunction<ImageType, double> BCOType;
        //        BCOType::Pointer interp = BCOType::New();
        //        interp->SetRadius(static_cast<unsigned int> (guiBCORadius->value()));
        //        m_Controller->GetModel()->GetResampler()->SetInterpolator(interp);
        break;
    }
    default:
        break;
    }
}


void OTBOrthoUtility::updateRPCModel(const QString &path){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        return;

    VectorImageType::Pointer vImgType = inputImage();
    if(vImgType.IsNull())
        return;

    GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
    rpcEstimator->SetInput(vImgType);

    QTextStream in(&file);

    QString text  = in.readAll();

    QStringList lines = text.split("\n");
    foreach(QString line , lines){
        QStringList tags = line.split("\t");

        if(tags.size() < 5)
            continue;

        Point2DType sensorPoint;
        sensorPoint[0] = tags.at(0).toDouble();
        sensorPoint[1] = tags.at(1).toDouble();

        Point3DType geoPoint;
        geoPoint[0] = tags.at(2).toDouble();
        geoPoint[1] = tags.at(3).toDouble();
        geoPoint[2] = tags.at(4).toDouble();

        qDebug() << "Adding GCP sensor: " << sensorPoint[0] << " <-> geo: ";

        rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }

    rpcEstimator->GetOutput()->UpdateOutputInformation();

    m_outputImage = rpcEstimator->GetOutput();
}

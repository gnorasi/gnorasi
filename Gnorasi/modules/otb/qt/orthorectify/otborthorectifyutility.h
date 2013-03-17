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

#ifndef OTBORTHORECTIFYUTILITY_H
#define OTBORTHORECTIFYUTILITY_H

#include <QObject>

#include "../viewer/vector_globaldefs.h"


/*!
 * \brief The OTBOrthoUtility class
 *
 */
class OTBOrthoUtility : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool     useDEM                  READ useDEM             WRITE setUseDEM             NOTIFY useDEMChanged)
    Q_PROPERTY(QString  DEMPath                 READ DEMPath            WRITE setDEMPath            NOTIFY DEMPathChanged)
    Q_PROPERTY(int      elevationLevel          READ elevationLevel     WRITE setElevationLevel     NOTIFY elevationLevelChanged)
    Q_PROPERTY(int      utmZone                 READ utmZone            WRITE setUtmZone            NOTIFY utmZoneChanged)
    Q_PROPERTY(int      hemisphere              READ hemisphere         WRITE setHemisphere         NOTIFY hemisphereChanged)
    Q_PROPERTY(double   tmScale                 READ tmScale            WRITE setTmScale            NOTIFY tmScaleChanged)
    Q_PROPERTY(QString  imageFile               READ imageFile          WRITE setImageFile          NOTIFY imageFileChanged)
    Q_PROPERTY(QString  epsg                    READ epsg               WRITE setEpsg               NOTIFY epsgChanged)

public:
    /** Output */
    typedef GenericRSResampleImageFilter<VectorImageType,VectorImageType>       ResampleFilterType;
    typedef GenericRSTransform<>                                                TransformType;

    typedef TransformType::OutputPointType                                      OutputPointType;

    enum MAPPROJECTION{
        MP_UTM                                  = 0,
        MP_LAMBERT2                             = 1,
        MP_WGS84                                = 2,
        MP_EPSG                                 = 3,
        MP_TM                                   = 4
    };


    enum INTERPOLATOR{
        INTERPOLATOR_LINEAR                     = 0,
        INTERPOLATOR_NEAREST                    = 1,
        INTERPOLATOR_BCO                        = 2
    };


    /*!
     * \brief OTBOrthoUtility
     * \param parent
     *
     *  ctor
     */
    explicit                                    OTBOrthoUtility(QObject *parent = 0);


    bool                                        useDEM()const { return m_useDEM; }
    void                                        setUseDEM(bool b) { m_useDEM = b; }


    QString                                     DEMPath() const { return m_demPath;}
    void                                        setDEMPath(const QString &p);


    int                                         elevationLevel() const {return m_elevationLevel; }
    void                                        setElevationLevel(int l) { m_elevationLevel =l; }


    int                                         utmZone() const { return m_utmZone; }
    void                                        setUtmZone( int z);


    int                                         hemisphere() const { return m_hemisphere; }
    void                                        setHemisphere(int );

    double                                      tmScale() const { return m_tmScale; }
    void                                        setTmScale(double s) { m_tmScale = s; }


    QString                                     imageFile() const { return m_imageFile; }
    void                                        setImageFile(const QString &s);


    MAPPROJECTION                               mapProjection() const { return m_mapProjection; }
    void                                        setMapProjection(MAPPROJECTION mp);


    INTERPOLATOR                                interpolator()     const { return m_interpolator; }
    void                                        setInterpolator(INTERPOLATOR ir);


    QString                                     epsg() const { return m_epsg; }
    void                                        setEpsg(const QString &e);

    /*!
     * \brief vectorImage
     * \return
     */
    void                                        setupVectorImage(/*VectorImageType *vImg*/);

    /*!
     * \brief orthoRectifiedImage
     * \return
     */
    VectorImageType::Pointer                    orthoRectifiedImage()  { return m_outputImage; }

    /*!
     * \brief applyOrthoRectification
     */
    void                                        applyOrthoRectification();


    /*!
     * \brief projectionRef
     * \return
     */
    std::string                                 projectionRef();


    /*!
     * \brief projectRegion
     * \param sizeX
     * \param sizeY
     * \param spacingX
     * \param spacingY
     * \param originX
     * \param originY
     */
    void                                        projectRegion(unsigned int sizeX,
                                                              unsigned int sizeY,
                                                              double spacingX,
                                                              double spacingY,
                                                              double originX,
                                                              double originY);
    /*!
     * \brief inputImage
     * \return
     */
    VectorImageType::Pointer                    inputImage();

    /*!
     * \brief inverseTransform
     * \return
     */
    TransformType::Pointer                      inverseTransform() const { return m_InverseTransform; }

    /*!
     * \brief transform
     * \return
     */
    TransformType::Pointer                      transform() const { return m_Transform; }

    /*!
     * \brief wholeOutputSpacing
     * \return
     */
    VectorImageType::SpacingType                wholeOutputSpacing() const { return m_WholeOutputSpacing; }

    /*!
     * \brief wholeOutputSize
     * \return
     */
    VectorImageType::SizeType                   wholeOutputSize() const { return m_WholeOutputSize; }

    /*!
     * \brief wholeOutputOrigin
     * \return
     */
    VectorImageType::PointType                  wholeOutputOrigin() const { return m_WholeOutputOrigin; }

    /*!
     * \brief wholeOutputSpacing
     * \return
     */
    VectorImageType::SpacingType                outputSpacing() const { return m_OutputSpacing; }

    /*!
     * \brief wholeOutputSize
     * \return
     */
    VectorImageType::SizeType                   outputSize() const { return m_OutputSize; }

    /*!
     * \brief wholeOutputOrigin
     * \return
     */
    VectorImageType::PointType                  outputOrigin() const { return m_OutputOrigin; }

    /*!
     * \brief UpdateUTMTransform
     * \param zone
     * \param hemisphere
     */
    void                                        UpdateUTMTransform(int zone, bool hemisphere);


    /*!
     * \brief UpdateTMTransform
     * \param scale
     * \param falseEasting
     * \param falseNorthing
     */
    void                                        UpdateTMTransform(double scale, double falseEasting, double falseNorthing);

    /*!
     * \brief UpdateInputUTMTransform
     *
     *  Update the Input UTM Projection
     *
     * \param zone
     * \param north
     */
    void                                        UpdateInputUTMTransform(int zone, bool north);

    /*!
     * \brief UpdateInputEPSGTransform
     *
     *  Update the EPSG Projection
     *
     * \param rsidNumber
     */
    void                                        UpdateInputEPSGTransform(int rsidNumber);

    /*!
     * \brief InitializeInputLambertIITransform
     *
     *  Initialize the Input Lambert II Projection
     */
    void                                        InitializeInputLambertIITransform();


    /*!
     * \brief UpdateInputTMTransform
     *
     *  Initialize the Input transmecator proejction
     *
     * \param scale
     * \param falseEasting
     * \param falseNorthing
     */
    void                                        UpdateInputTMTransform(double scale, double falseEasting, double falseNorthing);

    /*!
     * \brief updateRPCModel
     */
    void                                        updateRPCModel(const QString &);


public slots:

    /*!
     * \brief updateUtmZone
     * \param i
     */
    void                                        updateUtmZone(int i) { setUtmZone(i); }

    /*!
     * \brief updateHemisphere
     * \param hm
     */
    void                                        updateHemisphere(int hm) { setHemisphere(hm); }

    /*!
     * \brief updateTmScale
     * \param sc
     */
    void                                        updateTmScale(double sc);

    /*!
     * \brief updateEpsgCode
     * \param c
     */
    void                                        updateEpsgCode(const QString &c) { if(validateEpsgCode(c.toInt()))setEpsg(c); }


    /*!
     * \brief updateEPSGTransform
     * \param rsidNumber
     */
    void                                        updateEPSGTransform(int rsidNumber);


    /*!
     * \brief UpdateWGS84Transform
     *
     *  Initialize the WGS84 transform
     */
    void                                        UpdateWGS84Transform();


signals:
    void                                        useDEMChanged();
    void                                        DEMPathChanged();
    void                                        elevationLevelChanged();
    void                                        utmZoneChanged();
    void                                        hemisphereChanged();
    void                                        tmScaleChanged();
    void                                        imageFileChanged();
    void                                        epsgChanged();
    void                                        nonValidEpsgCode();

    void                                        transformChanged();

public slots:

private:

    /*!
     * \brief validateEpsgCode
     * \param rsidNumber
     * \return
     */
    bool                                        validateEpsgCode(int rsidNumber);

    /*!
     * \brief updateOutputParameters
     */
    void                                        updateOutputParameters();


    // Output Image Information
    VectorImageType::SizeType                   m_OutputSize;
    VectorImageType::PointType                  m_OutputOrigin;
    VectorImageType::SpacingType                m_OutputSpacing;

    // Whole output projected image information
    VectorImageType::SizeType                   m_WholeOutputSize;
    VectorImageType::PointType                  m_WholeOutputOrigin;
    VectorImageType::SpacingType                m_WholeOutputSpacing;

    TransformType::Pointer                      m_Transform;
    TransformType::Pointer                      m_InverseTransform;

    // Outputs
    ResampleFilterType::Pointer                 m_Resampler;

    std::string                                 m_OutputProjectionRef;

    VectorImageType::Pointer                    m_outputImage;

    MAPPROJECTION                               m_mapProjection;
    INTERPOLATOR                                m_interpolator;

    bool                                        m_useDEM;
    QString                                     m_demPath;
    int                                         m_elevationLevel;
    int                                         m_utmZone;
    int                                         m_hemisphere;
    double                                      m_tmScale;
    QString                                     m_imageFile;
    QString                                     m_epsg;


    tranverseMercatorProjectionType::Pointer    tmMapProjection;
};

#endif // OTBORTHORECTIFYUTILITY_H

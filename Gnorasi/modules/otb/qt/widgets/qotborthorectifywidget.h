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

#ifndef VRN_QOTBORTHORECTIFYRWIDGET_H
#define VRN_QOTBORTHORECTIFYRWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "modules/otb/processors/Geometry/otborthorectifyprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"

#include <ogr_spatialref.h>

#include "../orthorectify/otborthorectifyutility.h"

using namespace otb;

class OTBOrthoUtility;

namespace voreen {

/*!
 * \brief The QOtbOrthoRectifyWidget class
 */
class VRN_QT_API                QOtbOrthoRectifyWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    /** typedef projection model*/
    typedef OTBOrthoUtility             ModelType;
    typedef VectorImageType             ImageType;
    typedef ModelType::TransformType    TransformType;
    typedef ModelType::OutputPointType  PointType;
    typedef VectorImageType::IndexType  IndexType;

    /*!
     * \brief QOtbOrthoRectifyWidget
     */
                                QOtbOrthoRectifyWidget(QWidget*, OTBOrthoRectifyProcessor* );

    /*!
     * \brief ~QOtbOrthoRectifyWidget
     */
    virtual                     ~QOtbOrthoRectifyWidget();

    /*!
     * \brief initialize
     */
    void                        initialize();

    /*!
     * \brief updateFromProcessor , implementation
     */
    virtual void                updateFromProcessor();
    
protected:


signals:

    
public slots:
    //!
    void                        updateOnTransformChange();



private slots:

    /*!
     * \brief InitializeAction
     */
    void                        InitializeAction();

    /*!
     * \brief onOriginCartoCurrentSelectionChanged
     */
    void                        onOriginCartoCurrentSelectionChanged(int );

    /*!
     * \brief onHemisphereChanged
     */
    void                        onHemisphereChanged(int );

    /*!
     * \brief onMapProjectionChanged
     */
    void                        onMapProjectionChanged(int );

    /*!
     * \brief onDEMButtonGroupChanged
     */
    void                        onDEMButtonGroupChanged(int );

    /*!
     * \brief onInterpolatorChanged
     */
    void                        onInterpolatorChanged(int );

    /*!
     * \brief onDemFilePathButtonClicked
     */
    void                        onDemFilePathButtonClicked();

    /*!
     * \brief onNonValidEpsgCode
     * \param c
     */
    void                        onNonValidEpsgCode();

    /*!
     * \brief onRPCModelCheckBoxChanged
     */
    void                        onRPCModelCheckBoxChanged(bool );

private:

    /*!
     * \brief UpToDateTransform
     */
    void                        UpToDateTransform();

    /*!
     * \brief UpdateMapParam
     *
     *  Change east/north when change Long/Lat values
     */
    void                        UpdateMapParam();


    /*!
     * \brief UpdateLongLat
     *
     *  Update longitude and latitude values
     */
    void                        UpdateLongLat();

    /*!
     * \brief UpdateOutputParameters
     *
     *  Update parameter for output image
     */
    void                        UpdateOutputParameters();

    /*!
     * \brief UpdateUTMTransform
     */
    void                        UpdateUTMTransform();

    /*!
     * \brief UpdateTMTransform
     */
    void                        UpdateTMTransform();

    /*!
     * \brief UpdateWGS84Transform
     */
    void                        UpdateWGS84Transform();

    /*!
     * \brief UpdateEPSGTransform
     */
    void                        UpdateEPSGTransform();


    /*!
     * \brief UpdateInputUTMTransform
     */
    void                        UpdateInputUTMTransform();

    /*!
     * \brief UpdateInputLambertIITransform
     */
    void                        UpdateInputLambertIITransform();

    /*!
     * \brief UpdateInputTMTransform
     */
    void                        UpdateInputTMTransform();

    /*!
     * \brief UpdateInputEPSGTransform
     */
    void                        UpdateInputEPSGTransform();

    /*!
     * \brief FindParameter
     * \param oSRS
     * \param inParam
     * \param paramValue
     * \return
     *
     *  Find parameter value in the projection Ref tree
     */
    bool                        FindParameter(OGRSpatialReference oSRS, const char * inParam, double * paramValue);

    /*!
     * \brief RetrieveInputProjection
     */
    void                        RetrieveInputProjection();

    /*!
     * \brief UpdateOutputRegion
     *
     *  Method to update the model output attributes
     */
    void                        UpdateOutputRegion();

    /*!
     * \brief UpdateOriginGeographicalCoordinates
     *
     *  Method updating the Output Origin Geographical Coordinates
     */
    void                        UpdateOriginGeographicalCoordinates();

    /*!
     * \brief inVectorPort
     * \return
     */
    OTBVectorImagePort*         inVectorPort();

    /*!
     * \brief outVectorPort
     * \return
     */
    OTBVectorImagePort*         outVectorPort();

    /*!
     * \brief setupOutputByMapProjectionUTM
     */
    void                        setupOutputByMapProjectionUTM();

    /*!
     * \brief setupOutputByMapProjectionTM
     */
    void                        setupOutputByMapProjectionTM();

    /*!
     * \brief setupOutputByMapProjectionLambert2
     */
    void                        setupOutputByMapProjectionLambert2();

    /*!
     * \brief setupOutputByMapProjectionWGS84
     */
    void                        setupOutputByMapProjectionWGS84();

    /*!
     * \brief setupOutputByMapProjectionEPSG
     */
    void                        setupOutputByMapProjectionEPSG();

    /*!
     * \brief setupUpperLeftAsOutputOrgin
     */
    void                        setupUpperLeftAsOutputOrgin();

    /*!
     * \brief setupCenterAsOutputOrigin
     */
    void                        setupCenterAsOutputOrigin();

    /*!
     * \brief setupUserDefinedParametersAsOutputOrigin
     */
    void                        setupUserDefinedParametersAsOutputOrigin();

    /*!
     * \brief setEnabledOCCGroupBoxWidgets
     */
    void                        setEnabledOCCGroupBoxWidgets(bool );

    /*!
     * \brief setupLayoutIncome
     */
    void                        setupLayoutIncome();

    /*!
     * \brief setupLayoutOutcome
     */
    void                        setupLayoutOutcome();

    /*!
     * \brief setupLayoutSettings
     */
    void                        setupLayoutSettings();

    /*!
     * \brief setupByNothernHemishere
     */
    void                        setupByNothernHemishere();

    /*!
     * \brief setupBySoutherHemishere
     */
    void                        setupBySoutherHemishere();


    static const std::string    loggerCat_;


    QTabWidget                  *m_pTabWidget;


    QWidget                     *m_pInputTab;
    QWidget                     *m_pOutputTab;
    QWidget                     *m_pSettingsTab;


    QGroupBox                   *m_pInputGroupBox;
    QGroupBox                   *m_pOutputOrgCartoCoordGroupBox;
    QGroupBox                   *m_pOutputMapParametersGroupBox;
    QGroupBox                   *m_pOutputSizeSpacingGroupBox;
    QGroupBox                   *m_pSettingsDEMSelectionGroupBox;


    QRadioButton                *m_pOutputUsrDefParamRadionButton;
    QRadioButton                *m_pOutputUpperLeftRadionButton;
    QRadioButton                *m_pOutputCenteredRadionButton;
    QRadioButton                *m_pOutputNorthHemisphereRadioButton;
    QRadioButton                *m_pOutputSouthHemisphereRadioButton;
    QRadioButton                *m_pSettingsUseDemRadioButton;
    QRadioButton                *m_pSettingsUseAvrgElevationRadioButton;


    QLabel                      *m_pOutputMapProjectionLabel;
    QLabel                      *m_pOutputLngLabel;
    QLabel                      *m_pOutputLatLabel;
    QLabel                      *m_pOutputEastingLabel;
    QLabel                      *m_pOutputNorthingLabel;
    QLabel                      *m_pOutputSizeXLabel;
    QLabel                      *m_pOutputSizeYLabel;
    QLabel                      *m_pOutputSpacingXLabel;
    QLabel                      *m_pOutputSpacingYLabel;
    QLabel                      *m_pOutputMapParamZoneLabel;
    QLabel                      *m_pOutputRSIDLabel;
    QLabel                      *m_pOutputTMScaleLabel;
    QLabel                      *m_pSettingsInterpolatorLabel;
    QLabel                      *m_pSettingsInterpolatorParametersLabel;
    QLabel                      *m_pSettingsElevationValueLabel;
    QLabel                      *m_pSettingsBCOLabel;
    QLabel                      *m_pSettingsDemFilePathLabel;


    QSpinBox                    *m_pOutputZoneSpinBox;
    QSpinBox                    *m_pOutputSizeXSpinBox;
    QSpinBox                    *m_pOutputSizeYSpinBox;
    QSpinBox                    *m_pSettingsBCOSpinBox;
    QSpinBox                    *m_pSettingsElevationValueSpinBox;


    QPushButton                 *m_pSettingsDemFilePushButton;


    QDoubleSpinBox              *m_pOutputLngDoubleSpinBox;
    QDoubleSpinBox              *m_pOutputLatDoubleSpinBox;
    QDoubleSpinBox              *m_pOutputEastingDoubleSpinBox;
    QDoubleSpinBox              *m_pOutputNorthingDoubleSpinBox;
    QDoubleSpinBox              *m_pOutputTMScaleDoubleSpinBox;
    QDoubleSpinBox              *m_pOutputSpacingXDoubleSpinBox;
    QDoubleSpinBox              *m_pOutputSpacingYDoubleSpinBox;


    QComboBox                   *m_pOutputMapProjectionComboBox;
    QComboBox                   *m_pSettingsInterpolatorComboBox;

    QCheckBox                   *m_pIncomeEstimateRPCModelCheckBox;


    QLineEdit                   *m_pOutputRSIDLineEdit;


    int                         m_currentOCCMethod;

    /*!
     * \brief m_pOTBOrthoUtility
     */
    OTBOrthoUtility             *m_pOTBOrthoUtility;


    // Flag to determine if the input Projection Map is unknowm
    bool                        m_InputProjectionUnknown;
};

} // end of namespace voreen

#endif // VRN_QOTBORTHORECTIFYRWIDGET_H

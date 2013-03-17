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

#include "qotborthorectifywidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

#include "../viewer/utils/itiotbimagemanager.h"


using namespace itiviewer;
using namespace otb;


namespace voreen {


const std::string QOtbOrthoRectifyWidget::loggerCat_("voreen.QOtbOrthoRectifyWidget");


QOtbOrthoRectifyWidget::QOtbOrthoRectifyWidget(QWidget *parent, OTBOrthoRectifyProcessor *orprocessor)
    : QProcessorWidget(orprocessor, parent)
{
    tgtAssert(orprocessor, "No QOtbImageViewerWidget processor");

    setWindowTitle(QString::fromStdString(orprocessor->getName()));
    resize(500, 400);
    setPosition(QApplication::desktop()->width()/5,QApplication::desktop()->height()/4);

    m_InputProjectionUnknown = false;

    m_pOTBOrthoUtility                      = new OTBOrthoUtility(this);
    connect(m_pOTBOrthoUtility,SIGNAL(nonValidEpsgCode()),this,SLOT(onNonValidEpsgCode()));
    connect(m_pOTBOrthoUtility,SIGNAL(transformChanged()),this,SLOT(updateOnTransformChange()));
}

//!
void QOtbOrthoRectifyWidget::initialize(){
    QProcessorWidget::initialize();

    //---------------------------------------------------
    setupLayoutIncome();
    setupLayoutOutcome();
    setupLayoutSettings();

    //---------------------------------------------------
    m_pTabWidget                            = new QTabWidget(this);
    m_pTabWidget->addTab(m_pOutputTab, tr("Output"));
    m_pTabWidget->addTab(m_pInputTab, tr("Input"));
    m_pTabWidget->addTab(m_pSettingsTab, tr("Settings"));

    //---------------------------------------------------
    //! create a vertical box layout and add the splitter on it
    QVBoxLayout *layout                     = new QVBoxLayout(this);
    layout->addWidget(m_pTabWidget);
    this->setLayout(layout);

    //!
    initialized_ = true;
//    hide();
}


void QOtbOrthoRectifyWidget::setupLayoutIncome(){
    m_pInputTab = new QWidget(this);

    m_pIncomeEstimateRPCModelCheckBox       = new QCheckBox(tr("Estimate Input RPC Model"),this);
    m_pIncomeEstimateRPCModelCheckBox->setToolTip("the otb::GCPsToRPCSensorModelImageFilter needs at "
        "least 16 GCPs to estimate a proper RPC sensor model, although no warning will be reported to the user if the number of GCPs is lower than 16.");


    m_pInputGroupBox                        = new QGroupBox;
    m_pInputGroupBox->setTitle(tr("Information"));

    //---------------------------------------------------
    QVBoxLayout *igbLayout                  = new QVBoxLayout;

    m_pInputGroupBox->setLayout(igbLayout);

    //---------------------------------------------------
    QVBoxLayout *vbLayout                   = new QVBoxLayout;

//    vbLayout->addWidget(m_pInputGroupBox);

    vbLayout->addWidget(m_pIncomeEstimateRPCModelCheckBox);
    vbLayout->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Expanding,QSizePolicy::Expanding));

    m_pInputTab->setLayout(vbLayout);

    connect(m_pIncomeEstimateRPCModelCheckBox,SIGNAL(toggled(bool)),this,SLOT(onRPCModelCheckBoxChanged(bool)));
}


void QOtbOrthoRectifyWidget::setupLayoutOutcome(){
    m_pOutputTab = new QWidget(this);

    //---------------------------------------------------
    m_pOutputOrgCartoCoordGroupBox          = new QGroupBox;
    m_pOutputOrgCartoCoordGroupBox->setTitle(tr("Origin Cartographic Coordinates"));
    m_pOutputMapParametersGroupBox          = new QGroupBox(tr("Map parameters"),this);
    m_pOutputSizeSpacingGroupBox            = new QGroupBox(tr("Size and Spacing"),this);

    //---------------------------------------------------
    m_pOutputMapProjectionLabel             = new QLabel(tr("Map projection"),this);
    m_pOutputMapProjectionComboBox          = new QComboBox(this);
    QStringList list ;
    list << tr("UTM") << tr("Lambert2") << tr("WGS84") << tr("EPSG") << tr("TM");
    m_pOutputMapProjectionComboBox->addItems(list);
    QHBoxLayout *mpHLayout                  = new QHBoxLayout;
    mpHLayout->addWidget(m_pOutputMapProjectionLabel);
    mpHLayout->addWidget(m_pOutputMapProjectionComboBox);

    //---------------------------------------------------
    QButtonGroup *pButtonGroup              = new QButtonGroup(this);
    m_pOutputUsrDefParamRadionButton        = new QRadioButton(tr("User Defined Parameters"),this);
    m_pOutputUpperLeftRadionButton          = new QRadioButton(tr("Upper-Left as output origin"),this);
    m_pOutputCenteredRadionButton           = new QRadioButton(tr("Centered"),this);

    m_pOutputUsrDefParamRadionButton->setChecked(true);
    pButtonGroup->addButton(m_pOutputUsrDefParamRadionButton);
    pButtonGroup->setId(m_pOutputUsrDefParamRadionButton,1);
    pButtonGroup->addButton(m_pOutputUpperLeftRadionButton);
    pButtonGroup->setId(m_pOutputUpperLeftRadionButton,2);
    pButtonGroup->addButton(m_pOutputCenteredRadionButton);
    pButtonGroup->setId(m_pOutputCenteredRadionButton,3);
    m_currentOCCMethod                      = 1;

    QVBoxLayout *rdbVBoxLayout              = new QVBoxLayout;
    rdbVBoxLayout->addWidget(m_pOutputUsrDefParamRadionButton);
    rdbVBoxLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Preferred));

    QVBoxLayout *rdbVBoxLayout1             = new QVBoxLayout;
    rdbVBoxLayout1->addWidget(m_pOutputUpperLeftRadionButton);
    rdbVBoxLayout1->addWidget(m_pOutputCenteredRadionButton);

    QHBoxLayout *rdbHBoxLayout              = new QHBoxLayout;
    rdbHBoxLayout->addLayout(rdbVBoxLayout);
    rdbHBoxLayout->addLayout(rdbVBoxLayout1);

    //---------------------------------------------------
    m_pOutputLngLabel                       = new QLabel(tr("Longitude"),this);
    m_pOutputLatLabel                       = new QLabel(tr("Latitude"),this);
    m_pOutputEastingLabel                   = new QLabel(tr("Easting"),this);
    m_pOutputNorthingLabel                  = new QLabel(tr("Northing"),this);
    m_pOutputTMScaleLabel                   = new QLabel(tr("Scale"),this);

    m_pOutputLngDoubleSpinBox               = new QDoubleSpinBox(this);
    m_pOutputLatDoubleSpinBox               = new QDoubleSpinBox(this);
    m_pOutputEastingDoubleSpinBox           = new QDoubleSpinBox(this);
    m_pOutputNorthingDoubleSpinBox          = new QDoubleSpinBox(this);
    m_pOutputTMScaleDoubleSpinBox           = new QDoubleSpinBox(this);

    m_pOutputLngDoubleSpinBox->setRange(0.0,100000000.0);
    m_pOutputLngDoubleSpinBox->setDecimals(8);
    m_pOutputLatDoubleSpinBox->setRange(0.0,10000000.0);
    m_pOutputLatDoubleSpinBox->setDecimals(8);
    m_pOutputTMScaleDoubleSpinBox->setRange(0.0,100000.00);
    m_pOutputTMScaleDoubleSpinBox->setDecimals(4);
    m_pOutputTMScaleDoubleSpinBox->setValue(1.0);
    m_pOutputTMScaleDoubleSpinBox->setVisible(false);
    m_pOutputTMScaleLabel->setVisible(false);

    m_pOutputEastingDoubleSpinBox->setRange(0.0,1000000000.00);
    m_pOutputEastingDoubleSpinBox->setDecimals(2);
    m_pOutputNorthingDoubleSpinBox->setRange(0.0,1000000000.0);
    m_pOutputNorthingDoubleSpinBox->setDecimals(2);

    QFrame *hline1                          = new QFrame(this);
    hline1->setFrameShape(QFrame::HLine);
    hline1->setFrameStyle(QFrame::Sunken);
    QFrame *hline2                          = new QFrame(this);
    hline2->setFrameShape(QFrame::HLine);
    hline2->setFrameStyle(QFrame::Sunken);

    QVBoxLayout *mpVBoxLayout               = new QVBoxLayout;
    mpVBoxLayout->addWidget(m_pOutputTMScaleLabel);
    mpVBoxLayout->addWidget(m_pOutputTMScaleDoubleSpinBox);
    mpVBoxLayout->addWidget(m_pOutputLngLabel);
    mpVBoxLayout->addWidget(m_pOutputLngDoubleSpinBox);
    mpVBoxLayout->addWidget(hline1);
    mpVBoxLayout->addWidget(m_pOutputLatLabel);
    mpVBoxLayout->addWidget(m_pOutputLatDoubleSpinBox);
    mpVBoxLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Expanding));

    QVBoxLayout *mpVBoxLayout1              = new QVBoxLayout;
    mpVBoxLayout1->addWidget(m_pOutputEastingLabel);
    mpVBoxLayout1->addWidget(m_pOutputEastingDoubleSpinBox);
    mpVBoxLayout1->addWidget(hline2);
    mpVBoxLayout1->addWidget(m_pOutputNorthingLabel);
    mpVBoxLayout1->addWidget(m_pOutputNorthingDoubleSpinBox);
    mpVBoxLayout1->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Expanding));

    QHBoxLayout *occlayout                  = new QHBoxLayout;
    occlayout->addLayout(mpVBoxLayout);
    occlayout->addLayout(mpVBoxLayout1);

    m_pOutputOrgCartoCoordGroupBox->setLayout(occlayout);

    //---------------------------------------------------
    m_pOutputMapParamZoneLabel              = new QLabel(tr("Zone"),this);
    m_pOutputRSIDLabel                      = new QLabel(tr("RSID"),this);

    m_pOutputZoneSpinBox                    = new QSpinBox(this);
    m_pOutputZoneSpinBox->setMinimum(0);
    m_pOutputZoneSpinBox->setMaximum(1000);

    m_pOutputRSIDLineEdit                   = new QLineEdit(this);
    m_pOutputRSIDLineEdit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pOutputNorthHemisphereRadioButton     = new QRadioButton(tr("Northern hemisphere"),this);
    m_pOutputSouthHemisphereRadioButton     = new QRadioButton(tr("Southern hemisphere"), this);
    QButtonGroup *pButtonGroup2             = new QButtonGroup(this);
    pButtonGroup2->addButton(m_pOutputNorthHemisphereRadioButton,1);
    pButtonGroup2->addButton(m_pOutputSouthHemisphereRadioButton,2);

    m_pOutputNorthHemisphereRadioButton->setChecked(true);

    QVBoxLayout *rsVBOxLayout               = new QVBoxLayout;
    rsVBOxLayout->addWidget(m_pOutputRSIDLabel);
    rsVBOxLayout->addWidget(m_pOutputRSIDLineEdit);
    m_pOutputRSIDLabel->setVisible(false);
    m_pOutputRSIDLineEdit->setVisible(false);

    QVBoxLayout *mappVBoxLayout             = new QVBoxLayout;
    mappVBoxLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Expanding));
    mappVBoxLayout->addWidget(m_pOutputMapParamZoneLabel);
    mappVBoxLayout->addWidget(m_pOutputZoneSpinBox);
    mappVBoxLayout->addLayout(rsVBOxLayout);
    mappVBoxLayout->addWidget(m_pOutputNorthHemisphereRadioButton);
    mappVBoxLayout->addWidget(m_pOutputSouthHemisphereRadioButton);
    mappVBoxLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Expanding));

    m_pOutputMapParametersGroupBox->setLayout(mappVBoxLayout);


    //---------------------------------------------------
    m_pOutputSizeXLabel                     = new QLabel(tr("Size x"),this);
    m_pOutputSizeYLabel                     = new QLabel(tr("Size Y"),this);
    m_pOutputSpacingXLabel                  = new QLabel(tr("spacing x"),this);
    m_pOutputSpacingYLabel                  = new QLabel(tr("spacing y"),this);

    m_pOutputSizeXSpinBox                   = new QSpinBox(this);
    m_pOutputSizeXSpinBox->setRange(0,10000000000);
    m_pOutputSizeYSpinBox                   = new QSpinBox(this);
    m_pOutputSizeYSpinBox->setRange(0,10000000000);
    m_pOutputSpacingXDoubleSpinBox          = new QDoubleSpinBox(this);
    m_pOutputSpacingXDoubleSpinBox->setRange(0.0,1000000.0);
    m_pOutputSpacingXDoubleSpinBox->setDecimals(7);
    m_pOutputSpacingYDoubleSpinBox          = new QDoubleSpinBox(this);
    m_pOutputSpacingYDoubleSpinBox->setRange(-1000000.0,1000000.0);
    m_pOutputSpacingYDoubleSpinBox->setDecimals(7);

    QVBoxLayout *ssLayout                   = new QVBoxLayout;

    ssLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Fixed,QSizePolicy::Expanding));
    ssLayout->addWidget(m_pOutputSizeXLabel);
    ssLayout->addWidget(m_pOutputSizeXSpinBox);
    ssLayout->addWidget(m_pOutputSizeYLabel);
    ssLayout->addWidget(m_pOutputSizeYSpinBox);
    ssLayout->addWidget(m_pOutputSpacingXLabel);
    ssLayout->addWidget(m_pOutputSpacingXDoubleSpinBox);
    ssLayout->addWidget(m_pOutputSpacingYLabel);
    ssLayout->addWidget(m_pOutputSpacingYDoubleSpinBox);
    ssLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Fixed,QSizePolicy::Expanding));

    m_pOutputSizeSpacingGroupBox->setLayout(ssLayout);


    //---------------------------------------------------
    QHBoxLayout *hbLayout                   = new QHBoxLayout;;

    hbLayout->addWidget(m_pOutputMapParametersGroupBox);
    hbLayout->addWidget(m_pOutputSizeSpacingGroupBox);

    //---------------------------------------------------

    QFrame *hline_                          = new QFrame(this);
    hline_->setFrameStyle(QFrame::Sunken);
    hline_->setFrameShape(QFrame::HLine);

    QFrame *hline1_                         = new QFrame(this);
    hline1_->setFrameStyle(QFrame::Sunken);
    hline1_->setFrameShape(QFrame::HLine);

    QVBoxLayout *vbLayout                   = new QVBoxLayout;

    vbLayout->addLayout(mpHLayout);
    vbLayout->addWidget(hline1_);
    vbLayout->addWidget(m_pOutputOrgCartoCoordGroupBox);
    vbLayout->addLayout(rdbHBoxLayout);
    vbLayout->addWidget(hline_);
    vbLayout->addLayout(hbLayout);

    m_pOutputTab->setLayout(vbLayout);

    // setup connections
    connect(pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(onOriginCartoCurrentSelectionChanged(int)));
    connect(pButtonGroup2,SIGNAL(buttonClicked(int)),this,SLOT(onHemisphereChanged(int)));
    connect(m_pOutputMapProjectionComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onMapProjectionChanged(int)));
    connect(m_pOutputLatDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateLat(double)));
    connect(m_pOutputLngDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateLng(double)));
    connect(m_pOutputEastingDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateEasting(double)));
    connect(m_pOutputNorthingDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateNorthing(double)));
    connect(m_pOutputSizeXSpinBox,SIGNAL(valueChanged(int)),m_pOTBOrthoUtility,SLOT(updateSizeX(int)));
    connect(m_pOutputSizeYSpinBox,SIGNAL(valueChanged(int)),m_pOTBOrthoUtility,SLOT(updateSizeY(int)));
    connect(m_pOutputSpacingXDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateSpacingX(double)));
    connect(m_pOutputSpacingYDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateSpacingY(double)));
    connect(m_pOutputTMScaleDoubleSpinBox,SIGNAL(valueChanged(double)),m_pOTBOrthoUtility,SLOT(updateTmScale(double)));
    connect(m_pOutputRSIDLineEdit,SIGNAL(textChanged(QString)),m_pOTBOrthoUtility,SLOT(updateEpsgCode(QString)));

    m_pOutputRSIDLineEdit->setText(QString::number(4121));
}


void QOtbOrthoRectifyWidget::setupLayoutSettings(){
    m_pSettingsTab                          = new QWidget(this);

    m_pSettingsBCOLabel                     = new QLabel(tr("BCO"),this);
    m_pSettingsDemFilePathLabel             = new QLabel(tr("No file selected"),this);
    m_pSettingsDemFilePushButton            = new QPushButton(tr("Edit path"),this);
    m_pSettingsDEMSelectionGroupBox         = new QGroupBox(tr("DEM selection"),this);
    m_pSettingsElevationValueLabel          = new QLabel(tr("Elevavtion value"),this);
    m_pSettingsElevationValueSpinBox        = new QSpinBox(this);
    m_pSettingsInterpolatorComboBox         = new QComboBox(this);
    m_pSettingsInterpolatorLabel            = new QLabel(tr("Interpolator"),this);
    m_pSettingsInterpolatorParametersLabel  = new QLabel(tr("<h6>Interpolator parameters</h6>"),this);
    m_pSettingsUseAvrgElevationRadioButton  = new QRadioButton(tr("Use average elevation"),this);
    m_pSettingsUseDemRadioButton            = new QRadioButton(tr("Use DEM"),this);
    m_pSettingsBCOSpinBox                   = new QSpinBox(this);
    QButtonGroup *pButtonGroup              = new QButtonGroup(this);

    QStringList list;
    list << tr("Linear") << tr("Nearest") << tr("BCO");
    m_pSettingsInterpolatorComboBox->addItems(list);
    m_pSettingsBCOSpinBox->setRange(2,100);
    m_pSettingsElevationValueSpinBox->setRange(0,100);
    pButtonGroup->addButton(m_pSettingsUseAvrgElevationRadioButton,1);
    pButtonGroup->addButton(m_pSettingsUseDemRadioButton,2);
    m_pSettingsBCOLabel->setVisible(false);
    m_pSettingsBCOSpinBox->setVisible(false);
    m_pSettingsInterpolatorComboBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsBCOSpinBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsElevationValueSpinBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsDemFilePushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsBCOLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsBCOSpinBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsInterpolatorParametersLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSettingsElevationValueLabel->setEnabled(false);
    m_pSettingsElevationValueSpinBox->setEnabled(false);

    QFrame *hline                           = new QFrame(this);
    hline->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    hline->setFrameShape(QFrame::HLine);
    hline->setFrameShadow(QFrame::Sunken);
    QFrame *hline1                          = new QFrame(this);
    hline1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    hline1->setFrameShape(QFrame::HLine);
    hline1->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *vBoxLayout_                = new QVBoxLayout;
    vBoxLayout_->addWidget(m_pSettingsUseDemRadioButton);
    vBoxLayout_->addWidget(m_pSettingsDemFilePushButton);
    vBoxLayout_->addWidget(m_pSettingsDemFilePathLabel);
    vBoxLayout_->addWidget(m_pSettingsUseAvrgElevationRadioButton);
    vBoxLayout_->addWidget(m_pSettingsElevationValueLabel);
    vBoxLayout_->addWidget(m_pSettingsElevationValueSpinBox);

    m_pSettingsDEMSelectionGroupBox->setLayout(vBoxLayout_);

    QVBoxLayout *vbLayout                   = new QVBoxLayout;
    vbLayout->addWidget(m_pSettingsInterpolatorLabel);
    vbLayout->addWidget(m_pSettingsInterpolatorComboBox);
    vbLayout->addWidget(hline);
    vbLayout->addWidget(m_pSettingsInterpolatorParametersLabel);
    vbLayout->addWidget(m_pSettingsInterpolatorLabel);
    vbLayout->addWidget(m_pSettingsBCOLabel);
    vbLayout->addWidget(m_pSettingsBCOSpinBox);
    vbLayout->addWidget(hline1);
    vbLayout->addWidget(m_pSettingsDEMSelectionGroupBox);
    vbLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Expanding));

    m_pSettingsTab->setLayout(vbLayout);

    connect(m_pSettingsInterpolatorComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onInterpolatorChanged(int)));
    connect(pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(onDEMButtonGroupChanged(int)));
    connect(m_pSettingsDemFilePushButton,SIGNAL(clicked()),this,SLOT(onDemFilePathButtonClicked()));

    m_pSettingsUseAvrgElevationRadioButton->setChecked(true);
}


//!
void QOtbOrthoRectifyWidget::updateFromProcessor(){
    OTBOrthoRectifyProcessor* otborProcessor = dynamic_cast<OTBOrthoRectifyProcessor*>(processor_);

    if(!otborProcessor)
        return;

    QString cmd = QString::fromStdString(otborProcessor->getCommandName());
    if(!cmd.compare("update")){ // then do not set any images just update the the data based on the filter set by the gui parameters
        OTBVectorImagePort *vPort = outVectorPort();
        if(!vPort)
            return;

        UpdateOutputRegion();

        m_pOTBOrthoUtility->applyOrthoRectification();

        vPort->setData(m_pOTBOrthoUtility->orthoRectifiedImage());

    } else{ // set the raster and vector images
        //! get the first port
        OTBVectorImagePort *vPort = inVectorPort();
        if(!vPort)
            return;

        VectorImageType::Pointer vImg = vPort->getData();

        VectorImageType::Pointer outImg = m_pOTBOrthoUtility->orthoRectifiedImage();

        if(vImg.IsNotNull() && outImg.IsNull()){

            QString path = ITIOTBIMAGEMANAGER->imageFile(vPort);


            // TEST

            ReaderType::Pointer reader = ReaderType::New();
            reader->SetFileName(path.toStdString());
            reader->GenerateOutputInformation();
            reader->Update();
            typedef otb::InverseSensorModel<double> ModelType;
            ModelType::Pointer model = ModelType::New();
            model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());
            if (!model->IsValidSensorModel())
            {
                qDebug() << "this is not a valid sensor model";
            }else{

//                GCPsToSensorModelFilterType::Pointer rpcEstimator =
//                  GCPsToSensorModelFilterType::New();
//                rpcEstimator->SetInput(reader->GetOutput());

            }

            // TEST


            m_pOTBOrthoUtility->setImageFile(path);

            RetrieveInputProjection();

            UpdateEPSGTransform();
//            UpdateUTMTransform();

            InitializeAction();

            UpdateMapParam();
        }
    }
}


OTBVectorImagePort* QOtbOrthoRectifyWidget::inVectorPort(){
    OTBOrthoRectifyProcessor* otborProcessor = dynamic_cast<OTBOrthoRectifyProcessor*>(processor_);

    if(!otborProcessor)
        return 0;

    std::vector<Port*> l = otborProcessor->getInports();
    if(l.empty()){
        qDebug() << "image port list is empty..";
        return 0;
    }

    for(int i = 0 ; i < l.size(); i++){
        Port *port = l.at(i);
        QString className = QString::fromStdString(port->getName());

        if(!className.compare(QLatin1String("Vector Image Input"))){

            OTBVectorImagePort* vPort = (OTBVectorImagePort*)port;
            if(vPort)
                return vPort;
        }
    }

    return 0;
}


OTBVectorImagePort* QOtbOrthoRectifyWidget::outVectorPort(){
    OTBOrthoRectifyProcessor* otborProcessor = dynamic_cast<OTBOrthoRectifyProcessor*>(processor_);

    if(!otborProcessor)
        return 0;

    std::vector<Port*> l = otborProcessor->getOutports();
    if(l.empty()){
        qDebug() << "image port list is empty..";
        return 0;
    }

    for(int i = 0 ; i < l.size(); i++){
        Port *port = l.at(i);
        QString className = QString::fromStdString(port->getName());

        if(!className.compare(QLatin1String("Vector Image Output")) ){
            OTBVectorImagePort *vPort = (OTBVectorImagePort*)port;
            if(vPort)
                return vPort;
        }
    }

    return 0;
}


void QOtbOrthoRectifyWidget::setupOutputByMapProjectionUTM(){
    m_pOutputZoneSpinBox->setVisible(true);
    m_pOutputMapParamZoneLabel->setVisible(true);
    m_pOutputSouthHemisphereRadioButton->setVisible(true);
    m_pOutputNorthHemisphereRadioButton->setVisible(true);
    m_pOutputRSIDLabel->setVisible(false);
    m_pOutputRSIDLineEdit->setVisible(false);
    m_pOutputTMScaleDoubleSpinBox->setVisible(false);
    m_pOutputTMScaleLabel->setVisible(false);

    m_pOTBOrthoUtility->setMapProjection(OTBOrthoUtility::MP_UTM);
}


void QOtbOrthoRectifyWidget::setupOutputByMapProjectionTM(){
    m_pOutputZoneSpinBox->setVisible(false);
    m_pOutputMapParamZoneLabel->setVisible(false);
    m_pOutputSouthHemisphereRadioButton->setVisible(false);
    m_pOutputNorthHemisphereRadioButton->setVisible(false);
    m_pOutputRSIDLabel->setVisible(false);
    m_pOutputRSIDLineEdit->setVisible(false);
    m_pOutputTMScaleDoubleSpinBox->setVisible(true);
    m_pOutputTMScaleLabel->setVisible(true);

    m_pOTBOrthoUtility->setMapProjection(OTBOrthoUtility::MP_TM);
}


void QOtbOrthoRectifyWidget::setupOutputByMapProjectionLambert2(){
    m_pOutputZoneSpinBox->setVisible(false);
    m_pOutputMapParamZoneLabel->setVisible(false);
    m_pOutputNorthHemisphereRadioButton->setVisible(false);
    m_pOutputSouthHemisphereRadioButton->setVisible(false);
    m_pOutputRSIDLabel->setVisible(false);
    m_pOutputRSIDLineEdit->setVisible(false);
    m_pOutputTMScaleDoubleSpinBox->setVisible(false);
    m_pOutputTMScaleLabel->setVisible(false);

    m_pOTBOrthoUtility->setMapProjection(OTBOrthoUtility::MP_LAMBERT2);
}


void QOtbOrthoRectifyWidget::setupOutputByMapProjectionWGS84(){
    m_pOutputZoneSpinBox->setVisible(false);
    m_pOutputMapParamZoneLabel->setVisible(false);
    m_pOutputNorthHemisphereRadioButton->setVisible(false);
    m_pOutputSouthHemisphereRadioButton->setVisible(false);
    m_pOutputRSIDLabel->setVisible(false);
    m_pOutputRSIDLineEdit->setVisible(false);
    m_pOutputTMScaleDoubleSpinBox->setVisible(false);
    m_pOutputTMScaleLabel->setVisible(false);

    m_pOTBOrthoUtility->setMapProjection(OTBOrthoUtility::MP_WGS84);
}


void QOtbOrthoRectifyWidget::setupOutputByMapProjectionEPSG(){
    m_pOutputZoneSpinBox->setVisible(false);
    m_pOutputMapParamZoneLabel->setVisible(false);
    m_pOutputNorthHemisphereRadioButton->setVisible(false);
    m_pOutputSouthHemisphereRadioButton->setVisible(false);
    m_pOutputRSIDLabel->setVisible(true);
    m_pOutputRSIDLineEdit->setVisible(true);
    m_pOutputTMScaleDoubleSpinBox->setVisible(false);
    m_pOutputTMScaleLabel->setVisible(false);

    m_pOTBOrthoUtility->setMapProjection(OTBOrthoUtility::MP_EPSG);
}


void QOtbOrthoRectifyWidget::onHemisphereChanged(int idx){
    if(idx == 0) { // meaning its the northern{
        setupByNothernHemishere();
    }else { // its the southern hemisphere
        setupBySoutherHemishere();
    }

    m_pOTBOrthoUtility->setHemisphere(idx);
}


void QOtbOrthoRectifyWidget::setupByNothernHemishere(){

}


void QOtbOrthoRectifyWidget::setupBySoutherHemishere(){

}


void QOtbOrthoRectifyWidget::onMapProjectionChanged(int idx){

    if(m_pOutputUsrDefParamRadionButton->isChecked()){
        switch(idx){
            case 0:
                setupOutputByMapProjectionUTM();
                m_pOTBOrthoUtility->UpdateInputUTMTransform(m_pOutputZoneSpinBox->value(),m_pOutputNorthHemisphereRadioButton->isChecked());
                break;
            case 1:
                setupOutputByMapProjectionLambert2();
                m_pOTBOrthoUtility->InitializeInputLambertIITransform();
                break;
            case 2:
                setupOutputByMapProjectionWGS84();
                break;
            case 3:
                setupOutputByMapProjectionEPSG();
                m_pOTBOrthoUtility->UpdateInputEPSGTransform(m_pOutputRSIDLineEdit->text().toInt());
                break;
            case 4:
                setupOutputByMapProjectionTM();
                m_pOTBOrthoUtility->UpdateInputTMTransform(m_pOutputTMScaleDoubleSpinBox->value(), m_pOutputEastingDoubleSpinBox->value(), m_pOutputNorthingDoubleSpinBox->value());
                break;
            default:
                break;
        }

    }else{
        switch(idx){
            case 0:
                setupOutputByMapProjectionUTM();
                m_pOTBOrthoUtility->UpdateUTMTransform(m_pOutputZoneSpinBox->value(),m_pOutputNorthHemisphereRadioButton->isChecked());
                break;
            case 1:
                setupOutputByMapProjectionLambert2();
                break;
            case 2:
                setupOutputByMapProjectionWGS84();
                m_pOTBOrthoUtility->UpdateWGS84Transform();
                break;
            case 3:
                setupOutputByMapProjectionEPSG();
                m_pOTBOrthoUtility->updateEPSGTransform(m_pOutputRSIDLineEdit->text().toInt());
                break;
            case 4:
                setupOutputByMapProjectionTM();
                m_pOTBOrthoUtility->UpdateTMTransform(m_pOutputTMScaleDoubleSpinBox->value(), m_pOutputEastingDoubleSpinBox->value(), m_pOutputNorthingDoubleSpinBox->value());
                break;
            default:
                break;
        }
    }
}


void QOtbOrthoRectifyWidget::setupUserDefinedParametersAsOutputOrigin(){
    //! do sth here
}


void QOtbOrthoRectifyWidget::setupUpperLeftAsOutputOrgin(){
    //! do sth here
}


void QOtbOrthoRectifyWidget::setupCenterAsOutputOrigin(){
    //! do sth here
}


void QOtbOrthoRectifyWidget::setEnabledOCCGroupBoxWidgets(bool enabled){
    m_pOutputOrgCartoCoordGroupBox->setEnabled(enabled);
}


void QOtbOrthoRectifyWidget::onOriginCartoCurrentSelectionChanged(int idx){
    if(idx == m_currentOCCMethod)
        return;

    m_currentOCCMethod = idx;

    qDebug() << "on origin carto current selection changed , current button id clicked : " << idx;

    //! do sth for the index chosen

    switch (idx){
        case 1:
            setEnabledOCCGroupBoxWidgets(true);
            setupUserDefinedParametersAsOutputOrigin();
            break;
        case 2:
            setEnabledOCCGroupBoxWidgets(false);
            setupUpperLeftAsOutputOrgin();
            break;
        case 3:
            setEnabledOCCGroupBoxWidgets(false);
            setupCenterAsOutputOrigin();
            break;
        default:
            break;
    }
}


void QOtbOrthoRectifyWidget::onInterpolatorChanged(int idx){
    if(idx == 2){ // then it is BCO
        m_pSettingsBCOLabel->setVisible(true);
        m_pSettingsBCOSpinBox->setVisible(true);
    }else{
        m_pSettingsBCOLabel->setVisible(false);
        m_pSettingsBCOSpinBox->setVisible(false);
    }

    m_pOTBOrthoUtility->setInterpolator((OTBOrthoUtility::INTERPOLATOR)idx);
}


void QOtbOrthoRectifyWidget::onDEMButtonGroupChanged(int idx){
    if(idx == 1) { // then it a use dem
        m_pSettingsDemFilePathLabel->setEnabled(true);
        m_pSettingsDemFilePushButton->setEnabled(true);
        m_pSettingsElevationValueLabel->setEnabled(false);
        m_pSettingsElevationValueSpinBox->setEnabled(false);
    }else{
        m_pSettingsDemFilePathLabel->setEnabled(false);
        m_pSettingsDemFilePushButton->setEnabled(false);
        m_pSettingsElevationValueLabel->setEnabled(true);
        m_pSettingsElevationValueSpinBox->setEnabled(true);
    }
}


void QOtbOrthoRectifyWidget::onDemFilePathButtonClicked(){
    QString path = QFileDialog::getOpenFileName(this,tr("Open Dem file"),QDir::homePath());
    if(path.isEmpty())
        return;

    m_pSettingsDemFilePathLabel->setText(path);
}


void QOtbOrthoRectifyWidget::onNonValidEpsgCode(){
    QMessageBox::warning(this,tr("Error"),tr("Non valid EPSG code selected. Please input a valid EPSG code and try again."));
}


void QOtbOrthoRectifyWidget::UpdateOutputRegion(){

    // tell the model to project the region edited in the GUI
    unsigned int sizeX = m_pOutputSizeXSpinBox->value();
    unsigned int sizeY = m_pOutputSizeYSpinBox->value();
    double spacingX = m_pOutputSpacingXDoubleSpinBox->value();
    double spacingY = m_pOutputSpacingYDoubleSpinBox->value();
    double originX = m_pOutputLngDoubleSpinBox->value();
    double originY = m_pOutputLatDoubleSpinBox->value();

    m_pOTBOrthoUtility->projectRegion(sizeX, sizeY, spacingX, spacingY, originX, originY);
}


// Used to update the geographic origin of the output
// image when changing the cartographic coordinates
void QOtbOrthoRectifyWidget::UpdateOriginGeographicalCoordinates()
{

    ModelType::OutputPointType cartoPoint;
    ModelType::OutputPointType geoPoint;

    // Get the Transform from the model in order to project the new
    // carto point in WGS84 and update the Long/Lat fields
    TransformType::Pointer rsTransform = m_pOTBOrthoUtility->inverseTransform();

    itk::OStringStream oss;
    oss << setiosflags(std::ios_base::fixed);
    oss.str("");

    int currentMPIndex = m_pOutputMapProjectionComboBox->currentIndex();

    switch (currentMPIndex)
    {
    case (int)OTBOrthoUtility::MP_UTM:
    {
        cartoPoint[1] = m_pOutputNorthingDoubleSpinBox->value();
        cartoPoint[0] = m_pOutputEastingDoubleSpinBox->value();
        break;
    }
    case (int)OTBOrthoUtility::MP_LAMBERT2:
    {
        cartoPoint[1] = m_pOutputNorthingDoubleSpinBox->value();
        cartoPoint[0] = m_pOutputEastingDoubleSpinBox->value();
        break;
    }
    case (int)OTBOrthoUtility::MP_WGS84:
    {
        cartoPoint[1] = m_pOutputNorthingDoubleSpinBox->value();
        cartoPoint[0] = m_pOutputEastingDoubleSpinBox->value();
        break;
    }
    case (int)OTBOrthoUtility::MP_EPSG:
    {
        // Since no WGS84 param are available in the GUI,
        //
        cartoPoint[1] = m_pOutputLatDoubleSpinBox->value();
        cartoPoint[0] = m_pOutputLngDoubleSpinBox->value();
        break;
    }
    case (int)OTBOrthoUtility::MP_TM:
    {
        cartoPoint[1] = m_pOutputNorthingDoubleSpinBox->value();
        cartoPoint[0] = m_pOutputEastingDoubleSpinBox->value();
        break;
    }
    default:
        QMessageBox::warning(this,tr("Projection Problem"),tr("Problem with map projection type, please contact developpers"));
        break;
    }

  // Project the carto point in WGS84 using the transform set in
  // the model
  geoPoint = rsTransform->GetTransform()->GetFirstTransform()->TransformPoint(cartoPoint);

  //
  m_pOutputLatDoubleSpinBox->setValue(geoPoint[1]);
  m_pOutputLngDoubleSpinBox->setValue(geoPoint[0]);

  // Update output region
  // tell the model that the output region has been modified
  unsigned int sizeX = m_pOutputSizeXSpinBox->value();
  unsigned int sizeY = m_pOutputSizeYSpinBox->value();
  double spacingX = m_pOutputSpacingXDoubleSpinBox->value();
  double spacingY = m_pOutputSpacingYDoubleSpinBox->value();

  m_pOTBOrthoUtility->projectRegion(sizeX, sizeY, spacingX, spacingY, geoPoint[0], geoPoint[1]);
}

/**
 * Guess what is the Projection of the Origin if any
 */
void QOtbOrthoRectifyWidget::RetrieveInputProjection()
{
    VectorImageType::Pointer vImgType = m_pOTBOrthoUtility->inputImage();
    if(vImgType.IsNull())
        return;

    // The discriminative parameter to distinguish UTM and TransverseMercator
    double scale_factor = m_pOutputTMScaleDoubleSpinBox->value();

    // Get the projection Ref from the input Image
    //  std::string inputProjRef = m_Controller->GetModel()->GetInputImage()->GetProjectionRef();
    std::string inputProjRef = m_pOTBOrthoUtility->projectionRef();

    // If the Projection is not empty: Map Projection
    OGRSpatialReference oSRS;

    // Else perhaps a model sensor
    if (!inputProjRef.empty())
    {
        // From std::string to char*: Needed for the importFromWkt(char** projRef)
        char * inputProjchar = new char[inputProjRef.length() + 1];
        strcpy(inputProjchar, inputProjRef.c_str());

        // Import OGRSpatial Reference object from projectionRef
        // Warning: importFromWkt modifies the passed pointer !
        char* inputProjcharForImport = inputProjchar;
        oSRS.importFromWkt(&inputProjcharForImport);

        // First test if the node Projection exists: Avoid Segfaults when
        // uses strcmp with null values.
        if (oSRS.GetAttrValue("PROJECTION") != NULL)
        {
            // Get the value of the node PROJECTION
            const char * inputMap = oSRS.GetAttrValue("PROJECTION");

            // Test the different combination we want
            if (strcmp(inputMap, "Lambert_Conformal_Conic_2SP") == 0)
            {
                // Fill the GUI With the  LAMBERT 2 parameters
              setupOutputByMapProjectionLambert2();
            }
            else if (strcmp(inputMap, "Transverse_Mercator") == 0)
            {
                // Get the value of the parameter scale_factor
                if (this->FindParameter(oSRS, "scale_factor", &scale_factor))
                {

                    if (scale_factor == 0.9996 /** Value Specific to UTM */)
                    {
                        setupOutputByMapProjectionUTM();

                        // Get the number of the utm zone
                        int north = 0;
                        int zone = oSRS.GetUTMZone(&north);

                        // Fill the UTM Parameters in the GUI
                        itk::OStringStream oss;
                        oss << zone;
                        m_pOutputZoneSpinBox->setValue(zone);

                        if (north)
                        {
                            m_pOutputNorthHemisphereRadioButton->setChecked(true);
                        }
                        else
                        {
                            m_pOutputSouthHemisphereRadioButton->setChecked(true);
                        }
                    }
                    else
                    {
                        //Fill the TransverseMercator Parameters
                        setupOutputByMapProjectionTM();
                    }
                }
            }

            delete inputProjchar;
        }
    }

    // Sensor Model:: Test if KeyWorldList is not empty
    if (vImgType->GetImageKeywordlist().GetSize() > 0 && inputProjRef.empty())
    {
    // Select the Sensor Model in the InputMapType
        setupOutputByMapProjectionWGS84();
        m_pOutputMapProjectionComboBox->setCurrentIndex((int)OTBOrthoUtility::MP_WGS84);
    }

    // Default Case: Kwl & ProjRef  empty: Give the user the possibility to fill the proj he needs
    if (vImgType->GetImageKeywordlist().GetSize() == 0 && inputProjRef.empty())
    {
        m_InputProjectionUnknown = true;
    }
}


/**
 * In the OGR_SRSNode: Find the value of the parameter inParam if exists
 *
 */
bool QOtbOrthoRectifyWidget::FindParameter(OGRSpatialReference oSRS, const char * inParam, double * paramValue)
{
  // Get all the nodes of the Projection Ref Tree
  // and search for a child named: "scale_factor"
  OGR_SRSNode* node = oSRS.GetAttrNode("PROJCS");

  int nbChild = node->GetChildCount();
  for (int i = 0; i < nbChild; i++)
    {
    OGR_SRSNode* curChild = node->GetChild(i);
    int res = curChild->FindChild(inParam);
    if (res == 0)
      {
      // scale_factor parameter found
      *paramValue = strtod(curChild->GetChild(1)->GetValue(), NULL);
      return true;
      }
    }
  return false;
}


void QOtbOrthoRectifyWidget::InitializeAction()
{
    PointType middlePoint, geoPoint;

    // Get the transform from the model
    TransformType::Pointer rsTransform = m_pOTBOrthoUtility->inverseTransform();

    middlePoint[0] = m_pOTBOrthoUtility->wholeOutputOrigin()[0] +
      m_pOTBOrthoUtility->wholeOutputSize()[0] * m_pOTBOrthoUtility->wholeOutputSpacing()[0] / 2;

    middlePoint[1] = m_pOTBOrthoUtility->wholeOutputOrigin()[1] +
      m_pOTBOrthoUtility->wholeOutputSize()[1] * m_pOTBOrthoUtility->wholeOutputSpacing()[1] / 2;

    // Get the UL corner cartographic coordinates of the
    // region centered on the whole output middlePoint

    middlePoint[0] -= m_pOutputSizeXSpinBox->value() * m_pOutputSpacingXDoubleSpinBox->value()/2;
    middlePoint[1] -= m_pOutputSizeYSpinBox->value() * m_pOutputSpacingYDoubleSpinBox->value()/2;

    // Transform to geo projection
    geoPoint = rsTransform->GetTransform()->GetFirstTransform()->TransformPoint(middlePoint);

    m_pOutputLatDoubleSpinBox->setValue(geoPoint[1]);
    m_pOutputLngDoubleSpinBox->setValue(geoPoint[0]);

    // Update map parameters if inputTransform known
    if (!m_InputProjectionUnknown)
    {
        // Get the utm Zone && hemisphere  from a long, lat point in the image
        UtmForwardProjection::Pointer utmProjection = UtmForwardProjection::New();

        int zone = Utils::GetZoneFromGeoPoint(geoPoint[0], geoPoint[1]);

        m_pOutputZoneSpinBox->setValue(zone);

        if (geoPoint[1] >= 0)
        {
            m_pOutputNorthHemisphereRadioButton->setChecked(true);
        }
        else
        {
            m_pOutputSouthHemisphereRadioButton->setChecked(true);
        }

        UpdateUTMTransform();
        // End of filling the utm parameters
    }

  // Update the output parameters
    UpdateOutputParameters();
}

/**
 *
 */
void QOtbOrthoRectifyWidget::UpdateTMTransform()
{
    m_pOTBOrthoUtility->UpdateTMTransform(m_pOutputTMScaleDoubleSpinBox->value(),m_pOutputEastingDoubleSpinBox->value(),m_pOutputNorthingDoubleSpinBox->value());
}


void QOtbOrthoRectifyWidget::UpdateUTMTransform()
{

  //Get the projection parameters
  int utmZone = m_pOutputZoneSpinBox->value();
  bool north = false;
  if(m_pOutputNorthHemisphereRadioButton->isChecked())
      north = true;

  // Update the transform
  m_pOTBOrthoUtility->UpdateUTMTransform(utmZone, north);
}


/**
 * Tell the model to compute the LambertII transform
 * Done in the module cause the user is not allowed to modify this
 */
void QOtbOrthoRectifyWidget::UpdateWGS84Transform()
{

  m_pOTBOrthoUtility->UpdateWGS84Transform();
}


/**
 *
 */
void QOtbOrthoRectifyWidget::UpdateEPSGTransform()
{

  //Get the RSID number
  m_pOTBOrthoUtility->updateEPSGTransform(m_pOutputRSIDLineEdit->text().toInt());
}


void QOtbOrthoRectifyWidget::UpdateInputUTMTransform()
{

    // Update the transform
    m_pOTBOrthoUtility->UpdateInputUTMTransform(m_pOutputZoneSpinBox->value(), m_pOutputNorthHemisphereRadioButton->isChecked());

    // Update the whole transformation
    UpToDateTransform();
}

/**
 * Tell the model to compute the LambertII transform
 * Done in the module cause the user is not allowed to modify this
 */
void QOtbOrthoRectifyWidget::UpdateInputLambertIITransform()
{

    m_pOTBOrthoUtility->InitializeInputLambertIITransform();

    // Update the whole transformation
    UpToDateTransform();
}

/**
 *
 */
void QOtbOrthoRectifyWidget::UpdateInputTMTransform()
{
    m_pOTBOrthoUtility->UpdateInputTMTransform(m_pOutputTMScaleDoubleSpinBox->value(),m_pOutputEastingDoubleSpinBox->value(),m_pOutputNorthingDoubleSpinBox->value());

    // Update the whole transformation
    this->UpToDateTransform();
}

/**
 *
 */
void QOtbOrthoRectifyWidget::UpdateInputEPSGTransform()
{

    //Get the RSID number
    m_pOTBOrthoUtility->UpdateInputEPSGTransform(m_pOutputRSIDLineEdit->text().toInt());

    // Update the whole transformation
    UpToDateTransform();

}


/**
 *
 */
void QOtbOrthoRectifyWidget::UpdateMapParam()
{

    ModelType::OutputPointType geoPoint, newCartoPoint;
    // Get the new values of Long/Lat from the gui
    geoPoint[0] = m_pOutputLngDoubleSpinBox->value();
    geoPoint[1] = m_pOutputLatDoubleSpinBox->value();

    // Project the new geo Point
    TransformType::Pointer rsTransform = m_pOTBOrthoUtility->transform();

    int currentMPIndex = m_pOutputMapProjectionComboBox->currentIndex();

    switch (currentMPIndex)
    {
    case (int)OTBOrthoUtility::MP_UTM:
    {
        newCartoPoint = rsTransform->GetTransform()->GetSecondTransform()->TransformPoint(geoPoint);

        m_pOutputNorthingDoubleSpinBox->setValue(newCartoPoint[1]);
        m_pOutputEastingDoubleSpinBox->setValue(newCartoPoint[0]);
        break;
    }
    case (int)OTBOrthoUtility::MP_LAMBERT2:
    {
        newCartoPoint = rsTransform->GetTransform()->GetSecondTransform()->TransformPoint(geoPoint);
        m_pOutputNorthingDoubleSpinBox->setValue(newCartoPoint[1]);
        m_pOutputEastingDoubleSpinBox->setValue(newCartoPoint[0]);
        break;
    }
    case (int)OTBOrthoUtility::MP_TM:
    {
        newCartoPoint = rsTransform->GetTransform()->GetSecondTransform()->TransformPoint(geoPoint);
        m_pOutputNorthingDoubleSpinBox->setValue(newCartoPoint[1]);
        m_pOutputEastingDoubleSpinBox->setValue(newCartoPoint[0]);
        break;
    }
    case (int)OTBOrthoUtility::MP_WGS84:
    {
        break;
    }
    case (int)OTBOrthoUtility::MP_EPSG:
    {
        newCartoPoint = rsTransform->GetTransform()->GetSecondTransform()->TransformPoint(geoPoint);
        m_pOutputNorthingDoubleSpinBox->setValue(newCartoPoint[1]);
        m_pOutputEastingDoubleSpinBox->setValue(newCartoPoint[0]);
        break;
    }
    default:
        QMessageBox::warning(this,tr("Projection error"),tr("Problem with map projection type, please contact developpers"));
        break;
    }

  // Tell the model that the output region has changed since the
  // output geographical coordinates changes the cartographic orign in
  // the  user defined mode
//    UpdateOutputRegion();
}

/**
 *
 */

void QOtbOrthoRectifyWidget::UpdateOutputParameters()
{
    m_pOutputSpacingXDoubleSpinBox->setValue(m_pOTBOrthoUtility->outputSpacing()[0]);
    m_pOutputSpacingYDoubleSpinBox->setValue(m_pOTBOrthoUtility->outputSpacing()[1]);
    m_pOutputSizeXSpinBox->setValue(m_pOTBOrthoUtility->outputSize()[0]);
    m_pOutputSizeYSpinBox->setValue(m_pOTBOrthoUtility->outputSize()[1]);
}


/**
 *
 */
void QOtbOrthoRectifyWidget::UpdateLongLat()
{

    if (!m_InputProjectionUnknown)
    {
        PointType point, geoPoint;

        // Get the transform from the model
        TransformType::Pointer rsTransform = m_pOTBOrthoUtility->inverseTransform();

        // Do that if the Mode User Definition is not set
        if (!m_pOutputUpperLeftRadionButton->isChecked())
        {
            // Set the index to be the center of the ROI selected by the user
            if (m_pOutputCenteredRadionButton->isChecked())
            {
                // Get the cartographic middle point
                point[0] = m_pOTBOrthoUtility->wholeOutputOrigin()[0] +
                    m_pOTBOrthoUtility->wholeOutputSize()[0] * m_pOTBOrthoUtility->wholeOutputSpacing()[0]
                    / 2;

                point[1] = m_pOTBOrthoUtility->wholeOutputOrigin()[1] +
                    m_pOTBOrthoUtility->wholeOutputSize()[1] * m_pOTBOrthoUtility->wholeOutputSpacing()[1]
                    / 2;

                // Get the cartographic coordinates of the UL point of the
                // region centered on the output middle point
                point[0] -= m_pOutputSizeXSpinBox->value() * m_pOutputSpacingXDoubleSpinBox->value() / 2.;
                point[1] -= m_pOutputSizeYSpinBox->value() * m_pOutputSpacingYDoubleSpinBox->value() / 2.;
            }
            else {
                // Get the output Cartographic origin
                point[0] = m_pOTBOrthoUtility->wholeOutputOrigin()[0];
                point[1] = m_pOTBOrthoUtility->wholeOutputOrigin()[1];
            }

            // Transform to geo
            geoPoint = rsTransform->GetTransform()->GetFirstTransform()->TransformPoint(point);

            // Fill the datas in the GUI
            m_pOutputLngDoubleSpinBox->setValue(geoPoint[0]);

            m_pOutputLatDoubleSpinBox->setValue(geoPoint[1]);

            //Update the Map parameters
            UpdateMapParam();
        }
        else
        {
            UpdateOriginGeographicalCoordinates();
        }
    }

    // Common to both modes
    // Update the output origin in the model
//    UpdateOutputRegion();
}


void QOtbOrthoRectifyWidget::updateOnTransformChange(){
    // Update the cartographic parameters
    UpdateMapParam();
    // Update the output
    UpdateOutputParameters();
    // Update output whole origin image long/lat
    UpdateLongLat();
    // Update the origin cartographic coordinates
    UpdateOriginGeographicalCoordinates();
    // Update the output region
    UpdateOutputRegion();
}

/**
 * Update the output Information and the second transform
 */
void QOtbOrthoRectifyWidget::UpToDateTransform()
{
    int currentMpIndex = m_pOutputMapProjectionComboBox->currentIndex();

    switch (currentMpIndex)
    {
    case (int)OTBOrthoUtility::MP_UTM:
    {
        this->UpdateUTMTransform();
        break;
    }
    case (int)OTBOrthoUtility::MP_LAMBERT2:
    {
//        this->UpdateLambertIITransform();
        break;
    }
    case (int)OTBOrthoUtility::MP_TM:
    {
        this->UpdateTMTransform();
        break;
    }
    case (int)OTBOrthoUtility::MP_EPSG:
    {
        m_pOTBOrthoUtility->updateEPSGTransform(m_pOutputRSIDLineEdit->text().toInt());
        break;
    }
    default:
        break;
    }
}

void QOtbOrthoRectifyWidget::onRPCModelCheckBoxChanged(bool val){
    if(!val)
        return;

    QString path  = QFileDialog::getOpenFileName(this,tr("Open"),QDir::homePath());

    if(!path.isEmpty()){
        m_pOTBOrthoUtility->updateRPCModel(path);
    }
}


QOtbOrthoRectifyWidget::~QOtbOrthoRectifyWidget(){

}

} //namespace voreen

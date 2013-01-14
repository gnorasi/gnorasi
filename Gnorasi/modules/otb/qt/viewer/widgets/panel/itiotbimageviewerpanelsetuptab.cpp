#include "itiotbimageviewerpanelsetuptab.h"
#include "itiotbimageviewerpanel.h"
#include "../../utils/itiotbimagechannelprovider.h"

#include "../../vector_globaldefs.h"

#include "../../utils/itiotbimagemanager.h"

#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbImageViewerPanelSetupTab::ItiOtbImageViewerPanelSetupTab(ItiOtbImageViewerPanel *panel, QWidget *parent) :
    m_pItiOtbImageViewerPanel(panel), QWidget(parent)
{
    setWindowTitle(tr("Setup"));

    initialize();
}


//!
void ItiOtbImageViewerPanelSetupTab::setupColorCompositionGroupBox(){

    //!
    m_pGroupBoxColorComposition         = new QGroupBox(this);
    m_pRadioButtonGrayscaleMode         = new QRadioButton(this);
    m_pRadioButtonRGBCompositionMode    = new QRadioButton(this);
    m_pLabelGreyChannel                 = new QLabel(tr("Grey channel"),this);
    m_pSpinBoxRedChannel                = new QSpinBox(this);
    m_pSpinBoxGreenChannel              = new QSpinBox(this);
    m_pSpinBoxBlueChannel               = new QSpinBox(this);
    m_pSpinBoxGreyscaleChannel          = new QSpinBox(this);
    m_pLabelRedChannel                  = new QLabel(tr("Red channel"),this);
    m_pLabelGreenChannel                = new QLabel(tr("Green channel"),this);
    m_pLabelBlueChannel                 = new QLabel(tr("Blue channel"),this);
    m_pButtonApplyColorComposition      = new QPushButton(this);

    //!
    m_pGroupBoxColorComposition->setTitle(tr("Color Composition"));
    m_pRadioButtonGrayscaleMode->setText(tr("Greyscale mode"));
    m_pRadioButtonRGBCompositionMode->setText(tr("RGB composition mode"));
    m_pButtonApplyColorComposition->setText(tr("Apply"));
    m_pLabelRedChannel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelGreenChannel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelBlueChannel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelGreyChannel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pSpinBoxBlueChannel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSpinBoxGreenChannel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSpinBoxGreyscaleChannel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSpinBoxRedChannel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pSpinBoxRedChannel->setValue(1);
    m_pSpinBoxGreenChannel->setValue(2);
    m_pSpinBoxBlueChannel->setValue(3);

    //!
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_pLabelGreyChannel);
    hboxLayout->addWidget(m_pSpinBoxGreyscaleChannel);

    //!
    QHBoxLayout *hboxLayout1 = new QHBoxLayout;
    hboxLayout1->addWidget(m_pLabelRedChannel);
    hboxLayout1->addWidget(m_pSpinBoxRedChannel);

    QHBoxLayout *hboxLayout2 = new QHBoxLayout;
    hboxLayout2->addWidget(m_pLabelGreenChannel);
    hboxLayout2->addWidget(m_pSpinBoxGreenChannel);

    //!
    QHBoxLayout *hboxLayout3 = new QHBoxLayout;
    hboxLayout3->addWidget(m_pLabelBlueChannel);
    hboxLayout3->addWidget(m_pSpinBoxBlueChannel);


    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_pRadioButtonGrayscaleMode);
    layout->addLayout(hboxLayout);
    layout->addWidget(m_pRadioButtonRGBCompositionMode);
    layout->addLayout(hboxLayout1);
    layout->addLayout(hboxLayout2);
    layout->addLayout(hboxLayout3);
    layout->addSpacerItem(new QSpacerItem(100,100,QSizePolicy::Fixed,QSizePolicy::Expanding));
    layout->addWidget(m_pButtonApplyColorComposition);

    m_pGroupBoxColorComposition->setLayout(layout);

    connect(m_pRadioButtonGrayscaleMode,SIGNAL(clicked()),this,SLOT(onRadioButtonColorCompositionChanged()));
    connect(m_pRadioButtonRGBCompositionMode,SIGNAL(clicked()),this,SLOT(onRadioButtonColorCompositionChanged()));
    connect(m_pButtonApplyColorComposition,SIGNAL(clicked()),this,SLOT(onColorCompositionApplyButtonClicked()));

    m_pRadioButtonRGBCompositionMode->toggle();
    onRadioButtonColorCompositionChanged();
}

//!
void ItiOtbImageViewerPanelSetupTab::setupContrastEnhancememtGroupBox(){
    //!
    m_pGroupBoxContrastEnhancement      = new QGroupBox(this);
    m_pGroupBoxMethod                   = new QGroupBox(this);
    m_pComboBoxMethod                   = new QComboBox(this);
    m_pLabelLowerQuantile               = new QLabel(tr("Lower Quantile %"),this);
    m_pLabelUpperQuantile               = new QLabel(tr("Upper Quantile %"),this);
    m_pLabelStandardDeviation           = new QLabel(tr("Standard Deviation"),this);
    m_pLineEditLowerQuantileValue       = new QLineEdit(this);
    m_pLineEditStandardDeviationValue   = new QLineEdit(this);
    m_pLineEditUpperQuantileValue       = new QLineEdit(this);
    m_pButtonApplyContrastEnhancement   = new QPushButton(this);

    //!
    m_pGroupBoxContrastEnhancement->setTitle(tr("Contrast Enhancement"));
    m_pGroupBoxMethod->setTitle(tr("Method"));
    QStringList list;
    list << tr("Linear 0-255") << tr("Linear X%") << tr("Gaussian") << tr("Square roor");
    m_pComboBoxMethod->insertItems(0,list);
    m_pLineEditLowerQuantileValue->setText(QString::number(2.0,'f',1));
    m_pLineEditUpperQuantileValue->setText(QString::number(2.0,'f',1));
    m_pLineEditLowerQuantileValue->setValidator(new QDoubleValidator(0.0,1000.0,1,this));
    m_pLineEditUpperQuantileValue->setValidator(new QDoubleValidator(0.0,1000.0,1,this));
    m_pLineEditStandardDeviationValue->setText(QString::number(1.0,'f',2));
    m_pButtonApplyContrastEnhancement->setText(tr("Apply"));
    m_pLabelStandardDeviation->setVisible(false);
    m_pLineEditStandardDeviationValue->setVisible(false);

    //!
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_pLabelLowerQuantile);
    hboxLayout->addWidget(m_pLineEditLowerQuantileValue);

    //!
    QHBoxLayout *hboxLayout1 = new QHBoxLayout;
    hboxLayout1->addWidget(m_pLabelUpperQuantile);
    hboxLayout1->addWidget(m_pLineEditUpperQuantileValue);

    //!
    QHBoxLayout *hboxLayout2 = new QHBoxLayout;
    hboxLayout2->addWidget(m_pLabelStandardDeviation);
    hboxLayout2->addWidget(m_pLineEditStandardDeviationValue);

    //!
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(m_pComboBoxMethod);
    vBoxLayout->addLayout(hboxLayout);
    vBoxLayout->addLayout(hboxLayout1);
    vBoxLayout->addLayout(hboxLayout2);
    m_pGroupBoxMethod->setLayout(vBoxLayout);

    //!
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_pGroupBoxMethod);
    layout->addSpacerItem(new QSpacerItem(100,100,QSizePolicy::Preferred,QSizePolicy::Expanding));
    layout->addWidget(m_pButtonApplyContrastEnhancement);

    m_pGroupBoxContrastEnhancement->setLayout(layout);

    connect(m_pComboBoxMethod,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxContrastEnhancementMethodCurrentIndexChanged(int)));
    connect(m_pButtonApplyContrastEnhancement,SIGNAL(clicked()),this,SLOT(onContrastEnhancementApplyButtonClicked()));
}

//!
void ItiOtbImageViewerPanelSetupTab::initialize(){
    setupColorCompositionGroupBox();

    setupContrastEnhancememtGroupBox();

    QHBoxLayout *hboxLayout = new QHBoxLayout(this);
    hboxLayout->addWidget(m_pGroupBoxColorComposition);
    hboxLayout->addWidget(m_pGroupBoxContrastEnhancement);
    hboxLayout->addSpacerItem(new QSpacerItem(100,100,QSizePolicy::Expanding,QSizePolicy::Fixed));
    setLayout(hboxLayout);

    m_pRadioButtonGrayscaleMode->toggle();
    onRadioButtonColorCompositionChanged();
}

//!
void ItiOtbImageViewerPanelSetupTab::onSpinBoxRedChannelChanged(int val){
    Q_UNUSED(val)

    //!
    emit rgbColorCompositionChannelsChanged(m_pSpinBoxRedChannel->value(),m_pSpinBoxGreenChannel->value(),m_pSpinBoxBlueChannel->value());
}

//!
void ItiOtbImageViewerPanelSetupTab::onSpinBoxGreenChannelChanged(int val){
    Q_UNUSED(val)

    //!
    emit rgbColorCompositionChannelsChanged(m_pSpinBoxRedChannel->value(),m_pSpinBoxGreenChannel->value(),m_pSpinBoxBlueChannel->value());
}

//!
void ItiOtbImageViewerPanelSetupTab::onSpinBoxBlueChannelChanged(int val){
    Q_UNUSED(val)

    //!
    emit rgbColorCompositionChannelsChanged(m_pSpinBoxRedChannel->value(),m_pSpinBoxGreenChannel->value(),m_pSpinBoxBlueChannel->value());
}

//!
void ItiOtbImageViewerPanelSetupTab::onComboBoxContrastEnhancementMethodCurrentIndexChanged(int index){
    if(index == 2){
        m_pLabelLowerQuantile->setVisible(false);
        m_pLabelUpperQuantile->setVisible(false);
        m_pLabelStandardDeviation->setVisible(true);
        m_pLineEditLowerQuantileValue->setVisible(false);
        m_pLineEditUpperQuantileValue->setVisible(false);
        m_pLineEditStandardDeviationValue->setVisible(true);

        emit contrastEnhancementChanged(index,m_pLineEditStandardDeviationValue->text().toDouble(),0.0);
    }else{
        m_pLabelLowerQuantile->setVisible(true);
        m_pLabelUpperQuantile->setVisible(true);
        m_pLabelStandardDeviation->setVisible(false);
        m_pLineEditLowerQuantileValue->setVisible(true);
        m_pLineEditUpperQuantileValue->setVisible(true);
        m_pLineEditStandardDeviationValue->setVisible(false);

        emit contrastEnhancementChanged(index,m_pLineEditLowerQuantileValue->text().toDouble(),m_pLineEditUpperQuantileValue->text().toDouble());
    }
}

//!
void ItiOtbImageViewerPanelSetupTab::onRadioButtonColorCompositionChanged(){
    //!
    if(m_pRadioButtonGrayscaleMode->isChecked()){
        m_pLabelGreyChannel->setEnabled(true);
        m_pSpinBoxGreyscaleChannel->setEnabled(true);
        m_pLabelRedChannel->setEnabled(false);
        m_pLabelGreenChannel->setEnabled(false);
        m_pLabelBlueChannel->setEnabled(false);
        m_pSpinBoxRedChannel->setEnabled(false);
        m_pSpinBoxGreenChannel->setEnabled(false);
        m_pSpinBoxBlueChannel->setEnabled(false);

        //!
        emit greyScaleColorCompositionChannelChanged(m_pSpinBoxGreyscaleChannel->value());

    }else{
        m_pLabelGreyChannel->setEnabled(false);
        m_pSpinBoxGreyscaleChannel->setEnabled(false);
        m_pLabelRedChannel->setEnabled(true);
        m_pLabelGreenChannel->setEnabled(true);
        m_pLabelBlueChannel->setEnabled(true);
        m_pSpinBoxRedChannel->setEnabled(true);
        m_pSpinBoxGreenChannel->setEnabled(true);
        m_pSpinBoxBlueChannel->setEnabled(true);

        emit rgbColorCompositionChannelsChanged(m_pSpinBoxRedChannel->value(),m_pSpinBoxGreenChannel->value(),m_pSpinBoxBlueChannel->value());
    }
}

void ItiOtbImageViewerPanelSetupTab::onLowerQuantileLineEditChanged(const QString &text){
    double val = text.toDouble();

    emit lowerQuantileChanged(val);
}


void ItiOtbImageViewerPanelSetupTab::onStandardDeviationLineEdiitChanged(const QString &text){
    double val = text.toDouble();

    emit deviationChanged(val);
}


void ItiOtbImageViewerPanelSetupTab::onUpperQuantileLineEditChanged(const QString &text){
    double val = text.toDouble();

    emit upperQuantileChanged(val);
}

void ItiOtbImageViewerPanelSetupTab::setupChannels(){
    ItiOtbImageChannelProvider *provider = m_pItiOtbImageViewerPanel->provider();

    if(!provider)
        return;

    QList<int> list = provider->channels();

    if(list.size()<3)
        return;

    m_pSpinBoxGreyscaleChannel->setValue(list.at(0)+1);
    m_pSpinBoxRedChannel->setValue(list.at(0)+1);
    m_pSpinBoxGreenChannel->setValue(list.at(1)+1);
    m_pSpinBoxBlueChannel->setValue(list.at(2)+1);
}


void ItiOtbImageViewerPanelSetupTab::onContrastEnhancementApplyButtonClicked(){
    int index = m_pComboBoxMethod->currentIndex();

    switch(index){
    case 0:
        emit applyContrastEnhancementLinear0255();
        break;
    case 1:
        emit applyContrastEnhancementLinearXPerc();
        break;
    case 2:
        emit applyContrastEnhancementGaussian();
        break;
    case 3:
        break;
        emit applyContrastEnhancementSquareRoot();
    default:
        break;
    }
}

void ItiOtbImageViewerPanelSetupTab::onColorCompositionApplyButtonClicked(){

    if(m_pRadioButtonGrayscaleMode->isChecked()) {
        emit applyColorCompositionGreyscale();
    }else {
        emit applyColorCompositionRGB();
    }

}

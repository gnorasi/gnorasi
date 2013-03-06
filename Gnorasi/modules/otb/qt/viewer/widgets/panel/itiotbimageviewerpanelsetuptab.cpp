#include "itiotbimageviewerpanelsetuptab.h"
#include "itiotbimageviewerpanel.h"
#include "../../utils/itiotbimagechannelprovider.h"

#include "../../vector_globaldefs.h"

#include "../../utils/itiotbimagemanager.h"
#include "../../utils/itiotbregion.h"
#include "../../utils/itiotblevel.h"

#include <QVBoxLayout>
#include <QHash>


using namespace itiviewer;

ItiOtbImageViewerPanelSetupTab::ItiOtbImageViewerPanelSetupTab(ItiOtbImageViewerPanel *panel) :
    m_pItiOtbImageViewerPanel(panel)
{
    setWindowTitle(tr("Setup"));

    m_isGreyscale = false;

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

    m_pRadioButtonRGBCompositionMode->toggle();
    onRadioButtonColorCompositionChanged();

    connect(m_pRadioButtonGrayscaleMode,SIGNAL(clicked()),this,SLOT(onRadioButtonColorCompositionChanged()));
    connect(m_pRadioButtonRGBCompositionMode,SIGNAL(clicked()),this,SLOT(onRadioButtonColorCompositionChanged()));
    connect(m_pButtonApplyColorComposition,SIGNAL(clicked()),this,SLOT(onColorCompositionApplyButtonClicked()));
    connect(m_pSpinBoxRedChannel,SIGNAL(valueChanged(int)),this,SLOT(onChannelValueChanged(int)));
    connect(m_pSpinBoxGreenChannel,SIGNAL(valueChanged(int)),this,SLOT(onChannelValueChanged(int)));
    connect(m_pSpinBoxBlueChannel,SIGNAL(valueChanged(int)),this,SLOT(onChannelValueChanged(int)));
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

void ItiOtbImageViewerPanelSetupTab::setupClassficationGroupBox(){
    //
    m_pGroupBoxClassification = new QGroupBox(this);
    m_pGroupBoxClassification->setTitle(tr("Classification"));
    m_pTreeViewClassfication = new QTreeView(this);
    m_pClassificationModel = new QStandardItemModel(this);
    m_pClassificationModel->setHorizontalHeaderLabels(QStringList()<<tr("Class name"));
    m_pTreeViewClassfication->setModel(m_pClassificationModel);
    m_pLabelClassificationOutput = new QLabel(this);
    m_pLabelClassificationListHeader = new QLabel(tr("Class labels"),this);
    m_pTreeViewClassfication->header()->setVisible(false);


    QVBoxLayout *clVBoxLayout = new QVBoxLayout;
    clVBoxLayout->addWidget(m_pLabelClassificationListHeader);
    clVBoxLayout->addWidget(m_pTreeViewClassfication);
    clVBoxLayout->addWidget(m_pLabelClassificationOutput);

    m_pGroupBoxClassification->setLayout(clVBoxLayout);

    m_pGroupBoxClassification->setVisible(false);

    connect(m_pClassificationModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onClassLabelCheckstateToggled(QStandardItem*)));
}

//!
void ItiOtbImageViewerPanelSetupTab::initialize(){
    setupColorCompositionGroupBox();

    setupContrastEnhancememtGroupBox();

    setupClassficationGroupBox();

    QHBoxLayout *hboxLayout = new QHBoxLayout(this);
    hboxLayout->addWidget(m_pGroupBoxColorComposition);
    hboxLayout->addWidget(m_pGroupBoxContrastEnhancement);
    hboxLayout->addWidget(m_pGroupBoxClassification);
    hboxLayout->addSpacerItem(new QSpacerItem(100,100,QSizePolicy::Expanding,QSizePolicy::Fixed));
    setLayout(hboxLayout);
}

//!
void ItiOtbImageViewerPanelSetupTab::onChannelValueChanged(int val){
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

    if(list.isEmpty())
        return;

    m_pSpinBoxGreyscaleChannel->setMinimum(1);
    m_pSpinBoxRedChannel->setMinimum(1);
    m_pSpinBoxGreenChannel->setMinimum(1);
    m_pSpinBoxBlueChannel->setMinimum(1);
    m_pSpinBoxRedChannel->setMaximum(list.size());
    m_pSpinBoxGreenChannel->setMaximum(list.size());
    m_pSpinBoxBlueChannel->setMaximum(list.size());
    m_pSpinBoxGreyscaleChannel->setMaximum(list.size());

    m_pSpinBoxGreyscaleChannel->setValue(list.at(0)+1);
    m_pSpinBoxRedChannel->setValue(list.at(0)+1);

    if(list.size()>1){
        m_pSpinBoxGreenChannel->setValue(list.at(1)+1);

        if(list.size() > 2)
            m_pSpinBoxBlueChannel->setValue(list.at(2)+1);
        else
            m_pSpinBoxBlueChannel->setValue(list.at(1)+1);

        if(m_pRadioButtonGrayscaleMode->isChecked())
            m_pRadioButtonGrayscaleMode->toggle();

        if(!m_pRadioButtonRGBCompositionMode->isChecked())
            m_pRadioButtonRGBCompositionMode->toggle();

        m_pButtonApplyColorComposition->setEnabled(true);

        m_isGreyscale = false;
    }
    else{
        m_pSpinBoxGreenChannel->setValue(list.at(0)+1);
        m_pSpinBoxBlueChannel->setValue(list.at(0)+1);

        if(!m_pRadioButtonGrayscaleMode->isChecked())
            m_pRadioButtonGrayscaleMode->toggle();

        if(m_pRadioButtonRGBCompositionMode->isChecked())
            m_pRadioButtonRGBCompositionMode->toggle();

        m_pButtonApplyColorComposition->setEnabled(false);

        m_isGreyscale = true;
    }


    onRadioButtonColorCompositionChanged();

    setupClassificationData();
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
        m_isGreyscale = true;
        emit applyColorCompositionGreyscale();
    }else {
        m_isGreyscale = false;

        emit applyColorCompositionRGB();
    }

}

void ItiOtbImageViewerPanelSetupTab::setupClassificationData(){
    m_pClassificationModel->clear();

    QHash<int ,QColor> cdata = m_pItiOtbImageViewerPanel->manager()->classificationColorsIds();

    QList<Level*> rlist = m_pItiOtbImageViewerPanel->manager()->levels();
    if(rlist.isEmpty())
        return;

    Level *pLevel = rlist.first();

    QStandardItem *pRootItem = new QStandardItem(tr("Level %1").arg(QString::number(pLevel->id())));
    pRootItem->setFlags(pRootItem->flags() | Qt::ItemIsUserCheckable);
    pRootItem->setData(Qt::Checked,Qt::CheckStateRole);
    m_pClassificationModel->setItem(m_pClassificationModel->rowCount(),pRootItem);

    QHash<int,QString> data = m_pItiOtbImageViewerPanel->manager()->classficationNamesIds();
    QHash<int,QString>::const_iterator i;
    for(i = data.constBegin(); i != data.constEnd(); i++){
        QString name = i.value();
        int id = i.key();

        QStandardItem *pItem = new QStandardItem(name);
        QColor c = cdata.value(id);
        pItem->setData(c, Qt::DecorationRole);
        pItem->setData(id,Qt::UserRole);
        pItem->setData(Qt::Checked,Qt::CheckStateRole);
        pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);

        pRootItem->appendRow(pItem);
//        m_pClassificationModel->setItem(pRootItem->rowCount(),pItem);
    }

    m_pTreeViewClassfication->expandAll();

    QString text;
    text += tr("<h3>Classification statistics</h3><br/>");

    QList<Level*> levellist = m_pItiOtbImageViewerPanel->manager()->levels();
    QList<Level*>::const_iterator j;
    for(j = levellist.constBegin(); j != levellist.constEnd();j++){
        Level *pLevel  = *j;

        QList<Region*> regionlist = pLevel->regions();
        text += tr("Level %1. Number of segmentation regions : %2").arg(QString::number(pLevel->id())).arg(QString::number(regionlist.size()));
        text += "<br/>";

    }

    m_pLabelClassificationOutput->setText(text);

    if(cdata.isEmpty())
        m_pGroupBoxClassification->setVisible(false);
    else
        m_pGroupBoxClassification->setVisible(true);
}


void ItiOtbImageViewerPanelSetupTab::onClassLabelCheckstateToggled(QStandardItem *item){
    int cid = item->data(Qt::UserRole).toInt();
    bool checked = item->data(Qt::CheckStateRole).toBool();

    if(item->hasChildren()){
        for(int i = 0; i < item->rowCount(); i++){
            QStandardItem *citem = item->child(i,0);
            citem->setData(checked,Qt::CheckStateRole);
        }
    }else{
        emit classLabelToggled(checked,cid);

        emit classLabelChanged();
    }
}

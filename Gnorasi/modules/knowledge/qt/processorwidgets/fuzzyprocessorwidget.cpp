#include "fuzzyprocessorwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "doublespinboxdelegate.h"

using namespace otb;
//using namespace itiviewer;

namespace voreen {

const std::string FuzzyProcessorWidget::loggerCat_("voreen.FuzzyProcessorWidget");

FuzzyProcessorWidget::FuzzyProcessorWidget(QWidget *parent, FuzzyProcessor *fuzzyProcessor)
    : QProcessorWidget(fuzzyProcessor, parent)
{
    tgtAssert(fuzzyProcessor, "No FuzzyProcessorWidget processor");

    setWindowTitle(QString::fromStdString(fuzzyProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void FuzzyProcessorWidget::initialize(){
    QProcessorWidget::initialize();

    QGroupBox *pGroupBox = new QGroupBox(tr("Right-shoulder parameters"),this);

    m_pPushButtonAdd = new QPushButton(this);
    m_pPushButtonRemove = new QPushButton(this);
    m_pPushButtonCalculate = new QPushButton(this);

    m_pPushButtonAdd->setText(tr("Add"));
    m_pPushButtonRemove->setText(tr("Remove"));
    m_pPushButtonCalculate->setText(tr("Calculate"));

    m_pModel = new QStandardItemModel(this);
    QStringList headers;
    headers << tr("Attribute Name");
    m_pModel->setHorizontalHeaderLabels(headers);

    m_pModelSelection = new QStandardItemModel(this);
    QStringList headers1;
    headers1 << tr("Attribute Name") << "a" << "b";
    m_pModelSelection->setHorizontalHeaderLabels(headers1);

    m_pTableViewAvailable = new QTableView(this);
    m_pTableViewSelection = new QTableView(this);
    m_pTableViewAvailable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    m_pTableViewSelection->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pTableViewAvailable->setAlternatingRowColors(true);
    m_pTableViewSelection->setAlternatingRowColors(true);
    m_pTableViewAvailable->setModel(m_pModel);
    m_pTableViewSelection->setModel(m_pModelSelection);


    QHBoxLayout *layout = new QHBoxLayout;

    layout->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Expanding,QSizePolicy::Fixed));
    layout->addWidget(m_pPushButtonCalculate);

    QVBoxLayout *vboxLayout2 = new QVBoxLayout;
    vboxLayout2->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Fixed,QSizePolicy::Expanding));
    vboxLayout2->addWidget(m_pPushButtonAdd);
    vboxLayout2->addWidget(m_pPushButtonRemove);
    vboxLayout2->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Fixed,QSizePolicy::Expanding));

    QHBoxLayout *hboxLayout2 = new QHBoxLayout;
    hboxLayout2->addWidget(m_pTableViewAvailable);
    hboxLayout2->addLayout(vboxLayout2);
    hboxLayout2->addWidget(m_pTableViewSelection);

    QFrame *pFrame = new QFrame(this);
    pFrame->setFrameShape(QFrame::HLine);
    pFrame->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(layout);
    vLayout->addWidget(pFrame);
    vLayout->addLayout(hboxLayout2);

    pGroupBox->setLayout(vLayout);

    QHBoxLayout *bbLayout = new QHBoxLayout(this);
    bbLayout->addWidget(pGroupBox);
    setLayout(bbLayout);

    m_pFuzzyLabelMapUtility = new FuzzyLabelMapUtility(this);

    setupABFields();

    connect(m_pPushButtonAdd,SIGNAL(clicked()),this,SLOT(addSelection()));
    connect(m_pPushButtonCalculate,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(m_pPushButtonRemove,SIGNAL(clicked()),this,SLOT(removeSelection()));
}

void FuzzyProcessorWidget::setupABFields(){
    DoubleSpinBoxDelegate *spinBoxDelegate = new DoubleSpinBoxDelegate(-9999999.0, 9999999.0,2,this);
    m_pTableViewSelection->setItemDelegateForColumn(1,spinBoxDelegate);

    DoubleSpinBoxDelegate *spinBoxDelegate1 = new DoubleSpinBoxDelegate(-9999999.0, 9999999.0,2,this);
    m_pTableViewSelection->setItemDelegateForColumn(2,spinBoxDelegate1);
}


//!
void FuzzyProcessorWidget::updateFromProcessor(){
    FuzzyLabelMapUtility::LabelMapType *mapT = getMapFromPort();
    if(mapT){
        m_pFuzzyLabelMapUtility->parse(mapT);

        QStringList namesList = m_pFuzzyLabelMapUtility->getAttributeListNames();

        setupAvailableTableByList(namesList);
    }
}


FuzzyLabelMapUtility::LabelMapType* FuzzyProcessorWidget::getMapFromPort(){
    FuzzyProcessor* fProcessor = dynamic_cast<FuzzyProcessor*>(processor_);

    if(!fProcessor)
        return 0;

    std::vector<Port*> l = fProcessor->getInports();
    if(l.empty()){
        qDebug() << "label map port list is empty..";

    }else{
        //! get the first port
        Port *port = l.at(0);
        if(port->isConnected()){
            OTBLabelMapPort *lblMapPort = dynamic_cast<OTBLabelMapPort*>(port);
            if(lblMapPort){
                FuzzyLabelMapUtility::LabelMapType *mapT = (FuzzyLabelMapUtility::LabelMapType*)lblMapPort->getData();

                return mapT;
            }
        }
    }
    return 0;
}

void FuzzyProcessorWidget::setupAvailableTableByList(const QStringList &list){
    m_pModel->removeRows(0,m_pModel->rowCount());

    QStringList::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        QString name = *i;

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(name,Qt::DisplayRole);

        m_pModel->appendRow(pItem);
    }
}

void FuzzyProcessorWidget::addSelection(){
    QModelIndex index = m_pTableViewAvailable->currentIndex();

    if(!index.isValid())
        return;

    QString name = m_pModel->data(index).toString();

    QStandardItem *pItem = new QStandardItem();
    pItem->setText(name);

    m_pModelSelection->appendRow(pItem);
}

void FuzzyProcessorWidget::removeSelection(){
    QModelIndex index = m_pTableViewSelection->currentIndex();

    if(!index.isValid())
        return;

    m_pModelSelection->removeRow(index.row());
}

void FuzzyProcessorWidget::calculate(){

    FuzzyLabelMapUtility::LabelMapType *lblMap = getMapFromPort();

    for(int i = 0; i < m_pModelSelection->rowCount(); i++){
        QStandardItem *pItem = m_pModelSelection->item(i,0);
        QString atName = pItem->data(Qt::DisplayRole).toString();

        QStandardItem *pItemA =m_pModelSelection->item(i,1);
        if(!pItemA)
            continue;

        m_pFuzzyLabelMapUtility->updateMinValue(pItemA->data(Qt::DisplayRole).toDouble());

        QStandardItem *pItemB = m_pModelSelection->item(i,2);
        if(!pItemB)
            continue;

        m_pFuzzyLabelMapUtility->updateMinValue(pItemB->data(Qt::DisplayRole).toDouble());

        m_pFuzzyLabelMapUtility->calculateValues(lblMap,atName);
    }





    FuzzyProcessor *fProcessor = dynamic_cast<FuzzyProcessor*>(processor_);
    if(!fProcessor)
        return;

//    const OTBLabelMapPort::LabelMapPointer lpointer = (OTBLabelMapPort::LabelMapPointer*)(lblMap);

    fProcessor->setOutputData(lblMap);

    QString text = m_pFuzzyLabelMapUtility->constructCsvFromLabelMap(lblMap);

    fProcessor->setTextOutputData(text.toStdString());

//    qDebug() << text;

//    QFile file(QFileDialog::getSaveFileName(this,tr("Save"),QDir::homePath()));
//    if(!file.open(QIODevice::WriteOnly))
//        return;

//    QTextStream out(&file);
//    out << text;
//    file.close();

}

FuzzyProcessorWidget::~FuzzyProcessorWidget(){
//    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen

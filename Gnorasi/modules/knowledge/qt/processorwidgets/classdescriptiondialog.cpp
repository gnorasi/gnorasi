#include "classdescriptiondialog.h"

#include "fuzzyruleview.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

#include "../utils/objectlevelmanager.h"
#include "../utils/objectlevel.h"
#include "../models/ontologyclass.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../fuzzy/fuzzyrule.h"
#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"
#include "../fuzzy/fuzzyoperator.h"
#include "../fuzzy/fuzzyoperatormanager.h"
#include "../fuzzy/fuzzyrulemanager.h"
#include "../fuzzy/fuzzyfunction.h"
#include "../fuzzy/fuzzyfunctionmanager.h"

#include <QDebug>
#include <QTime>

ClassDescriptionDialog::ClassDescriptionDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Class Description"));

    initialize();

    // Create seed for the random
    // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

void ClassDescriptionDialog::clearFuzzyRules(){
    m_pFuzzyRuleModel->removeRows(0,m_pFuzzyRuleModel->rowCount());
}

void ClassDescriptionDialog::initializeFuzzyOperators(){
    FUZZYOPERATORMANAGER->clear();

    if(FUZZYOPERATORMANAGER->count())
        return;

    FuzzyOperator *pOperator = new FuzzyOperator(this);
    pOperator->setName(tr("min"));
    FUZZYOPERATORMANAGER->addOperator(pOperator);

    pOperator = new FuzzyOperator(this);
    pOperator->setName(tr("max"));
    FUZZYOPERATORMANAGER->addOperator(pOperator);

}

void ClassDescriptionDialog::initializeFuzzyRuleTreeView(){
    QStandardItem *pContainedItem = new QStandardItem();
    pContainedItem->setData(tr("Contained"),Qt::DisplayRole);
    pContainedItem->setData(-101);

    m_pFuzzyRuleModel->setItem(0,pContainedItem);

    m_pInheritedItem = new QStandardItem();
    m_pInheritedItem->setData(tr("Inherited"),Qt::DisplayRole);
    m_pInheritedItem->setData(-102);

    m_pFuzzyRuleModel->setItem(1,m_pInheritedItem);

    QList<FuzzyOperator*> list = FUZZYOPERATORMANAGER->fuzzyOperatorList();
    QList<FuzzyOperator*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        FuzzyOperator *pO = *i;

        QString name = pO->name();
        m_pOperatorItem = new QStandardItem();
        m_pOperatorItem->setData(name,Qt::DisplayRole);
        m_pOperatorItem->setData(-103);
        pContainedItem->setChild(0,m_pOperatorItem);

        m_pFuzzyRuleView->setOperatorItem(m_pOperatorItem);

        m_pFuzzyRuleView->expand(m_pFuzzyRuleModel->indexFromItem(pContainedItem));

        return;
    }

}


void ClassDescriptionDialog::processParentClass(OntologyClass *pClass){
    QFont font;
    font.setBold(true);

    QString opername = pClass->opername();
    QStandardItem *pOperatorItem = new QStandardItem();
    pOperatorItem->setData(-103);
    pOperatorItem->setData(QString("%1 (%2)").arg(opername).arg(pClass->name()),Qt::DisplayRole);
    pOperatorItem->setData(font, Qt::FontRole);
    m_pInheritedItem->setChild(m_pInheritedItem->rowCount(),pOperatorItem);

    QList<FuzzyRule*> fList = pClass->fuzzyRuleList(pClass->level());

    QList<FuzzyRule*>::const_iterator i;
    for(i = fList.constBegin(); i != fList.constEnd(); i++){

        FuzzyRule *pRule = *i;

        QString name = pRule->name();

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(name,Qt::DisplayRole);
        pItem->setData(pRule->id());

        pOperatorItem->setChild(pOperatorItem->rowCount(),pItem);
    }

    OntologyClass *pParentClass = pClass->parent();
    if(pParentClass)
        processParentClass(pParentClass);
}


void ClassDescriptionDialog::setupData(){

    Q_ASSERT(m_pOperatorItem);

    OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_classId);
    if(!pClass)
        return;

    m_pFuzzyRuleView->setClassId(pClass->id());

    int levelId = pClass->level();
    ObjectLevel *pOL = OBJECTLEVELMANAGER->objectLevelById(levelId);
    if(!pOL)
        return;

    QString helpertext = QString("Level %1").arg(levelId);
    int ci = m_pObjectLevelComboBox->findText(helpertext);
    m_pObjectLevelComboBox->setCurrentIndex(ci);

    m_pQLineEdit->setText(pClass->name());

    QString opername = pClass->opername();
    m_pOperatorItem->setData(opername,Qt::DisplayRole);

    QList<FuzzyRule*> fList = pClass->fuzzyRuleList(levelId);

    QList<FuzzyRule*>::const_iterator i;
    for(i = fList.constBegin(); i != fList.constEnd(); i++){

        FuzzyRule *pRule = *i;

        QString name = pRule->name();

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(name,Qt::DisplayRole);
        pItem->setData(pRule->id());

        m_pOperatorItem->setChild(m_pOperatorItem->rowCount(),pItem);
    }

    m_pFuzzyRuleView->expand(m_pFuzzyRuleModel->indexFromItem(m_pOperatorItem));

    OntologyClass *pParentClass = pClass->parent();
    if(pParentClass)
        processParentClass(pParentClass);

    m_pFuzzyRuleView->expandAll();
}


void ClassDescriptionDialog::initialize(){
    QGroupBox *pQGroupBox       = new QGroupBox(tr("Name"),this);
    QGroupBox *pQGroupBox1      = new QGroupBox(tr("Level"),this);
    QLabel *pLabel2             = new QLabel(tr("Expression"),this);
    m_pQLineEdit                = new QLineEdit(this);
    m_pObjectLevelComboBox      = new QComboBox(this);

    m_pQLineEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this ));

    QHBoxLayout *hboxlayout     = new QHBoxLayout();
    hboxlayout->addWidget(m_pQLineEdit);
    pQGroupBox->setLayout(hboxlayout);

    QHBoxLayout *hboxlayout1    = new QHBoxLayout();
    hboxlayout1->addWidget(m_pObjectLevelComboBox);
    pQGroupBox1->setLayout(hboxlayout1);

    m_pOkButton                 = new QPushButton(tr("Ok"),this);
    m_pCancelButton             = new QPushButton(tr("Cancel"),this);

    QHBoxLayout *hboxlayout2    = new QHBoxLayout();
    hboxlayout2->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout2->addWidget(m_pOkButton);
    hboxlayout2->addWidget(m_pCancelButton);

    m_pFuzzyRuleModel = new QStandardItemModel(this);
    m_pFuzzyRuleView = new FuzzyRuleView(this);
    m_pFuzzyRuleView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pFuzzyRuleView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pFuzzyRuleView->setModel(m_pFuzzyRuleModel);

    QVBoxLayout *vboxlayout     = new QVBoxLayout();
    vboxlayout->addWidget(pQGroupBox);
    vboxlayout->addWidget(pQGroupBox1);
    vboxlayout->addWidget(pLabel2);
    vboxlayout->addWidget(m_pFuzzyRuleView);
    vboxlayout->addLayout(hboxlayout2);

    QStringList headers;
    headers << tr("Name");

    m_pFuzzyRuleModel->setHorizontalHeaderLabels(headers);

    setupObjectLevels();

    initializeFuzzyOperators();
    initializeFuzzyRuleTreeView();

    setLayout(vboxlayout);

    connect(m_pOkButton,SIGNAL(clicked()),this,SLOT(onOkClicked()));
    connect(m_pCancelButton,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(m_pObjectLevelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onLevelComboboxChanged(int)));
    connect(m_pFuzzyRuleView,SIGNAL(fuzzyRuleAdded(int)),this,SLOT(onFuzzyRuleAdded(int)));
}

void ClassDescriptionDialog::onLevelComboboxChanged( int index){
    int lid = levelId();
    m_pFuzzyRuleView->setLevelId(lid);
}

int ClassDescriptionDialog::levelId() const{
    QString levelText = m_pObjectLevelComboBox->currentText();
    int levelId = levelText.mid(levelText.indexOf(" ")+1).toInt();

    ObjectLevel *pLevel = OBJECTLEVELMANAGER->objectLevelById(levelId);
    if(!pLevel){
        return -1;
    }

    return levelId;
}

bool ClassDescriptionDialog::createNewClass(){

    QString name = m_pQLineEdit->text();
    if(!validateLabel(name)){
        QMessageBox::critical(this,tr("Create error"),tr("Could not create the ontology class, please change the name and try again."),QMessageBox::Ok);
        return false;
    }

    int lId = levelId();

    OntologyClass *pParentClass = NULL;

    if(!m_parentClassId.isEmpty()){
        pParentClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_parentClassId);
    }

    OntologyClass *pClass = new OntologyClass(pParentClass);
    pClass->setName(name);
    pClass->setId(name);
    pClass->setLevel(lId);
    pClass->setopername(m_pOperatorItem->data(Qt::DisplayRole).toString());
    pClass->setIdx(ONTOLOGYCLASSIFICATIONMANAGER->uniqueIdx());
    int r = ClassDescriptionDialog::randInt(0,255);
    int g = ClassDescriptionDialog::randInt(0,255);
    int b = ClassDescriptionDialog::randInt(0,255);
    QColor color = QColor(r,g,b);
    pClass->setColor(color.name());
    m_classId = name;
    ONTOLOGYCLASSIFICATIONMANAGER->addOntologyClass(pClass);

    if(pParentClass){
        pParentClass->addChild(pClass);
        pClass->setparentId(pParentClass->id());
        pClass->setParent(pParentClass);
    }

    return true;
}

bool ClassDescriptionDialog::editClass(){
    OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_classId);
    if(!pClass)
        return false;

    QString name = m_pQLineEdit->text();

    if(name.compare(pClass->id())){

        if(!validateLabel(name)){
            QMessageBox::critical(this,tr("Create error"),tr("Could not create the ontology class,please change the name and try again."),QMessageBox::Ok);
            return false;
        }

        pClass->setName(name);
        pClass->setId(name);
    }

    int lId = levelId();
    QString opername = m_pOperatorItem->data(Qt::DisplayRole).toString();

    pClass->setLevel(lId);
    pClass->setopername(opername);
    m_classId = name;

    return true;
}

void ClassDescriptionDialog::onOkClicked(){
    if(m_classId.isEmpty()){
        if(!createNewClass())
            return;
    } else if (!editClass())
        return;

    accept();
}


void ClassDescriptionDialog::onCancelClicked(){
    // do not fucking do sth

    reject();
}


void ClassDescriptionDialog::setupObjectLevels()
{
    m_pObjectLevelComboBox->clear();

    QList<ObjectLevel*> levelList = OBJECTLEVELMANAGER->objectLevelList();
    QList<ObjectLevel*>::const_iterator i;
    for(i = levelList.constBegin(); i != levelList.constEnd(); i++){
        ObjectLevel *pLevel = *i;

        QString name = pLevel->name();
        int id = pLevel->id();

        m_pObjectLevelComboBox->addItem(name,id);
        m_pFuzzyRuleView->setLevelId(id);
    }
}

//! this is a validation label functionality . All it does is to check for already added OntologyClass and
//! return false if there are already functions and true if no
bool ClassDescriptionDialog::validateLabel(const QString &name){

    QList<OntologyClass*> list = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassList();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClass *pClass = *i;

        QString oname = pClass->name();

        if( !oname.compare(name))
            return false;
    }

    return true;
}


void ClassDescriptionDialog::onFuzzyRuleAdded(int id){
    FuzzyRule *pRule = FUZZYRULEMANAGER->fuzzyRuleById(id);
    if(!pRule)
        return;

    OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_classId);
    if(!pClass)
        return;

    pClass->addFuzzyRule(levelId(),pRule);

    QStandardItem *pItem = new QStandardItem();
    pItem->setData(pRule->name(),Qt::DisplayRole);
    pItem->setData(pRule->id());

    m_pOperatorItem->setChild(m_pOperatorItem->rowCount(),pItem);
    m_pFuzzyRuleView->expandAll();
}

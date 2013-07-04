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

ClassDescriptionDialog::ClassDescriptionDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Class Description"));

    initialize();
}

void ClassDescriptionDialog::clearFuzzyRules(){
    m_pFuzzyRuleModel->removeRows(0,m_pFuzzyRuleModel->rowCount());
}

void ClassDescriptionDialog::initializeFuzzyOperators(){
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

    QStandardItem *pInheritedItem = new QStandardItem();
    pInheritedItem->setData(tr("Inherited"),Qt::DisplayRole);
    pInheritedItem->setData(-102);

    m_pFuzzyRuleModel->setItem(1,pInheritedItem);

    QList<FuzzyOperator*> list = FUZZYOPERATORMANAGER->fuzzyOperatorList();
    QList<FuzzyOperator*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        FuzzyOperator *pO = *i;

        QString name = pO->name();
        m_pOperatorItem = new QStandardItem();
        m_pOperatorItem->setData(name,Qt::DisplayRole);
        m_pOperatorItem->setData(-103);
        pContainedItem->setChild(0,m_pOperatorItem);

        m_pFuzzyRuleView->expand(m_pFuzzyRuleModel->indexFromItem(pContainedItem));

        return;
    }

}

void ClassDescriptionDialog::setupData(){

    Q_ASSERT(m_pOperatorItem);

    OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_classId);
    if(!pClass)
        return;

    int levelId = pClass->level();
    ObjectLevel *pOL = OBJECTLEVELMANAGER->objectLevelById(levelId);
    if(!pOL)
        return;

    QString helpertext = QString("Level %1").arg(levelId);
    int ci = m_pObjectLevelComboBox->findText(helpertext);
    m_pObjectLevelComboBox->setCurrentIndex(ci);

    m_pQLineEdit->setText(pClass->name());


    QHash<int,FuzzyRule*> rulesList = pClass->fuzzyRuleHash();
    QHash<int,FuzzyRule*>::const_iterator i;
    for(i = rulesList.constBegin(); i != rulesList.constEnd(); i++){
        int attributeId = i.key();

        FuzzyRule *pRule = i.value();

        ObjectAttribute *pAttribute = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(levelId,attributeId);
        if(!pAttribute)
            continue;

        QString name = pAttribute->name();

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(name,Qt::DisplayRole);
        pItem->setData(pRule->name());

        m_pOperatorItem->setChild(m_pOperatorItem->rowCount(),pItem);

        m_pFuzzyRuleView->expand(m_pFuzzyRuleModel->indexFromItem(m_pOperatorItem));
    }
}

void ClassDescriptionDialog::initialize(){
    QGroupBox *pQGroupBox       = new QGroupBox(tr("Name"),this);
    QGroupBox *pQGroupBox1      = new QGroupBox(tr("Level"),this);
    QLabel *pLabel2             = new QLabel(tr("Expression"),this);
    m_pQLineEdit                = new QLineEdit(this);
    m_pObjectLevelComboBox      = new QComboBox(this);

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

void ClassDescriptionDialog::createNewClass(){

    QString name = m_pQLineEdit->text();
    if(!validateLabel(name)){
        QMessageBox::critical(this,tr("Create error"),tr("Could not create the ontology class,please change the name and try again."),QMessageBox::Ok);
        return;
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
    m_classId = name;
    ONTOLOGYCLASSIFICATIONMANAGER->addOntologyClass(pClass);

    if(pParentClass)
        pParentClass->addChild(pClass);
}

void ClassDescriptionDialog::editClass(){
    OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_classId);
    if(!pClass)
        return;

    QString name = m_pQLineEdit->text();

    if(name.compare(pClass->id())){

        if(!validateLabel(name)){
            QMessageBox::critical(this,tr("Create error"),tr("Could not create the ontology class,please change the name and try again."),QMessageBox::Ok);
            return;
        }

        pClass->setName(name);
        pClass->setId(name);
    }

    int lId = levelId();

    pClass->setLevel(lId);
    m_classId = name;
}

void ClassDescriptionDialog::onOkClicked(){
    if(m_classId.isEmpty())
        createNewClass();
    else
        editClass();

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

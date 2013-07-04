#include "membershipfunctiondialog.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>

#include "../fuzzy/fuzzyfunction.h"
#include "../fuzzy/fuzzyfunctionmanager.h"
#include "../fuzzy/fuzzyfunctionfactory.h"
#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"

#include "../fuzzy/fuzzyrule.h"
#include "../fuzzy/fuzzyrulemanager.h"

MembershipFunctionDialog::MembershipFunctionDialog(int lid, const QString &atid, QWidget *parent) :
    m_levelId(lid), m_attributeId(atid), QDialog(parent)
{
    initialize();
}


void MembershipFunctionDialog::setupByRuleId(int id){
    FuzzyRule *prule = FUZZYRULEMANAGER->fuzzyRuleById(id);
    if(!prule)
        return;

    m_pParameterModel->clear();

    QString atid = prule->attribute();
    m_attributeId = atid;
    ObjectAttribute *pAttr = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(m_levelId,atid);
    if(pAttr)
        m_pAttributeLabel->setText(pAttr->name());

    FuzzyFunction *pFunction = prule->funzzyFunction();
    if(pFunction){
        // fucking do sth

        int pcount = pFunction->parametersCount();
        QStringList headers;
        for(int i = 0; i < pcount; i++){

            int code = 97; // 'a'
            code += i;

            QString lstr = QString("%1").arg((char)code);

            headers << lstr;

            QStandardItem *pItem = new QStandardItem();
            m_pParameterModel->setItem(i,pItem);
        }

        m_pParameterModel->setVerticalHeaderLabels(headers);

        QStringList hheaders;
        hheaders << tr("Value");
        m_pParameterModel->setHorizontalHeaderLabels(hheaders);
    }
}

void MembershipFunctionDialog::setupAvailableFunctionsButtons(){
    if(!FUZZYFUNCTIONMANAGER->count()){
        FuzzyFunction *pFunction = m_pFuzzyFunctionFactory->createFuzzyFunction(QString::fromAscii(FUZZYFUNCTIONLEFTSHOULDER));
        pFunction->seticon(QString(":/voreenve/icons/leftShoulder.jpg"));
        FUZZYFUNCTIONMANAGER->addFuzzyFunction(pFunction);

        pFunction = m_pFuzzyFunctionFactory->createFuzzyFunction(QString::fromAscii(FUZZYFUNCTIONRIGHTSHOULDER));
        pFunction->seticon(QString(":/voreenve/icons/rightShoulder.jpg"));
        FUZZYFUNCTIONMANAGER->addFuzzyFunction(pFunction);

        pFunction = m_pFuzzyFunctionFactory->createFuzzyFunction(QString::fromAscii(FUZZYFUNCTIONTRAPEZOIDAL));
        pFunction->seticon(QString(":/voreenve/icons/trapezoidal.jpg"));
        FUZZYFUNCTIONMANAGER->addFuzzyFunction(pFunction);

        pFunction = m_pFuzzyFunctionFactory->createFuzzyFunction(QString::fromAscii(FUZZYFUNCTIONTRIANGLULAR));
        pFunction->seticon(QString(":/voreenve/icons/triangular.jpg"));
        FUZZYFUNCTIONMANAGER->addFuzzyFunction(pFunction);
    }

    createbuttons();
}

void MembershipFunctionDialog::createbuttons(){
    int counter = 0;
    QList<FuzzyFunction*> list = FUZZYFUNCTIONMANAGER->fuzzyFunctionList();
    QList<FuzzyFunction*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){

        FuzzyFunction *pfunction = *i;
        QString name = pfunction->name();
        QString icon = pfunction->icon();
        m_helperHash[counter] = name;

        QPushButton *button = new QPushButton(name,this);
        button->setMinimumHeight(120);
        button->setIcon(QIcon(icon));

        m_pButtonGroup->addButton(button,counter);

        m_pHBoxButtonLayout->addWidget(button);

        counter++;
    }
}

void MembershipFunctionDialog::initialize(){
    QLabel *pfearureLabel = new QLabel(tr("Feature"),this);
    m_pAttributeLabel = new QLabel(tr("title"),this);

    ObjectAttribute *pAttr = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(m_levelId,m_attributeId);
    if(pAttr)
        m_pAttributeLabel->setText(pAttr->name());

    QGroupBox *pGroupBox = new QGroupBox(this);
    QVBoxLayout *vboxlayout = new QVBoxLayout;

    QHBoxLayout *hboxlayout1 = new QHBoxLayout;
    hboxlayout1->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout1->addWidget(pfearureLabel);
    hboxlayout1->addSpacerItem(new  QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    QHBoxLayout *hboxlayout2 = new QHBoxLayout;
    hboxlayout2->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout2->addWidget(m_pAttributeLabel);
    hboxlayout2->addSpacerItem(new  QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    vboxlayout->addLayout(hboxlayout1);
    vboxlayout->addLayout(hboxlayout2);
    pGroupBox->setLayout(vboxlayout);

    m_pParameterListView = new QTableView(this);
    m_pParameterListView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pParameterModel = new QStandardItemModel();
    m_pParameterListView->setModel(m_pParameterModel);

    m_pFuzzyFunctionFactory = new FuzzyFunctionFactory(this);

    QGroupBox *pGroupBox1 = new QGroupBox(tr("Initialize"),this);
    m_pButtonGroup = new QButtonGroup(this);
    m_pHBoxButtonLayout = new QHBoxLayout;
    pGroupBox1->setLayout(m_pHBoxButtonLayout);

    setupAvailableFunctionsButtons();

    QGroupBox *pGroupBox2 = new QGroupBox(tr("Function parameters"),this);
    QVBoxLayout *vboxlayout2 = new QVBoxLayout;
    vboxlayout2->addWidget(m_pParameterListView);
    pGroupBox2->setLayout(vboxlayout2);

    m_pOkButton = new QPushButton(tr("Ok"), this);
    m_pCancelButton = new QPushButton(tr("Cancel"),this);

    QHBoxLayout *hboxlayout3 = new QHBoxLayout;
    hboxlayout3->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout3->addWidget(m_pOkButton);
    hboxlayout3->addWidget(m_pCancelButton);

    QVBoxLayout *totalLayout = new QVBoxLayout;
    totalLayout->addWidget(pGroupBox);
    totalLayout->addWidget(pGroupBox1);
    totalLayout->addWidget(pGroupBox2);
    totalLayout->addLayout(hboxlayout3);

    setLayout(totalLayout);

    connect(m_pOkButton,SIGNAL(clicked()),this,SLOT(onOkClicked()));
    connect(m_pCancelButton,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(m_pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(onFuzzyFunctionButtonClicked(int)));
}


void MembershipFunctionDialog::onFuzzyFunctionButtonClicked(int id){
    QString at = m_helperHash.value(id);
    FuzzyFunction *pFunction = FUZZYFUNCTIONMANAGER->fuzzyFunctionByName(at);
    if(!pFunction)
        return;

    m_pParameterModel->clear();

    int pcount = pFunction->parametersCount();
    QStringList headers;
    for(int i = 0; i < pcount; i++){

        int code = 97; // 'a'
        code += i;

        QString lstr = QString("%1").arg((char)code);

        headers << lstr;

        QStandardItem *pItem = new QStandardItem();
        m_pParameterModel->setItem(i,pItem);
    }

    m_pParameterModel->setVerticalHeaderLabels(headers);

    QStringList hheaders;
    hheaders << tr("Value");
    m_pParameterModel->setHorizontalHeaderLabels(hheaders);

}

void MembershipFunctionDialog::onOkClicked(){

    accept();
}

void MembershipFunctionDialog::onCancelClicked(){
    reject();
}

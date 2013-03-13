#include "fuzzydefinitionprocessorwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "comboboxdelegate.h"

#include <QDomDocument>

using namespace otb;
//using namespace itiviewer;

namespace voreen {

const std::string FuzzyDefinitionProcessorWidget::loggerCat_("voreen.FuzzyDefinitionProcessorWidget");

FuzzyDefinitionProcessorWidget::FuzzyDefinitionProcessorWidget(QWidget *parent, FuzzyDefinitionProcessor *fuzzyDefinitionProcessor)
    : QProcessorWidget(fuzzyDefinitionProcessor, parent)
{
    tgtAssert(fuzzyDefinitionProcessor, "No FuzzyDefinitionProcessorWidget processor");

    setWindowTitle(QString::fromStdString(fuzzyDefinitionProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void FuzzyDefinitionProcessorWidget::initialize(){
    QProcessorWidget::initialize();

    QGroupBox *pGroupBox = new QGroupBox(tr("Fuzzy Definition"),this);

    QLabel *pLabel = new QLabel(tr("Ontology Class"),this);

    m_pOntologyClassComboBox = new QComboBox(this);

    m_pRulesModel = new QStandardItemModel(this);

    m_pRulesTableView = new QTableView(this);
    QStringList headers;
    headers << tr("Name") << tr("Operator") << tr("Value");
    m_pRulesModel->setHorizontalHeaderLabels(headers);

    m_pMinRadioButton = new QRadioButton(this);
    m_pMaxRadioButton = new QRadioButton(this);

    m_pMaxRadioButton->setText(tr("Max"));
    m_pMinRadioButton->setText(tr("Min"));

    QButtonGroup *buttonGroup = new QButtonGroup(this);

    buttonGroup->addButton(m_pMinRadioButton);
    buttonGroup->addButton(m_pMaxRadioButton);

    m_pMaxRadioButton->setChecked(true);

    m_pRulesTableView->setModel(m_pRulesModel);

    setupOperatorField();
}


void FuzzyDefinitionProcessorWidget::setupOperatorField(){
    QStringList list;
    list << ">=" << ">" << "<=" << "<";
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(list,this);
    m_pRulesTableView->setItemDelegateForColumn(1,pComboBoxDelegate);
}


//!
void FuzzyDefinitionProcessorWidget::updateFromProcessor(){
    FuzzyLabelMapUtility::LabelMapType *mapT = getMapFromPort();
    if(mapT){
        setupFuzzyAtributes(mapT,QLatin1String("Fuzzy_"));
    }


}


FuzzyLabelMapUtility::LabelMapType* FuzzyDefinitionProcessorWidget::getMapFromPort(){
    FuzzyDefinitionProcessor* fProcessor = dynamic_cast<FuzzyDefinitionProcessor*>(processor_);

    if(!fProcessor)
        return 0;

    std::vector<Port*> l = fProcessor->getInports();
    if(l.empty()){
        qDebug() << "label map port list is empty..";

    }else{
        //! get the first port
        for(int i = 0; i < l.size(); i++){
            Port *port = l.at(i);
            if(port->isConnected()){
                OTBLabelMapPort *lblMapPort = dynamic_cast<OTBLabelMapPort*>(port);
                if(lblMapPort){
                    FuzzyLabelMapUtility::LabelMapType *mapT = (FuzzyLabelMapUtility::LabelMapType*)lblMapPort->getData();

                    return mapT;
                }
            }
        }
    }
    return 0;
}


void FuzzyDefinitionProcessorWidget::setupFuzzyAtributes(FuzzyLabelMapUtility::LabelMapType *mapT, const QString &prepkey){

    for(unsigned int i = 1; i < mapT->GetNumberOfLabelObjects(); i++){
        FuzzyLabelMapUtility::LabelObjectType* lblObject = mapT->GetLabelObject(i);

        std::vector<std::string> attrList = lblObject->GetAvailableAttributes();
        if(!attrList.empty()){
            for(int j = 0; j < attrList.size(); j ++){
                QString name = QString::fromStdString(attrList.at(j));

                if(name.startsWith(prepkey)){
//                    m_pOntologyClassesComboBox->addItem(name);
                    m_fuzzyAttributesList << name;
                    break;
                }
            }
        }
    }
}

QString FuzzyDefinitionProcessorWidget::constructXmlFile() const{
    QString text;

    QDomDocument doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    QDomElement rootElement = doc.createElement(QLatin1String("fuzzy"));

    QList<FuzzyRule*>::const_iterator i;
    for(i = m_fuzzyRuleList.constBegin(); i != m_fuzzyRuleList.constEnd(); i++){
        FuzzyRule *pRule = *i;
        QDomElement fuzzyRuleElemet = doc.createElement(QLatin1String("fuzzyRule"));
        fuzzyRuleElemet.setAttribute(QLatin1String("id"),pRule->id());
        fuzzyRuleElemet.setAttribute(QLatin1String("operator"),pRule->opr());
        rootElement.appendChild(fuzzyRuleElemet);

        QDomElement bodyElement = doc.createElement(QLatin1String("body"));
        fuzzyRuleElemet.appendChild(bodyElement);

        QList<FuzzyRestriction*> rlist = pRule->restrictions();
        QList<FuzzyRestriction*>::const_iterator j;
        for(j = rlist.constBegin(); j != rlist.constEnd(); j++){
            FuzzyRestriction *pRestriction = *j;

            QDomElement fuzzyRestrictionElement = doc.createElement("restriction");
            fuzzyRestrictionElement.setAttribute(QLatin1String("property"),pRestriction->fuzzyProperty);
            fuzzyRestrictionElement.setAttribute(QLatin1String("value"),QString::number(pRestriction->val,'f',2));
            fuzzyRestrictionElement.setAttribute(QLatin1String("operator"),pRestriction->opr);
            bodyElement.appendChild(fuzzyRestrictionElement);
        }
    }

    return text;
}



FuzzyDefinitionProcessorWidget::~FuzzyDefinitionProcessorWidget(){
//    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen

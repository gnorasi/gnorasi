#include <QDebug>
#include <QFileDialog>

#include "fuzzylabelmaputility.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

#include "itkShapeLabelObject.h"
#include "itkPolyLineParametricPath.h"
#include "itkVectorContainer.h"

using namespace otb;

FuzzyLabelMapUtility::FuzzyLabelMapUtility(QObject *parent) :
    QObject(parent)
{
}


void FuzzyLabelMapUtility::parse(LabelMapType *lblmap){

    for(unsigned int i = 1; i < lblmap->GetNumberOfLabelObjects(); i++){
        LabelObjectType* lblObject = lblmap->GetLabelObject(i);

        std::vector<std::string> attrList = lblObject->GetAvailableAttributes();
        if(!attrList.empty()){
            for(int j = 0; j < attrList.size(); j ++){
                QString name = QString::fromStdString(attrList.at(j));
                m_attributeList << name;
            }
            break;
        }
        else{

        }
    }
}


void FuzzyLabelMapUtility::calculateValues(LabelMapType *lblMap, const QStringList &list){
    for(unsigned int i = 1; i < lblMap->GetNumberOfLabelObjects(); i++){
        LabelObjectType* lblObject = lblMap->GetLabelObject(i);

        QStringList::const_iterator p;
        for(p = list.constBegin(); p != list.constEnd(); p++){
            QString atName = *p;
//            const std::string atKey = atName.toStdString();
            double val = (double)lblObject->GetAttribute(atName.toLatin1().constData());

            double calval = calculateValue(val,m_a,m_b);

            QString newName = QString("Fuzzy_%1").arg(atName);

            lblObject->SetAttribute(newName.toLatin1().constData(),calval);
        }
    }
}

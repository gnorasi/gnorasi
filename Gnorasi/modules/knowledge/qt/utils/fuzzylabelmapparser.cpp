#include <QDebug>
#include <QFileDialog>

#include "fuzzylabelmapparser.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

#include "itkShapeLabelObject.h"
#include "itkPolyLineParametricPath.h"
#include "itkVectorContainer.h"

using namespace otb;

FuzzyLabelMapParser::FuzzyLabelMapParser(QObject *parent) :
    QObject(parent)
{
}


void FuzzyLabelMapParser::parse(LabelMapType *lblmap){

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

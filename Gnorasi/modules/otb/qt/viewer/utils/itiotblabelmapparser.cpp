#include <QDebug>

#include "itiotblabelmapparser.h"

#include "itiotbregion.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

using namespace itiviewer;
using namespace otb;

LabelMapParser::LabelMapParser(QObject *parent) :
    QObject(parent)
{
}


QList<Region*> LabelMapParser::parse(LabelMapType *lblmap){
    QList<Region*> list;

    for(unsigned int i = 1; i < lblmap->GetNumberOfLabelObjects(); i++){
        LabelObjectType* lblObject = lblmap->GetLabelObject(i);

//        PolygonType *pol = lblObject->GetPolygon();

//        QStringList slist;

//        std::vector<std::string> list = lblObject->GetAvailableAttributes();
//        for(int l = 0; l < list.size(); l++)
//            slist << QString::fromStdString(list.at(l));

//        qDebug() << i << "\t" << slist.join("\t") << "\n";

//        qDebug() << i << "\tLength: " << pol->GetLength() << "\tArea : " << pol->GetArea() << "\n";
        qDebug() << i << "\t" << lblObject->GetPhysicalSize();
    }

    return list;
}

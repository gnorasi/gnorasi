#include <QDebug>
#include <QFileDialog>

#include "itiotblabelmapparser.h"

#include "itiotbregion.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

#include "itkShapeLabelObject.h"

using namespace itiviewer;
using namespace otb;

LabelMapParser::LabelMapParser(QObject *parent) :
    QObject(parent)
{
}


QList<Region*> LabelMapParser::parse(LabelMapType *lblmap){
    QList<Region*> list;

    QStringList slist;

    for(unsigned int i = 1; i < lblmap->GetNumberOfLabelObjects(); i++){
        LabelObjectType* lblObject = lblmap->GetLabelObject(i);

//        lblObject->GetCentroid();

//        PolygonType *pol = lblObject->GetPolygon();

//        QStringList slist;

//        std::vector<std::string> list = lblObject->GetAvailableAttributes();
//        for(int l = 0; l < list.size(); l++)
//            slist << QString::fromStdString(list.at(l));

//        qDebug() << i << "\t" << slist.join("\t") << "\n";

//        qDebug() << i << "\tLength: " << pol->GetLength() << "\tArea : " << pol->GetArea() << "\n";

         QString line = QString("%1\t%2\t%3").arg(QString::number(i)).arg(QString::number(lblObject->GetCentroid()[0],'f',2)).arg(QString::number(lblObject->GetCentroid()[1],'f',2));

        slist << line;
    }

    QString path = QFileDialog::getSaveFileName(0,QLatin1String("Save"),QDir::homePath());
    if(!path.contains(".csv"))
        path.append(".csv");

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
        return list;

    QTextStream out(&file);
    out << slist.join("\n");

    file.close();

    return list;
}

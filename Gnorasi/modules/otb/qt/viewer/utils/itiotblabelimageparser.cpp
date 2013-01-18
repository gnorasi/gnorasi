#include <QDebug>
#include <QFileDialog>

#include "itiotblabelimageparser.h"

#include "itiotbregion.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

#include "itkShapeLabelObject.h"

//#include "otbLabelImageToVectorDataFilter.h"

using namespace itiviewer;
using namespace otb;

LabelImageParser::LabelImageParser(QObject *parent) :
    QObject(parent)
{
}


QList<Region*> LabelImageParser::parse(LabelImageType *lblimage){
    QList<Region*> list;

    QStringList slist;


    typedef otb::LabelImageToVectorDataFilter<LabelImageType> LabelFilterType;

    LabelFilterType::Pointer filter = LabelFilterType::New();

    filter->SetInput(lblimage);
    filter->Update();



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

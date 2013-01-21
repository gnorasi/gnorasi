#include <QDebug>
#include <QFileDialog>

#include "itiotblabelmapparser.h"

#include "itiotbregion.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

#include "itkShapeLabelObject.h"
#include "itkPolyLineParametricPath.h"
#include "itkVectorContainer.h"

#include "itiotbimagemanager.h"

using namespace itiviewer;
using namespace otb;

LabelMapParser::LabelMapParser(QObject *parent) :
    QObject(parent)
{
}


QList<Region*> LabelMapParser::parse(LabelMapType *lblmap){
    QList<Region*> list;

    QStringList slist;

    const unsigned int VDimension = 2;
    typedef itk::ContinuousIndex<double,VDimension>    ContinuousIndexType;
    typedef itk::Index<  VDimension >                  IndexType;
    typedef itk::Offset< VDimension >                  OffsetType;
    typedef itk::Point<double,VDimension>              PointType;
    typedef itk::Vector<double,VDimension>             VectorType;
    typedef ContinuousIndexType                   VertexType;
    typedef itk::VectorContainer<unsigned, VertexType> VertexListType;

    double x = 0;
    double y = 0;

    int counter = 0;

    VectorImageType *img = ITIOTBIMAGEMANAGER->image();
    if(!img)
        return list;

    QList<QPolygon> pollist;

    for(unsigned int i = 1; i < lblmap->GetNumberOfLabelObjects(); i++){
        LabelObjectType* lblObject = lblmap->GetLabelObject(i);

        PolygonType *pol = lblObject->GetPolygon();

        const VertexListType *vList = pol->GetVertexList();

        VertexListType::const_iterator point = vList->begin();

        QPolygon plgon;

        while(point != vList->end())
        {
            ContinuousIndexType cit = *point;

            VectorIndexType index;

            bool val = img->TransformPhysicalPointToIndex(cit,index);

            if(val){
                x = index[0];
                y = index[1];

                plgon << QPoint(x,y);
            }

            point++;
        }

        pollist.append(plgon);

        Region *pRegion = new Region(this);
        pRegion->setArea(plgon);
        pRegion->setSegmentationId(counter++);

        list << pRegion;
    }

    QString path = QFileDialog::getSaveFileName(0,QLatin1String("Save"),QDir::homePath());
    if(!path.contains(".csv"))
        path.append(".csv");

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
        return list;

    QTextStream out(&file);
    out << LabelMapParser::contructTextFromPolygonList(pollist);

    file.close();

    return list;
}

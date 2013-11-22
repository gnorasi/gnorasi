#include <QDebug>
#include <QFileDialog>

#include "itiotblabelmapparser.h"

#include "itiotbregion.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

#include "itkShapeLabelObject.h"
#include "itkPolyLineParametricPath.h"
#include "itkVectorContainer.h"
#include "otbVectorData.h"
#include "otbLabelMapToVectorDataFilter.h"
#include "itkPreOrderTreeIterator.h"
#include "otbObjectList.h"
#include "otbPolygon.h"

#include "itiotbimagemanager.h"

#include "../../../../knowledge/qt/utils/ontologyclassificationmanager.h"
#include "../../../../knowledge/qt/models/ontologyclass.h"

using namespace itiviewer;
using namespace otb;

LabelMapParser::LabelMapParser(QObject *parent) :
    QObject(parent)
{
}


//QList<itiviewer::Region*> LabelMapParser::parse(LabelMapType *lblmap){

//    const unsigned int VDimension = 2;
//    typedef itk::ContinuousIndex<double,VDimension>    ContinuousIndexType;
//    typedef itk::Index<  VDimension >                  IndexType;
//    typedef itk::Offset< VDimension >                  OffsetType;
//    typedef itk::Point<double,VDimension>              PointType;
//    typedef itk::Vector<double,VDimension>             VectorType;
//    typedef ContinuousIndexType                   VertexType;
//    typedef itk::VectorContainer<unsigned, VertexType> VertexListType;

//    QList<Region*> list;

//    VectorImageType *img = m_pManager->image();
//    if(!img)
//        return list;

//    int x       = 0;
//    int y       = 0;
//    int counter = 0;

//    QList<QPolygon> pollist;

//    qDebug() << "number of labelobjects in labelmap : " << lblmap->GetNumberOfLabelObjects();

//    m_classLabelIdsNames.clear();
//    m_classLabelIdsNames[1001] = tr("Unclassified");

//    for(unsigned int i = 1; i < lblmap->GetNumberOfLabelObjects(); i++){
//        LabelObjectType* lblObject = lblmap->GetLabelObject(i);

//        PolygonType *pol = lblObject->GetPolygon();

//        QPolygon plgon;

//        const VertexListType *vList = pol->GetVertexList();
//        VertexListType::const_iterator point = vList->begin();
//        while(point != vList->end())
//        {
//            ContinuousIndexType cit = *point;

//            VectorIndexType index;

//            bool val = img->TransformPhysicalPointToIndex(cit,index);

//            if(val){

//                x = index[0];
//                y = index[1];

//                QPoint p(x,y);

//                plgon.append(p);
//            }

//            point++;
//        }

//        if(plgon.isEmpty())
//            continue;

//        LabelMapParser::validatePolygon(plgon);

//        pollist.append(plgon);

//        Region *pRegion = new Region();
//        pRegion->setArea(plgon);
//        pRegion->setSegmentationId(counter++);

//        if(lblObject->HasClassLabel()){

//            int classificationId = (int)lblObject->GetClassLabel();

//            if(!m_classLabelIdsNames.contains(classificationId)){

//                OntologyClass *pOntologyClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyByIdx(classificationId);
//                if(pOntologyClass){

//                    QString ontologyClassName = pOntologyClass->name();
//                    qDebug() << "found the ontology class name : " << ontologyClassName << " , ontology class id : " << classificationId;
//                    m_classLabelIdsNames[classificationId] = ontologyClassName;
//                }else
//                    qDebug() << "could not find the ontology class , ontology class id : " << classificationId;
//            }

//            pRegion->setClassificationId(classificationId);
//        }

//        list << pRegion;
//    }

//    qDebug() << "Number of classification ids : " << m_classLabelIdsNames.size();

//    //    QString path = QFileDialog::getSaveFileName(0,QLatin1String("Save"),QDir::homePath());
//    //    if(!path.contains(".csv"))
//    //        path.append(".csv");

//    //    QFile file(path);
//    //    if(!file.open(QIODevice::WriteOnly))
//    //        return list;

//    //    QTextStream out(&file);
//    //    out << LabelMapParser::contructTextFromPolygonList(pollist);

//    //    file.close();



//    return list;
//}


QList<itiviewer::Region*> LabelMapParser::parse(LabelMapType *lblmap){

    const unsigned int VDimension = 2;
    typedef itk::ContinuousIndex<double,VDimension>                             ContinuousIndexType;
    typedef ContinuousIndexType                                                 VertexType;
    typedef itk::VectorContainer<unsigned, VertexType>                          VertexListType;
    typedef double                                                              ValuePrecision;
    const int dimension = 2;
    typedef unsigned int                                                        PixelType;//pas le choix de prendre unsigned int comme type
    typedef otb::VectorData<PixelType, dimension, ValuePrecision>               VectorDataType;
    typedef otb::LabelMapToVectorDataFilter<LabelMapType, VectorDataType>       LabelMapToVectorDataFilterType;
    typedef VectorDataType::DataTreeType                                        DataTreeType;
    typedef itk::PreOrderTreeIterator<DataTreeType>                             TreeIteratorType;
    typedef otb::Polygon<double>                                                PolygonType;
    typedef PolygonType::VertexListConstIteratorType                            PolygonIteratorType;

    int x       = 0;
    int y       = 0;
    int counter = 0;

    QList<Region*> list;
    QList<QPolygon> pollist;

    m_classLabelIdsNames.clear();
    m_classLabelIdsNames[1001] = tr("Unclassified");


    for(unsigned int i = 1; i < lblmap->GetNumberOfLabelObjects(); i++){

        LabelObjectType* lblObject = lblmap->GetLabelObject(i);

        if(lblObject->HasClassLabel()){

            int classificationId = (int)lblObject->GetClassLabel();

            if(!m_classLabelIdsNames.contains(classificationId)){

                OntologyClass *pOntologyClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyByIdx(classificationId);
                if(pOntologyClass){

                    QString ontologyClassName = pOntologyClass->name();
                    qDebug() << "found the ontology class name : " << ontologyClassName << " , ontology class id : " << classificationId;
                    m_classLabelIdsNames[classificationId] = ontologyClassName;
                }else
                    qDebug() << "could not find the ontology class , ontology class id : " << classificationId;
            }

            Region *pRegion = new Region();
            pRegion->setClassificationId(classificationId);
            pRegion->setSegmentationId(counter++);

            list << pRegion;
        }else
            qDebug() << "\n HasClassLabel returns false \n";
    }

    LabelMapToVectorDataFilterType::Pointer LabelMapToVectorDataFilter = LabelMapToVectorDataFilterType::New();
    LabelMapToVectorDataFilter->SetInput(lblmap);
    LabelMapToVectorDataFilter->Update();

    VectorDataType *vdt = LabelMapToVectorDataFilter->GetOutput();

    counter = 0;
    TreeIteratorType it(vdt->GetDataTree());
    it.GoToBegin();
    while (!it.IsAtEnd())
    {
        if (it.Get()->IsPolygonFeature())
        {

            PolygonType::Pointer pt = it.Get()->GetPolygonExteriorRing();

            QPolygon plgon;
            const VertexListType *vList = pt->GetVertexList();
            VertexListType::const_iterator point = vList->begin();
            while(point != vList->end())
            {
                ContinuousIndexType cit = *point;

                x = cit[0];
                y = cit[1];

                QPoint p(x,y);

                plgon.append(p);

                point++;
            }

//            pollist.append(plgon);

            LabelMapParser::validatePolygon(plgon);

            if(counter < list.size()){

                Region *pRegion = list.at(counter);
                pRegion->setArea(plgon);

                qDebug() << plgon;

                counter++;
            }
        }else{
            qDebug() << "\n IsPolygonFeature returns false \n";
        }

        ++it;
    }



    return list;
}

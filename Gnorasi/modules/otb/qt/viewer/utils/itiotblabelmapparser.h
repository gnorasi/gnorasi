#ifndef ITIOTBLABELMAPPARSER_H
#define ITIOTBLABELMAPPARSER_H

#include <QtCore>
#include <QtGui>
#include <QObject>
#include <QPolygon>
#include <QStringList>

#include "../../../ports/otblabelmapport.h"

//#include "itkShapeLabelObject.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

using namespace voreen;

namespace itiviewer {

class Region;
class ItiOtbImageManager;

class LabelMapParser : public QObject
{
    Q_OBJECT
public:
    explicit LabelMapParser(QObject *parent = 0);

    typedef unsigned long                         LabelType;
    typedef otb::Polygon<double> PolygonType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType>        LabelMapType;
    
    /*!
     * \brief parse
     * \param lblmap
     * \return
     */
    QList<itiviewer::Region*> parse(LabelMapType* lblmap);


    QHash<int,QString> classLabelIdsNames() const {return m_classLabelIdsNames; }


    void setManager(ItiOtbImageManager *manager) { m_pManager = manager; }

signals:
    
public slots:
    
private:
    static inline QString contructTextFromPolygon(const QPolygon &pol);

    static inline QString contructTextFromPolygonList(const QList<QPolygon> &list);

    static inline bool validatePolygon(QPolygon &pol);

    QHash<int,QString> m_classLabelIdsNames;

    ItiOtbImageManager *m_pManager;

};

QString LabelMapParser::contructTextFromPolygon(const QPolygon &pol){
    QStringList list;

    QPolygon::const_iterator i;
    for(i = pol.constBegin(); i != pol.constEnd(); i ++){
        QPointF point = *i;
        list << QString("(%1 %2)").arg(QString::number(point.x())).arg(QString::number(point.y()));
    }

    return list.join(",");
}

QString LabelMapParser::contructTextFromPolygonList(const QList<QPolygon> &list){
    QStringList textList;

    QList<QPolygon>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        QPolygon pol = *i;
        QString text = LabelMapParser::contructTextFromPolygon(pol);

        textList << text;
    }

    return textList.join("\n");
}

bool LabelMapParser::validatePolygon(QPolygon &pol){
    QList<QPoint> helperlist;

    QPolygon::const_iterator i;

    for(i = pol.constBegin(); i != pol.constEnd(); i++){
        QPoint p = *i;

        if(!helperlist.contains(p))
            helperlist << p;
    }

    pol.clear();
    QList<QPoint>::const_iterator j;
    for(j = helperlist.constBegin(); j != helperlist.constEnd(); j++){
        pol << *j;
    }

    return true;
}

} // end of namespace itiviewer

#endif // ITIOTBLABELMAPPARSER_H

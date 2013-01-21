#ifndef ITIOTBLABELMAPPARSER_H
#define ITIOTBLABELMAPPARSER_H

#include <QObject>
#include <QPolygon>
#include <QStringList>

#include "../../../ports/otblabelmapport.h"

//#include "itkShapeLabelObject.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

using namespace voreen;

namespace itiviewer {

class Region;

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
    QList<Region*> parse(LabelMapType* lblmap);

signals:
    
public slots:
    
private:
    static inline QString contructTextFromPolygon(const QPolygon &pol);

    static inline QString contructTextFromPolygonList(const QList<QPolygon> &list);

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

} // end of namespace itiviewer

#endif // ITIOTBLABELMAPPARSER_H

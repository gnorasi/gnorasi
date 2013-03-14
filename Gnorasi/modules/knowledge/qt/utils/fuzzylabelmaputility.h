#ifndef FUZZYLABELMAPUTILITY_H
#define FUZZYLABELMAPUTILITY_H

#include <QtCore>
#include <QtGui>
#include <QObject>
#include <QPolygon>
#include <QStringList>

#include "../../../ports/otblabelmapport.h"

#include "otbAttributesMapLabelObjectWithClassLabel.h"

using namespace voreen;

class FuzzyLabelMapUtility : public QObject
{
    Q_OBJECT
public:
    explicit FuzzyLabelMapUtility(QObject *parent = 0);

    typedef unsigned long                         LabelType;
    typedef otb::Polygon<double> PolygonType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType>        LabelMapType;
    
    /*!
     * \brief parse
     * \param lblmap
     * \return
     */
    void parse(LabelMapType* lblmap);

    QStringList getAttributeListNames() const { return m_attributeList; }

    /*!
     * \brief calculateValues
     */
    void calculateValues(LabelMapType *lblMap, const QString& );

    QString constructCsvFromLabelMap(LabelMapType *lblMap);


signals:
    
public slots:

    void updateMinValue(double a) { m_a = a; }

    void updateMaxValue(double b) { m_b = b; }
    
private:
    static inline QString contructTextFromPolygon(const QPolygon &pol);

    static inline QString contructTextFromPolygonList(const QList<QPolygon> &list);

    static inline bool validatePolygon(const QPolygon &pol);

    QStringList m_attributeList;

    double m_a;
    double m_b;

};

inline double calculateValue(double val, double a , double b){
    if(val <= a)
        return 0.0;
    else if(val >= b)
        return 1.0;
    else
        return (val -a)/(b-a);
}

QString FuzzyLabelMapUtility::contructTextFromPolygon(const QPolygon &pol){
    QStringList list;

    QPolygon::const_iterator i;
    for(i = pol.constBegin(); i != pol.constEnd(); i ++){
        QPointF point = *i;
        list << QString("(%1 %2)").arg(QString::number(point.x())).arg(QString::number(point.y()));
    }

    return list.join(",");
}

QString FuzzyLabelMapUtility::contructTextFromPolygonList(const QList<QPolygon> &list){
    QStringList textList;

    QList<QPolygon>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        QPolygon pol = *i;
        QString text = FuzzyLabelMapUtility::contructTextFromPolygon(pol);

        textList << text;
    }

    return textList.join("\n");
}

bool FuzzyLabelMapUtility::validatePolygon(const QPolygon &pol){
    QList<QPoint> helperlist;

    QPolygon::const_iterator i;

    for(i = pol.constBegin(); i != pol.constEnd(); i++){
        QPoint p = *i;

        if(helperlist.contains(p))
            return false;

        helperlist.append(p);
    }

    return true;
}

#endif // FUZZYLABELMAPUTILITY_H

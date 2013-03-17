#include "itiotbregion.h"

namespace itiviewer {

Region::Region(QObject *parent) :
    m_segmentationId(1),
    m_classificationId(1001),
    QObject(parent)
{
//    int r = randInt(0,255);
//    int g = randInt(0,255);
//    int b = randInt(0,255);
//    m_color = QColor(r,g,b);
    m_color = Qt::transparent;
    m_brush = QBrush(m_color);

    m_pen = QPen(QBrush(Qt::black),2.0);
}

void Region::drawRegion(QPainter *painter, ImageRegionType &extent, const QRectF &rect, double iz){
    modifyPolygonByExtent(extent, rect, iz);

    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->setOpacity(0.9);

    painter->drawPolygon(m_paintedArea);
}


void Region::setArea(QPolygon p){
    m_area = p;
    m_paintedArea = p;
}

void Region::modifyPolygonByExtent(ImageRegionType &extent, const QRectF &rect, double iz){

    QPolygon::const_iterator i;
    for(i = m_area.constBegin(); i != m_area.constEnd(); i++){
        QPoint point = *i;

        int idx = m_area.indexOf(point);

        QPoint hp;

        int helperx = (point.x() - rect.x())*iz;
        int helpery = (point.y() - rect.y())*iz;

        hp.setX(helperx);
        hp.setY(helpery);

        if(extent.GetIndex()[0] > 0)
            hp.setX(hp.x()+extent.GetIndex()[0]);

        if(extent.GetIndex()[1] > 0)
            hp.setY(hp.y()+extent.GetIndex()[1]);

//        hp.setX(point.x()*iz + (int)extent.GetIndex()[0]);
//        hp.setY(point.y()*iz + (int)extent.GetIndex()[1]);

        m_paintedArea.replace(idx,hp);
    }
}

}

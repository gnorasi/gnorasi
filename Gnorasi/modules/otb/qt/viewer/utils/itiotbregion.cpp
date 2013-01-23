#include "itiotbregion.h"

namespace itiviewer {

Region::Region(QObject *parent) :
    m_segmentationId(1),
    m_classificationId(1),
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

void Region::drawRegion(QPainter *painter, ImageRegionType &extent, double iz){
    modifyPolygonByExtent(extent,iz);

    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->setOpacity(0.3);

    painter->drawPolygon(m_paintedArea);
}


void Region::setArea(QPolygon p){
    m_area = p;
    m_paintedArea = p;
}

void Region::modifyPolygonByExtent(ImageRegionType &extent, double iz){

    QPolygon::const_iterator i;
    for(i = m_area.constBegin(); i != m_area.constEnd(); i++){
        QPoint point = *i;

        int idx = m_area.indexOf(point);

        QPoint hp;

        hp.setX(point.x()*iz + (int)extent.GetIndex()[0]);
        hp.setY(point.y()*iz + (int)extent.GetIndex()[1]);

        if(hp.x() < (int)extent.GetIndex()[0]){
            qDebug() << "wtf!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        }

        m_paintedArea.replace(idx,hp);
    }
}

}

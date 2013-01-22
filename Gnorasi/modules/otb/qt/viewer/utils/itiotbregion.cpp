#include "itiotbregion.h"

using namespace itiviewer;

Region::Region(QObject *parent) :
    QObject(parent)
{
}

void Region::drawRegion(QPainter *painter, ImageRegionType &extent, double iz){
    modifyPolygonByExtent(extent,iz);

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

        m_paintedArea.replace(idx,hp);
    }
}

#include "itiotbregion.h"

using namespace itiviewer;

Region::Region(QObject *parent) :
    QObject(parent)
{
}

void Region::drawRegion(QPainter *painter, ImageRegionType &extent){
    modifyPolygonByExtent(extent);
    //!
    //! TODO
    //! add painting functionality here
    //!
    painter->drawPolygon(m_paintedArea);
}


void Region::setArea(QPolygon p){
    m_area = p;
    m_paintedArea = p;
}

void Region::modifyPolygonByExtent(ImageRegionType &extent){

    QPolygon::const_iterator i;
    for(i = m_area.constBegin(); i != m_area.constEnd(); i++){
        QPoint point = *i;

        int idx = m_area.indexOf(point);

        QPoint hp;

        hp.setX(point.x() + (int)extent.GetIndex()[0]);
        hp.setY(point.y() + (int)extent.GetIndex()[1]);

        m_paintedArea.replace(idx,hp);
    }
}

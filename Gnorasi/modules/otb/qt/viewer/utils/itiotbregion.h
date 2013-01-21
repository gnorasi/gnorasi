#ifndef ITIOTBREGION_H
#define ITIOTBREGION_H

#include <QObject>
#include <QPolygonF>
#include <QColor>

#include <QPainter>

#include "../vector_globaldefs.h"

namespace itiviewer {

/*!
 * \brief The Region class
 */
class Region : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int          segmentationId      READ segmentationId     WRITE setSegmentationId     NOTIFY  segmentationIdChanged)
    Q_PROPERTY(int          classificationId    READ classificationId   WRITE setClassificationId   NOTIFY  classificationIdChanged)
    Q_PROPERTY(QPolygon     area                READ area               WRITE setArea               NOTIFY  areaChanged)
    Q_PROPERTY(QColor       color               READ color              WRITE setColor              NOTIFY  colorChanged)
public:
    /*!
     * \brief Region
     * \param parent
     */
    explicit    Region(QObject *parent = 0);

    int         segmentationId() const {return m_segmentationId;}
    void        setSegmentationId( int i) { m_segmentationId = i ; }
    
    int         classificationId() const { return m_classificationId; }
    void        setClassificationId(int i) { m_classificationId = i; }

    QPolygon    area() const { return m_area; }
    void        setArea(QPolygon p);

    QColor      color() const { return m_color; }
    void        setColor(QColor c) { m_color = c; }

    void        drawRegion(QPainter * painter, ImageRegionType &extent);

signals:
    void        segmentationIdChanged();
    void        classificationIdChanged();
    void        areaChanged();
    void        colorChanged();

public slots:

private:
    void        modifyPolygonByExtent(ImageRegionType &);

    int         m_segmentationId;
    int         m_classificationId;
    QPolygon    m_area;
    QColor      m_color;

    QPolygon    m_paintedArea;
    
};

} // using namesoace itiviewer

#endif // ITIOTBREGION_H

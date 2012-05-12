/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

//#include "annotationfencegraphicsitem.h"
//#include "networkeditor_common.h"
//
//#include <QGraphicsScene>
//#include <QGraphicsSceneMouseEvent>
//#include <QPainter>
//#include <QStyleOptionGraphicsItem>
//
//namespace voreen {
//
//namespace {
//    QPen borderPen = QPen(Qt::black, 1);
//
//    // the normal background color
//    const QColor backgroundColor = QColor(255,255,255,128);
//    // the default color
//    const QColor standardColor = QColor(Qt::darkYellow).darker();
//    // the normal hovering color
//    const QColor hoverColor = QColor(Qt::darkGray).darker().darker()/* QColor(44,44,114,255)*/;
//    const QColor glossColor = QColor(255,255,255,128);
//
//
//    // size of the WidgetIndicator
//    const QSize size = QSize(8, 8);
//}
//
//AnnotationFenceHandleGraphicsItem::AnnotationFenceHandleGraphicsItem(QGraphicsItem* parent)
//#if (QT_VERSION >= 0x040600)
//    : QGraphicsObject(parent)
//#else
//    : QObject()
//    , QGraphicsItem(parent)
//#endif
//{
//    setFlag(ItemIsMovable);
//    setAcceptsHoverEvents(true);
//    //setZValue(ZValueAnnotationFenceHandleGraphicsItem);
//    //setFlag(QGraphicsItem::ItemStacksBehindParent, false);
//}
//
//void AnnotationFenceHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
//    //QGraphicsItem* s = parentItem();
//    //if (mapToScene(e->pos()).x() > mapToScene(parentObject()->pos()).x() && mapToScene(e->pos()).y() > mapToScene(parentObject()->pos()).y()) {
//    //if (e->scenePos().x() > mapToScene(parentObject()->pos()).x() && e->scenePos().y() > mapToScene(parentObject()->pos()).y()) {
//    //std::cout << "(" << e->scenePos().x() << "," << e->scenePos().y() << ")   ||  (" << mapToScene(parentObject()->pos()).x() <<
//    //    "," << mapToScene(parentObject()->pos()).y() << ")" << std::endl;
//        QPointF modPos = mapToParent(e->pos()) - QPointF(size.width(), size.height());
//        setPos(modPos);
//        emit sizeChanged(mapToParent(e->pos()));
//    //}
//}
//
//QRectF AnnotationFenceHandleGraphicsItem::boundingRect() const {
//    return QRect(0, 0, size.width(), size.height());
//}
//
//void AnnotationFenceHandleGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/) {
//    // add gloss effect
//    QRectF glossArea = boundingRect();
//    glossArea.setLeft(glossArea.left() + glossArea.width() * 0.05);
//    glossArea.setHeight(glossArea.height() * 0.5);
//    glossArea.setWidth(glossArea.width() * 0.9);
//    painter->setPen(QPen(QColor(0,0,0,0), 0));
//    painter->setBrush(glossColor);
//    painter->drawEllipse(glossArea);
//
//    painter->setPen(borderPen);
//    painter->setBrush(standardColor);
//
//    if (option->state & QStyle::State_MouseOver)
//        painter->setBrush(hoverColor);
//
//    painter->drawEllipse(boundingRect());
//}
//
//AnnotationFenceGraphicsItem::AnnotationFenceGraphicsItem(QGraphicsItem* parent)
//#if (QT_VERSION >= 0x040600)
//    : QGraphicsObject(parent)
//#else
//    : QObject()
//    , QGraphicsItem(parent)
//#endif
//    , handle_(parent)
//{
//    connect(&handle_, SIGNAL(sizeChanged(const QPointF&)), this, SLOT(sizeHasChanged(const QPointF&)));
//    setZValue(ZValueAnnotationFenceGraphicsItem);
//    setFlag(QGraphicsItem::ItemStacksBehindParent);
//}
//
//QRectF AnnotationFenceGraphicsItem::boundingRect() const {
//    return QRect(-4, -4, llr_.x(), llr_.y());
//}
//
//void AnnotationFenceGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/) {
//    QPen pen;
//    pen.setStyle(Qt::DashLine);
//    pen.setWidth(3);
//    painter->setPen(pen);
//    painter->drawRect(boundingRect());
//}
//
//void AnnotationFenceGraphicsItem::sizeHasChanged(const QPointF& position) {
//    llr_ = position;
//    emit sizeChanged(position);
//    scene()->invalidate();
//}
//
//void AnnotationFenceGraphicsItem::setPosition(const QPointF& lowerRight) {
//    llr_ = lowerRight;
//    handle_.setPos(lowerRight - QPointF(8,8));
//    //scene()->invalidate();
//}

//} // namespace

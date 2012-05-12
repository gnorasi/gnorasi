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

#ifndef VRN_ANNOTATIONFENCEGRAPHICSITEM_H
#define VRN_ANNOTATIONFENCEGRAPHICSITEM_H

//#include <QGraphicsItem>
//#include <QObject>
//
//namespace voreen {
//
//class AnnotationFenceGraphicsItem;
//
//#if (QT_VERSION >= 0x040600)
//class AnnotationFenceHandleGraphicsItem : public QGraphicsObject {
//#else
//class AnnotationFenceHandleGraphicsItem : public QObject, public QGraphicsItem {
//#endif
//Q_OBJECT
//public:
//    AnnotationFenceHandleGraphicsItem(QGraphicsItem* parent);
//
//    QRectF boundingRect() const;
//
//protected:
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
//    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
//
//signals:
//    void sizeChanged(const QPointF& position);
//};
//
//
//#if (QT_VERSION >= 0x040600)
//class AnnotationFenceGraphicsItem : public QGraphicsObject {
//#else
//class AnnotationFenceGraphicsItem : public QObject, public QGraphicsItem {
//#endif
//Q_OBJECT
//public:
//    AnnotationFenceGraphicsItem(QGraphicsItem* parent);
//
//    QRectF boundingRect() const;
//
//    void setPosition(const QPointF& lowerRight);
//
//private slots:
//    void sizeHasChanged(const QPointF& position);
//
//signals:
//    void sizeChanged(const QPointF& position);
//
//protected:
//    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
//
//private:
//    AnnotationFenceHandleGraphicsItem handle_;
//    QPointF llr_;
//};
//
//
//} // namespace

#endif // VRN_ANNOTATIONFENCEGRAPHICSITEM_H

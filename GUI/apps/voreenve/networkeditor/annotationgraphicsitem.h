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

#ifndef VRN_ANNOTATIONGRAPHICSITEM_H
#define VRN_ANNOTATIONGRAPHICSITEM_H

//#include "annotationfencegraphicsitem.h"
#include "annotationclosebutton.h"
#include "networkeditor_common.h"
#include "textgraphicsitem.h"

#include <QObject>
#include <QGraphicsItem>

namespace voreen {

class NetworkAnnotation;

#if (QT_VERSION >= 0x040600)
class AnnotationGraphicsItem : public QGraphicsObject {
#else
class AnnotationGraphicsItem : public QObject, public QGraphicsItem {
#endif
Q_OBJECT

public:
    AnnotationGraphicsItem(NetworkAnnotation* annotation);
    //~AnnotationGraphicsItem();

    enum { Type = UserType + UserTypesAnnotationGraphicsItem };
    int type() const;
    QRectF boundingRect() const;

    NetworkAnnotation* getAnnotation() const;

    void enterEditMode();

signals:
    void closeButtonPressed();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);

private slots:
    void editFinished();
    void nameChanged();
    //void fenceChanged(const QPointF& position);

private:
    TextGraphicsItem textItem_;
    AnnotationCloseButton closeButton_;
    //AnnotationFenceGraphicsItem fence_;
    NetworkAnnotation* annotation_;
};

} // namespace

#endif // VRN_ANNOTATIONGRAPHICSITEM_H

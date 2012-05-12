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

#include "annotationgraphicsitem.h"

#include "voreen/core/io/serialization/meta/networkannotation.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionGraphicsItem>

namespace voreen {

namespace {
    const QColor background = QColor(234, 224, 136, 255);
    const QColor backgroundTop = QColor(210, 200, 115, 255);
    const QColor backgroundTopSelected = QColor(255, 170, 85, 255);
    qreal topSize = 15.0;
}

AnnotationGraphicsItem::AnnotationGraphicsItem(NetworkAnnotation* annotation)
#if (QT_VERSION >= 0x040600)
    : QGraphicsObject(0)
#else
    : QObject()
    , QGraphicsItem(0)
#endif
    , textItem_(QString::fromStdString(annotation->getText()), this)
    //, fence_(this)
    , closeButton_(this)
    , annotation_(annotation)
{
    tgtAssert(annotation, "passed null pointer");

#if (QT_VERSION >= 0x040600)
    setFlag(ItemSendsScenePositionChanges);
#endif
    
    textItem_.setDefaultTextColor(Qt::black);

    closeButton_.setPos(boundingRect().width() - closeButton_.boundingRect().width() - 7, -19);
    connect(&closeButton_, SIGNAL(pressed()), this, SIGNAL(closeButtonPressed()));

    connect(&textItem_, SIGNAL(renameFinished()), this, SLOT(editFinished()));
    connect(&textItem_, SIGNAL(textChanged()), this, SLOT(nameChanged()));
    //connect(&fence_, SIGNAL(sizeChanged(const QPointF&)), this, SLOT(fenceChanged(const QPointF&)));

    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setAcceptsHoverEvents(true);
    setZValue(ZValueNoteGraphicsItem);

    setPos(annotation->getPosition().first, annotation->getPosition().second);

    //if (annotation->hasFence())
    //    fence_.setPosition(QPointF(annotation->getFencePosition().first, annotation->getFencePosition().second));
    //else
    //    fence_.setPosition(textItem_.boundingRect().bottomRight());
}

//AnnotationGraphicsItem::~AnnotationGraphicsItem() {
//    scene()->removeItem(&fence_);
//}


int AnnotationGraphicsItem::type() const {
    return Type;
}

QRectF AnnotationGraphicsItem::boundingRect() const {
    QRectF rect = textItem_.boundingRect().adjusted(-5.0, -20.0, 5.0, 5.0);
    if (rect.width() < 100.0)
        rect.setWidth(100.0);
    return rect;
}

NetworkAnnotation* AnnotationGraphicsItem::getAnnotation() const {
    return annotation_;
}

void AnnotationGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/) {
    painter->setBrush(background);
    painter->drawRect(boundingRect());

    if (option->state & QStyle::State_Selected)
        painter->setBrush(backgroundTopSelected);
    else
        painter->setBrush(backgroundTop);
    painter->drawRect(QRectF(boundingRect().topLeft(), boundingRect().topRight() + QPointF(0, topSize)));

    QColor lineColor(Qt::darkYellow);
    lineColor = lineColor.darker();
    painter->setPen(QPen(lineColor));
    painter->drawLine(boundingRect().topLeft() + QPointF(0,topSize), boundingRect().topRight() + QPointF(0, topSize));
}

void AnnotationGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    annotation_->setPosition(pos().x(), pos().y());
    QGraphicsItem::mouseMoveEvent(e);
}

void AnnotationGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    enterEditMode();
    QGraphicsItem::mouseDoubleClickEvent(e);
}

void AnnotationGraphicsItem::enterEditMode() {
    textItem_.setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem_.setFlag(QGraphicsItem::ItemIsFocusable, true);
    textItem_.setFocus();
}

void AnnotationGraphicsItem::editFinished() {
    textItem_.setTextInteractionFlags(Qt::NoTextInteraction);
    textItem_.setFlag(QGraphicsItem::ItemIsFocusable, false);
    textItem_.setFlag(QGraphicsItem::ItemIsSelectable, false);
    annotation_->setText(textItem_.toPlainText().toStdString());
    nameChanged();
}

QVariant AnnotationGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemSelectedChange || change == ItemScenePositionHasChanged) {
        if (!value.toBool() && (textItem_.textInteractionFlags() & Qt::TextEditorInteraction))
            // item is deselected and in renaming mode -> finish renaming
            editFinished();
    }

    return QGraphicsItem::itemChange(change, value);
}


void AnnotationGraphicsItem::nameChanged() {
    prepareGeometryChange();

    closeButton_.setPos(boundingRect().width() - closeButton_.boundingRect().width() - 7, -19);
    
    if (scene())
        scene()->invalidate();
}

//void AnnotationGraphicsItem::fenceChanged(const QPointF& position) {
//    annotation_->setFencePosition(position.x(), position.y());
//}


} // namespace

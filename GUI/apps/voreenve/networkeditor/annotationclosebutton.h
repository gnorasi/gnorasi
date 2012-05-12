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

#ifndef VRN_ANNOTATIONCLOSEBUTTON_H
#define VRN_ANNOTATIONCLOSEBUTTON_H

#include <QObject>
#include <QGraphicsItem>

#include "networkeditor_common.h"

namespace voreen {

#if (QT_VERSION >= 0x040600)
class AnnotationCloseButton : public QGraphicsObject {
#else
class AnnotationCloseButton : public QObject, public QGraphicsItem {
#endif
Q_OBJECT
public:
    /**
     * Constructor of an instance. Sets the item to accept hover events and sets a tooltip text
     * \param parent The parent item. Might be 0
     */
    AnnotationCloseButton(QGraphicsItem* parent);

    /**
     * The bounding rect of this PortGraphicsItem. \sa QGraphicsItem::boundingRect()
     * \return The bounding rect
     */
    QRectF boundingRect() const;

    /// The type of this QGraphicsItem subclass
    enum {Type = UserType + UserTypesAnnotationCloseButton};

    /**
     * Returns the type of this class. Necessary for all QGraphicsItem subclasses.
     * \return The type of this class
     */
    int type() const;

signals:
    /// This signal will be emitted if this item was been clicked upon
    void pressed();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

} // namespace

#endif // VRN_ANNOTATIONCLOSEBUTTON_H

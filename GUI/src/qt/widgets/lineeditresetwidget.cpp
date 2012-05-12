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

#include "voreen/qt/widgets/lineeditresetwidget.h"

#include <QStyle>
#include <QToolButton>

namespace voreen {

LineEditResetWidget::LineEditResetWidget(QWidget* parent)
    : QLineEdit(parent)
{
    clearButton_ = new QToolButton(this);
    QPixmap pixmap(":/voreenve/icons/input-reset.png");
    clearButton_->setIcon(QIcon(pixmap));
    clearButton_->setStyleSheet("QToolButton { border: none; padding: 3px; }");
    clearButton_->setCursor(Qt::ArrowCursor);
    clearButton_->hide();
    clearButton_->move(20,0);
    connect(clearButton_, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateClearButton(const QString&)));
}

void LineEditResetWidget::resizeEvent(QResizeEvent*){
    QSize size = clearButton_->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    clearButton_->move(rect().right()- frameWidth - size.width(), (rect().bottom() + 1 - size.height())/2);
}

void LineEditResetWidget::updateClearButton(const QString &text){
    clearButton_->setVisible(!text.isEmpty());
}

} //namespace voreen

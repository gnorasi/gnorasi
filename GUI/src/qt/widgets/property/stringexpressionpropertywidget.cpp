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

#include "voreen/qt/widgets/property/stringexpressionpropertywidget.h"

//#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "voreen/qt/widgets/customlabel.h"
#include "voreen/core/utils/stringconversion.h"

namespace voreen {

StringExpressionPropertyWidget::StringExpressionPropertyWidget(StringExpressionProperty* prop, QWidget* parent)
    : QPropertyWidget(prop, parent)
    , property_(prop)
{

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    addLayout(verticalLayout);

    QHBoxLayout* topLayout = new QHBoxLayout();
    verticalLayout->addLayout(topLayout);

    // label
    nameLabel_ = new CustomLabel(prop_->getGuiName().c_str(), const_cast<StringExpressionPropertyWidget*>(this), 
        const_cast<StringExpressionPropertyWidget*>(this), 0, false, true);
    nameLabel_->setMinimumWidth(80);
    //nameLabel_->setWordWrap(true);
    QFontInfo fontInfo(font());
    nameLabel_->setFont(QFont(fontInfo.family(), fontSize_));
    nameLabel_->setToolTip("id: " + QString::fromStdString(prop_->getID()));
    nameLabel_->setVisible(isVisible());
    nameLabel_->setEnabled(isEnabled());
    topLayout->addWidget(nameLabel_, 2);

    //comboBox
    comboBox_ = new QComboBox();
    comboBox_->setMaximumWidth(200);
    topLayout->addWidget(comboBox_, 3);

    //text field
    textEdit_ = new QTextEdit(QString(prop->get().c_str()));
    verticalLayout->addWidget(textEdit_);

    connect(textEdit_, SIGNAL(textChanged()), this, SLOT(setProperty()));
    connect(textEdit_, SIGNAL(textChanged()), this, SIGNAL(widgetChanged()));
    
    //QFontInfo fontInfo(font());
    comboBox_->setFont(QFont(fontInfo.family(), QPropertyWidget::fontSize_));

    //connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setProperty(std::string)));
    connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(clicked()));

    //button
    //button_ = new QPushButton("Add");

    //connect(button_, SIGNAL(clicked()), this, SLOT(clicked()));
    //connect(button_, SIGNAL(clicked(void)), this, SIGNAL(widgetChanged(void)));
    //addWidget(button_);
    //button_->setFont(QFont(fontInfo.family(), QPropertyWidget::fontSize_));

    //update and make visible
    updateFromProperty();

    addVisibilityControls();
}

StringExpressionPropertyWidget::~StringExpressionPropertyWidget() {
}

void StringExpressionPropertyWidget::updateFromProperty() {
    // Update lineEdit
    /*lineEdit_->blockSignals(true);
    lineEdit_->setText(QString::fromStdString(property_->get()));
    lineEdit_->blockSignals(false);*/
    textEdit_->blockSignals(true);
    QString st = QString::fromStdString(property_->get());
    if(st != textEdit_->toPlainText())
        textEdit_->setText(st);
    textEdit_->blockSignals(false);

    //update ComboBox
    comboBox_->blockSignals(true);
    comboBox_->clear();
    std::vector<std::string> entries = property_->getGuiStrings();
    std::vector<std::string>::const_iterator iter;
    for (iter = entries.begin(); iter != entries.end(); ++iter) {
        comboBox_->addItem(QString::fromStdString(*iter), QString::fromStdString(property_->getPlaceHolder(*iter)));
    }
    comboBox_->blockSignals(false);
}

void StringExpressionPropertyWidget::setProperty() {
    //QCursor cursor = textEdit_->cursor();

    if (!disconnected_) {
        property_->set(trim(strReplaceAll(textEdit_->document()->toPlainText().toStdString(), "\r\n", "\n")));
        emit modified();
    }
    else
        updateFromProperty();

    //textEdit_->setCursor(cursor);
    //textEdit_->setCursor(QTextCursor::M, false);
}

void StringExpressionPropertyWidget::clicked() {
    if ((!disconnected_) && (comboBox_->currentIndex() != 0))
        property_->clicked(comboBox_->currentText().toStdString());
}

CustomLabel* StringExpressionPropertyWidget::getNameLabel() const {
    // prevents name label from being shown left of the property widget
    return 0;
}

} // namespace

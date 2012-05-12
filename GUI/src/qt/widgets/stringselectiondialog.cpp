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

#include "voreen/qt/widgets/stringselectiondialog.h"

#include <QDialogButtonBox>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace voreen {

StringSelectionDialog::StringSelectionDialog(QStringList& resultList, const QStringList& initialList, QWidget* parent)
    : QDialog(parent)
    , resultList_(resultList)
    , initialList_(initialList)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    treeWidget_ = new QTreeWidget;
    treeWidget_->setHeaderHidden(true);
    treeWidget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    layout->addWidget(treeWidget_);

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(buttonBox, 0, Qt::AlignRight);

    foreach (const QString& str, initialList) {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(str));
        treeWidget_->addTopLevelItem(item);
    }
}

void StringSelectionDialog::accept() {
    QList<QTreeWidgetItem*> items = treeWidget_->selectedItems();
    foreach (QTreeWidgetItem* item, items) {
        resultList_.append(item->text(0));
    }
    QDialog::accept();
}


} // namespace

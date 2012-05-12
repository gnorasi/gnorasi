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

#ifndef VRN_DOCUMENTATIONWIDGET_H
#define VRN_DOCUMENTATIONWIDGET_H

#include <QWidget>
#include <QMap>
class QLabel;
class QTextEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QBoxLayout;

namespace voreen {

class Processor;
class Property;
class VoreenModule;

class DocumentationWidget : public QWidget {
Q_OBJECT
public:
    DocumentationWidget(QWidget* parent = 0);
    void initWithProcessor(const Processor* processor);
    void initWithModule(const VoreenModule* module);

signals:
    void finishedEditing();

private slots:
    void saveDescription();
    void propertyTreeSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void portTreeSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

private:
    void setTitleLabel(const std::string& title);

    QBoxLayout* layout_;
    QLabel* titleLabel_;

    QLabel* descriptionTextLabel_;
    QTextEdit* descriptionEdit_;

    QLabel* propertyTextLabel_;
    QWidget* propertyContainer_;
    QTreeWidget* propertyTree_;
    QTextEdit* propertyEdit_;
    QMap<QTreeWidgetItem*, std::string> propertyIDMap_;

    QLabel* portTextLabel_;
    QWidget* portContainer_;
    QTreeWidget* portTree_;
    QTextEdit* portEdit_;

    QMap<QTreeWidgetItem*, std::string> propertyValueMap_;
    QMap<QTreeWidgetItem*, std::string> portValueMap_;

    const Processor* processor_;
    const VoreenModule* module_;
};

} // namespace

#endif // VRN_DOCUMENTATIONWIDGET_H

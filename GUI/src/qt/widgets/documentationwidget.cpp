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

#include "voreen/qt/widgets/documentationwidget.h"

#include "voreen/qt/voreenapplicationqt.h"
#include "voreen/core/processors/processor.h"
#include "voreen/core/ports/port.h"
#include "voreen/core/properties/property.h"
#include "voreen/core/voreenmodule.h"
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

namespace voreen {

DocumentationWidget::DocumentationWidget(QWidget* parent)
    : QWidget(parent, Qt::Tool)
    , layout_(new QVBoxLayout(this))
    , titleLabel_(new QLabel("", this))
    , descriptionTextLabel_(new QLabel(tr("Description"), this))
    , descriptionEdit_(new QTextEdit("", this))
    , propertyTextLabel_(new QLabel(tr("Properties"), this))
    , propertyContainer_(new QWidget(this))
    , propertyTree_(new QTreeWidget(this))
    , propertyEdit_(new QTextEdit("", this))
    , portTextLabel_(new QLabel(tr("Ports"), this))
    , portContainer_(new QWidget(this))
    , portTree_(new QTreeWidget(this))
    , portEdit_(new QTextEdit("", this))
{
    setWindowTitle(tr("Documentation Editor"));

    layout_->addWidget(titleLabel_);
    layout_->addSpacing(5);

    layout_->addWidget(descriptionTextLabel_);
    descriptionEdit_->setFixedSize(640, 150);
    layout_->addWidget(descriptionEdit_);

    layout_->addSpacing(5);
    layout_->addWidget(propertyTextLabel_);
    QHBoxLayout* propertyLayout = new QHBoxLayout();
    propertyTree_->setColumnCount(1);
    propertyTree_->setHeaderHidden(true);
    connect(propertyTree_, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(propertyTreeSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    propertyLayout->addWidget(propertyTree_);
    propertyLayout->addWidget(propertyEdit_);
    propertyContainer_->setLayout(propertyLayout);
    propertyContainer_->setFixedSize(640, 135);
    layout_->addWidget(propertyContainer_);

    layout_->addSpacing(5);
    layout_->addWidget(portTextLabel_);
    QHBoxLayout* portLayout = new QHBoxLayout();
    portTree_->setColumnCount(1);
    portTree_->setHeaderHidden(true);
    connect(portTree_, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(portTreeSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    portLayout->addWidget(portTree_);
    portLayout->addWidget(portEdit_);
    portContainer_->setLayout(portLayout);
    portContainer_->setFixedSize(640, 135);
    layout_->addWidget(portContainer_);

    layout_->addSpacing(5);

    QPushButton* saveButton = new QPushButton(tr("Save"), this);
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(saveDescription()));
    connect(saveButton, SIGNAL(clicked(bool)), this, SIGNAL(finishedEditing()));
    layout_->addWidget(saveButton);
}

void DocumentationWidget::setTitleLabel(const std::string& title) {
    titleLabel_->setText(QString::fromStdString("<h3><b>" + title + "</b></h3>"));
}

void DocumentationWidget::initWithProcessor(const Processor* processor) {
    processor_ = processor;
    module_ = 0;

    setTitleLabel(processor->getClassName());
    const VoreenModule* module = VoreenApplication::app()->getModule(processor_->getModuleName());
    std::string description = module->getDocumentationDescription(processor->getClassName());
    descriptionEdit_->setPlainText(QString::fromStdString(description));

    propertyIDMap_.clear();
    propertyTree_->clear();
    propertyValueMap_.clear();
    propertyEdit_->setPlainText("");
    const std::vector<Property*>& properties = processor->getProperties();
    QList<QTreeWidgetItem*> items;
    foreach (Property* prop, properties) {
        QTreeWidgetItem* item = new QTreeWidgetItem(propertyTree_, QStringList(QString::fromStdString(prop->getGuiName())));
        propertyIDMap_.insert(item, prop->getID());
        items.append(item);
    }
    propertyTree_->insertTopLevelItems(0, items);

    portTree_->clear();
    portValueMap_.clear();
    portEdit_->setPlainText("");
    const std::vector<Port*>& ports = processor->getPorts();
    items.clear();
    foreach (Port* port, ports) {
        QTreeWidgetItem* item = new QTreeWidgetItem(portTree_, QStringList(QString::fromStdString(port->getName())));
        items.append(item);
    }
    portTree_->insertTopLevelItems(0, items);

    propertyTextLabel_->show();
    propertyContainer_->show();
    portTextLabel_->show();
    portContainer_->show();
}

void DocumentationWidget::initWithModule(const VoreenModule* module) {
    processor_ = 0;
    module_ = module;

    setTitleLabel(module->getName());
    descriptionEdit_->setPlainText(QString::fromStdString(module->getDescription()));

    propertyTextLabel_->hide();
    propertyContainer_->hide();
    portTextLabel_->hide();
    portContainer_->hide();
    adjustSize();
}

void DocumentationWidget::propertyTreeSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous) {
    if (!current)
        return;

    if (previous)
        propertyValueMap_[previous] = propertyEdit_->toPlainText().toStdString();

    if (propertyValueMap_.contains(current))
        propertyEdit_->setPlainText(QString::fromStdString(propertyValueMap_[current]));
    else {
        const VoreenModule* module = VoreenApplication::app()->getModule(processor_->getModuleName());
        std::string propertyID = propertyIDMap_.value(current);
        std::string description = module->getDocumentationProperty(processor_->getClassName(), propertyID);
        propertyEdit_->setPlainText(QString::fromStdString(description));
    }
}

void DocumentationWidget::portTreeSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous) {
    if (!current)
        return;

    if (previous)
        portValueMap_[previous] = portEdit_->toPlainText().toStdString();

    if (portValueMap_.contains(current))
        portEdit_->setPlainText(QString::fromStdString(portValueMap_[current]));
    else {
        const VoreenModule* module = VoreenApplication::app()->getModule(processor_->getModuleName());
        std::string portName = current->text(0).toStdString();
        std::string description = module->getDocumentationPort(processor_->getClassName(), portName);
        portEdit_->setPlainText(QString::fromStdString(description));
    }
}

void DocumentationWidget::saveDescription() {
    tgtAssert(processor_ || module_, "either a processor or a module must be set");
    tgtAssert((processor_ && module_) == 0, "only a processor or a module must be set but not both");

    if (processor_) {
        const VoreenModule* module = VoreenApplication::app()->getModule(processor_->getModuleName());

        // Save general description
        std::string description = descriptionEdit_->toPlainText().toStdString();
        module->setDocumentationDescription(processor_->getClassName(), description);

        // Save currently selected property
        QList<QTreeWidgetItem*> selectedItems = propertyTree_->selectedItems();
        if (selectedItems.count() > 0) {
            QTreeWidgetItem* currentPropertyItem = selectedItems.first();
            propertyValueMap_[currentPropertyItem] = propertyEdit_->toPlainText().toStdString();
        }

        // Save all new property values
        foreach (QTreeWidgetItem* item, propertyValueMap_.keys()) {
            std::string propertyID = propertyIDMap_.value(item);
            description = propertyValueMap_[item];
            module->setDocumentationProperty(processor_->getClassName(), propertyID, description);
        }
        propertyValueMap_.clear();

        // Save currently selected port
        selectedItems = portTree_->selectedItems();
        if (selectedItems.count() > 0) {
            QTreeWidgetItem* currentPortItem = selectedItems.first();
            portValueMap_[currentPortItem] = portEdit_->toPlainText().toStdString();
        }

        // Save all new port values
        foreach (QTreeWidgetItem* item, portValueMap_.keys()) {
            std::string portName = item->text(0).toStdString();
            description = portValueMap_[item];
            module->setDocumentationPort(processor_->getClassName(), portName, description);
        }
        portValueMap_.clear();

    }
    else {
        std::string description = descriptionEdit_->toPlainText().toStdString();
        module_->setDescription(description);
    }
}

} // namespace

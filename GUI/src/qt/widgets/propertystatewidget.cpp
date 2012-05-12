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

#include "voreen/qt/widgets/propertystatewidget.h"

#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"
#include "voreen/core/network/processornetwork.h"
#include "voreen/core/network/propertystate.h"
#include "voreen/core/network/propertystatecollection.h"
#include "voreen/core/properties/property.h"
#include "voreen/qt/widgets/propertyselectiondialog.h"
#include "voreen/qt/widgets/stringselectiondialog.h"

#include <QAction>
#include <QDateTime>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <fstream>

#define UNTITLEDPROPERTYCOLLECTION "Untitled property state collection"

namespace {
inline QStringList stringListFromStdString(const std::string& string) {
    return QStringList(QString::fromStdString(string));
}
inline QStringList stringListFromStdString(const std::string& string1, const std::string& string2) {
    QStringList result;
    result << QString::fromStdString(string1) << QString::fromStdString(string2);
    return result;
}
inline std::string convertTime(const std::string& time) {
    return QDateTime::fromString(QString::fromStdString(time), Qt::ISODate).toString(Qt::SystemLocaleShortDate).toStdString();
}

QString whatsThisInfo = "<h3>Property State History</h3>This widget provides access to previously stored sets of property values. \
                        States can be either saved directly within the workspace or they can be exported and the resulting *.vps \
                        file can be referenced. Referenced files will be shown in the list as <i>italic</i>. It is possible to \
                        <p><img src=\":/voreenve/icons/clear.png\" width=\"15\"> <b>Create</b> a new set of properties</p> \
                        <p><img src=\":/voreenve/icons/document_open.png\" width=\"15\"> <b>Open</b> or <b>Import</b> a set of \
                        property states. <i>Open</i> will store the states within the workspace while <i>Import</i> will store only \
                        the path to the selected file(s) in the workspace</p> \
                        <p><img src=\":/voreenve/icons/document_save.png\" width=\"15\"> <b>Export</b> a set of property states to an \
                        external file so that it can be later imported or loaded back into another workspace.</p> \
                        <p><img src=\":/voreenve/icons/eraser.png\" width=\"15\"> <b>Deletes</b> one or more property states from the \
                        workspace. If the simple erase is chosed for a <i>referenced</i> state, the state is not deleted permanently \
                        but only removed locally. To remove a reference for good, select the \"Remove references\" sub menu item</p> \
                        <p><b>Rename</b> by selecting a property state or a single processor and pressing \"F2\". The name of the \
                        state serves no purpose by itself but if the processor name is changed, the properties stored within will be \
                        applied to the processor in the network with the new name.</p>\
                        <p><b>Apply </b> by double clicking a property set, a processor or a single property in the tree. The processor \
                        name is checked during this step, so if the processor has been renamed, the property values are applied to the \
                        processor in the network which has this name now. This may be used to apply a set of properties to a different \
                        processor for which it was originally saved.</p>";
}

namespace voreen {

Property* StateItem::getProperty(ProcessorNetwork* network, const std::string& processorName, const std::string& propertyID) const {
    Processor* processor = network->getProcessor(processorName);
    if (processor)
        return processor->getProperty(propertyID);
    else
        return 0;
}

CollectionTreeItem::CollectionTreeItem(QTreeWidgetItem* parent, PropertyStateCollection* collection)
    : QTreeWidgetItem(parent, stringListFromStdString(collection->getName(), convertTime(collection->getTimeStamp())))
    , collection_(collection)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

void CollectionTreeItem::applyState(ProcessorNetwork* network) {
    foreach (PropertyState* state, collection_->getPropertyStates()) {
        Property* prop = getProperty(network, state->getPropertyOwner(), state->getPropertyID());
        if (prop)
            state->applyStateToProperty(prop);
    }
}

void CollectionTreeItem::setName(const std::string& name) {
    collection_->setName(name);
}

PropertyStateCollection* CollectionTreeItem::getCollection() const {
    return collection_;
}


ProcessorTreeItem::ProcessorTreeItem(QTreeWidgetItem* parent, PropertyStateCollection* collection, const std::string& processorName)
    : QTreeWidgetItem(parent, stringListFromStdString(processorName))
    , collection_(collection)
    , processorName_(processorName)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

void ProcessorTreeItem::applyState(ProcessorNetwork* network) {
    foreach (PropertyState* state, collection_->getPropertyStates()) {
        if (state->getPropertyOwner() == processorName_) {
            Property* prop = getProperty(network, state->getPropertyOwner(), state->getPropertyID());
            if (prop)
                state->applyStateToProperty(prop);
        }
    }
}

PropertyStateCollection* ProcessorTreeItem::getCollection() const {
    std::vector<PropertyState*> stateList;
    foreach (PropertyState* state, collection_->getPropertyStates()) {
        if (state->getPropertyOwner() == processorName_)
            stateList.push_back(state);
    }
    QString time = QDateTime::currentDateTime().toString(Qt::ISODate);
    PropertyStateCollection* result = new PropertyStateCollection(UNTITLEDPROPERTYCOLLECTION, time.toStdString(), stateList);
    return result;
}

const std::string& ProcessorTreeItem::getProcessorName() const {
    return processorName_;
}

void ProcessorTreeItem::setProcessorName(const std::string& processorName) {
    collection_->renamePropertyOwner(processorName_, processorName);
    processorName_ = processorName;
}

PropertyTreeItem::PropertyTreeItem(QTreeWidgetItem* parent, PropertyState* state) 
    : QTreeWidgetItem(parent, stringListFromStdString(state->getPropertyName()))
    , state_(state)
{}

void PropertyTreeItem::applyState(ProcessorNetwork* network) {
    Property* prop = getProperty(network, state_->getPropertyOwner(), state_->getPropertyID());
    if (prop)
        state_->applyStateToProperty(prop);
}

PropertyStateCollection* PropertyTreeItem::getCollection() const {
    QString time = QDateTime::currentDateTime().toString(Qt::ISODate);
    PropertyStateCollection* result = new PropertyStateCollection(UNTITLEDPROPERTYCOLLECTION, time.toStdString(), state_);
    return result;
}

PropertyState* PropertyTreeItem::getPropertyState() const {
    return state_;
}

NoEditTreeDelegate::NoEditTreeDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{}

QWidget* NoEditTreeDelegate::createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const {
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

PropertyStateWidget::PropertyStateWidget(ProcessorNetwork* processorNetwork, QWidget* parent)
    : QWidget(parent)
    , network_(processorNetwork)
    , isEditing_(false)
{
    setWhatsThis(whatsThisInfo);

    QVBoxLayout* layout = new QVBoxLayout(this);

    treeWidget_ = new QTreeWidget;
    treeWidget_->setHeaderHidden(true);
    treeWidget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeWidget_->setColumnCount(2);
    treeWidget_->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    treeWidget_->setExpandsOnDoubleClick(false);
    //treeWidget_->setDragEnabled(true);
    //treeWidget_->setDragDropMode(QAbstractItemView::InternalMove);
    treeWidget_->header()->setResizeMode(0, QHeaderView::Stretch);
    treeWidget_->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    treeWidget_->header()->setStretchLastSection(false);

    treeDelegate_ = new QStyledItemDelegate;
    connect(treeDelegate_, SIGNAL(commitData(QWidget*)), this, SLOT(editorClosed(QWidget*)));
    treeWidget_->setItemDelegate(treeDelegate_);
    noEditTreeDelegate_ = new NoEditTreeDelegate;
    treeWidget_->setItemDelegateForColumn(1, noEditTreeDelegate_);
    connect(treeWidget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(restoreValue(QTreeWidgetItem*, int)));
    layout->addWidget(treeWidget_);

    QToolBar* toolbar = new QToolBar;
    toolbar->setIconSize(QSize(20,20));
    toolbar->addAction(QIcon(":/voreenve/icons/clear.png"), tr("Create a new Property State Snapshot"), this, SLOT(createSnapshot()));
    QAction* openAction = new QAction(QIcon(":/voreenve/icons/document_open.png"), tr("Open or import one or multiple Property State files"), this);
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(importEntities()));
    QMenu* openMenu = new QMenu;
    QAction* openRefAction = new QAction(tr("Open files by reference"), this);
    connect(openRefAction, SIGNAL(triggered(bool)), this, SLOT(referenceFiles()));
    openMenu->addAction(openRefAction);
    QAction* openDirRefAction = new QAction(tr("Open directories by reference"), this);
    connect(openDirRefAction, SIGNAL(triggered(bool)), this, SLOT(referenceDirectory()));
    openMenu->addAction(openDirRefAction);
    openAction->setMenu(openMenu);
    toolbar->addAction(openAction);
    toolbar->addAction(QIcon(":/voreenve/icons/document_save.png"), tr("Export the selected Property States"), this, SLOT(exportEntities()));

    QWidget* space = new QWidget;
    space->resize(0, 0);
    space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(space);

    QAction* deleteAction = new QAction(QIcon(":/voreenve/icons/eraser.png"), tr("Delete the currently selected Property States"), this);
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteEntity()));
    QMenu* deleteMenu = new QMenu;
    QAction* removeRefAction = new QAction(tr("Remove references"), this);
    connect(removeRefAction, SIGNAL(triggered(bool)), this, SLOT(deleteReferences()));
    deleteMenu->addAction(removeRefAction);
    deleteAction->setMenu(deleteMenu);
    toolbar->addAction(deleteAction);
    layout->addWidget(toolbar);

    createTreeItems();
}

PropertyStateWidget::~PropertyStateWidget() {
    delete treeDelegate_;
    delete noEditTreeDelegate_;
}

void PropertyStateWidget::setProcessorNetwork(ProcessorNetwork* network) {
    network_ = network;
    createTreeItems();
}

void PropertyStateWidget::createTreeItems() {
    treeWidget_->clear();
    const std::vector<PropertyStateCollection*>& collections = network_->getPropertyStateCollections();
    foreach (PropertyStateCollection* collection, collections)
        addPropertyStateCollectionItem(collection, false);

    const std::vector<PropertyStateCollection*>& referencedCollections = network_->getReferencedPropertyStateCollections();
    foreach (PropertyStateCollection* collection, referencedCollections)
        addPropertyStateCollectionItem(collection, true);
}

CollectionTreeItem* PropertyStateWidget::createCollectionTreeItem(PropertyStateCollection* collection, bool referenced) {
    CollectionTreeItem* collectionItem = new CollectionTreeItem(0, collection);

    if (referenced) {
        {
        QFont font = collectionItem->font(0);
        font.setItalic(true);
        collectionItem->setFont(0, font);
        }
        {
        QFont font = collectionItem->font(1);
        font.setItalic(true);
        collectionItem->setFont(1, font);
        }
    }

    std::vector<ProcessorTreeItem*> processorItems;

    const std::vector<PropertyState*>& states = collection->getPropertyStates();
    foreach (PropertyState* state, states) {
        ProcessorTreeItem* parentProcessor = 0;
        foreach (ProcessorTreeItem* processorItem, processorItems) {
            if (processorItem->text(0) == QString::fromStdString(state->getPropertyOwner())) {
                parentProcessor = processorItem;
                break;
            }
        }

        if (!parentProcessor) {
            parentProcessor = new ProcessorTreeItem(collectionItem, collection, state->getPropertyOwner());
            processorItems.push_back(parentProcessor);
        }
        new PropertyTreeItem(parentProcessor, state);
    }

    return collectionItem;
}


void PropertyStateWidget::addPropertyStateCollectionItem(PropertyStateCollection* collection, bool referenced) {
    CollectionTreeItem* collectionItem = createCollectionTreeItem(collection, referenced);
    treeWidget_->addTopLevelItem(collectionItem);
}

void PropertyStateWidget::addReferencedPropertyStateCollection(const std::string& reference) {
    std::vector<PropertyStateCollection*> collections;
    std::fstream f;
    f.open(reference.c_str(), std::ios::in);
    XmlDeserializer d;
    d.read(f);
    d.deserialize("PropertyStateCollections", collections, "PropertyStateCollection");
    f.close();

    foreach (PropertyStateCollection* collection, collections) {
        addPropertyStateCollectionItem(collection, true);
    }
}


void PropertyStateWidget::createSnapshot() {
    QList<Property*> properties;

    PropertySelectionDialog dialog(properties, network_, this);
    dialog.exec();

    if (properties.size() == 0)
        return;

    std::vector<PropertyState*> states(properties.size());

    for (int i = 0; i < properties.size(); ++i) {
        Property* prop = properties.at(i);
        PropertyState* state = new PropertyState(prop);
        states[i] = state;

    }

    QString time = QDateTime::currentDateTime().toString(Qt::ISODate);
    PropertyStateCollection* collection = new PropertyStateCollection(UNTITLEDPROPERTYCOLLECTION, time.toStdString(), states);
    network_->addPropertyStateCollection(collection);
    emit modifiedNetwork();
    addPropertyStateCollectionItem(collection, false);
}

void PropertyStateWidget::deleteEntity() {
    QList<QTreeWidgetItem*> selectedItems = treeWidget_->selectedItems();
    for (int i = 0; i < selectedItems.count(); ++i) {
        QTreeWidgetItem* currItem = selectedItems.at(i);
        CollectionTreeItem* collItem = dynamic_cast<CollectionTreeItem*>(currItem);
        ProcessorTreeItem* procItem = dynamic_cast<ProcessorTreeItem*>(currItem);
        PropertyTreeItem* propItem = dynamic_cast<PropertyTreeItem*>(currItem);
        if (collItem) {
            network_->removePropertyStateCollection(collItem->getCollection());
        }
        else if (procItem) {
            CollectionTreeItem* parentCollection = dynamic_cast<CollectionTreeItem*>(procItem->parent());
            parentCollection->getCollection()->removePropertyOwner(procItem->getProcessorName());
        }
        else if (propItem) {
            CollectionTreeItem* parentCollection = dynamic_cast<CollectionTreeItem*>(propItem->parent()->parent());
            parentCollection->getCollection()->removePropertyState(propItem->getPropertyState());
        }
        else
            tgtAssert(false, "QTreeWidgetItem is neither a CollectionTI, ProcessorTI nor PropertyTI");

        delete currItem;
    }

    if (selectedItems.size() > 0)
        emit modifiedNetwork();
}

void PropertyStateWidget::deleteReferences() {
    QStringList referencesToDelete;
    QStringList references;

    foreach (const std::string& ref, network_->getReferencedFiles())
        references.append(QString::fromStdString(ref));
    foreach (const std::string& ref, network_->getReferencedDirectories())
        references.append(QString::fromStdString(ref));

    StringSelectionDialog dialog(referencesToDelete, references, this);
    dialog.exec();

    foreach (const QString& str, referencesToDelete) {
        network_->removePropertyStateCollectionReferenceFile(str.toStdString());
        network_->removePropertyStateCollectionReferenceDirectory(str.toStdString());
    }

    if (referencesToDelete.size() > 0) {
        createTreeItems();
        emit modifiedNetwork();
    }
}

void PropertyStateWidget::exportEntities() {
    QList<QTreeWidgetItem*> selectedItems = treeWidget_->selectedItems();
    std::vector<PropertyStateCollection*> exportItems;

    foreach (QTreeWidgetItem* treeItem, selectedItems) {
        StateItem* stateItem = dynamic_cast<StateItem*>(treeItem);
        PropertyStateCollection* collection = stateItem->getCollection();
        exportItems.push_back(collection);
    }


    QString fileName = QFileDialog::getSaveFileName(0, tr("VoreenVE"), "", "Voreen Property Export (*.vps)");
    std::fstream f;
    f.open(fileName.toStdString().c_str(), std::ios::out);
    
    XmlSerializer s;
    s.serialize("PropertyStateCollections", exportItems, "PropertyStateCollection");
    s.write(f);
    f.close();
}

void PropertyStateWidget::importEntities() {
    QStringList files = QFileDialog::getOpenFileNames(0, tr("VoreenVE"), "", "Voreen Property Export (*.vps)");

    foreach (const QString& file, files) {
        std::vector<PropertyStateCollection*> collections;
        std::fstream f;
        f.open(file.toStdString().c_str(), std::ios::in);
        XmlDeserializer d;
        d.read(f);
        d.deserialize("PropertyStateCollections", collections, "PropertyStateCollection");
        f.close();
        
        foreach (PropertyStateCollection* collection, collections) {
            addPropertyStateCollectionItem(collection, false);
            network_->addPropertyStateCollection(collection);
        }
    }

    emit modifiedNetwork();
}

void PropertyStateWidget::referenceFiles() {
    QStringList files = QFileDialog::getOpenFileNames(0, tr("VoreenVE"), "", "Voreen Property Export (*.vps)");

    foreach (const QString& file, files) {
        addReferencedPropertyStateCollection(file.toStdString());
        network_->addPropertyStateCollectionReferenceFile(file.toStdString());
    }

    emit modifiedNetwork();
}

void PropertyStateWidget::referenceDirectory() {
    QString dirPath = QFileDialog::getExistingDirectory(0, tr("VoreenVE"), "");
    network_->addPropertyStateCollectionReferenceDirectory(dirPath.toStdString());

    createTreeItems();
    emit modifiedNetwork();
}

void PropertyStateWidget::applyToOtherEntity() {

}

void PropertyStateWidget::restoreValue() {
    QList<QTreeWidgetItem*> selectedItems = treeWidget_->selectedItems();
    foreach (QTreeWidgetItem* treeItem, selectedItems) {
        StateItem* stateItem = dynamic_cast<StateItem*>(treeItem);
        stateItem->applyState(network_);
    }
}

void PropertyStateWidget::restoreValue(QTreeWidgetItem* item, int) {
    StateItem* stateItem = dynamic_cast<StateItem*>(item);
    stateItem->applyState(network_);

    //PropertyTreeItem* propertyTreeItem = dynamic_cast<PropertyTreeItem*>(item);
    //if (propertyTreeItem)
    //    propertyTreeItem->applyState(network_);
}

void PropertyStateWidget::editorClosed(QWidget* editor) {
    isEditing_ = false;
    QLineEdit* edit = dynamic_cast<QLineEdit*>(editor);
    tgtAssert(edit, "Editor is no QLineEdit");
    const QString& newText = edit->text();
    QList<QTreeWidgetItem*> selectedItems = treeWidget_->selectedItems();
    QTreeWidgetItem* currentItem = selectedItems.front();
    CollectionTreeItem* collectionItem = dynamic_cast<CollectionTreeItem*>(currentItem);
    ProcessorTreeItem* processorItem = dynamic_cast<ProcessorTreeItem*>(currentItem);

    if (collectionItem)
        collectionItem->setName(newText.toStdString());
    else if (processorItem)
        processorItem->setProcessorName(newText.toStdString());
    else
        tgtAssert(false, "Parent of editor QLineEdit was neither CollectionTI or ProcessorTI");

    emit modifiedNetwork();
}

} // namespace

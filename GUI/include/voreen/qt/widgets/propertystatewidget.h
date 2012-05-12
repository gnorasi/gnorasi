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

#ifndef VRN_PROPERTYSTATEWIDGET_H
#define VRN_PROPERTYSTATEWIDGET_H

#include "voreen/qt/voreenqtdefine.h"
#include <QWidget>
//#include <QTreeWidget>
#include <QStyledItemDelegate>
#include <QTreeWidgetItem>

//class QStyledItemDelegate;
class QTreeWidget;

namespace voreen {

class ProcessorNetwork;
class Property;
class PropertyState;
class PropertyStateCollection;

class StateItem {
public:
    virtual void applyState(ProcessorNetwork* network) = 0;
    virtual PropertyStateCollection* getCollection() const = 0;

protected:
    Property* getProperty(ProcessorNetwork* network, const std::string& processorName, const std::string& propertyName) const;
};

class CollectionTreeItem : public QTreeWidgetItem, public StateItem {
public:
    CollectionTreeItem(QTreeWidgetItem* parent, PropertyStateCollection* collection);

    void setName(const std::string& name);

    void applyState(ProcessorNetwork* network);
    PropertyStateCollection* getCollection() const;

private:
    PropertyStateCollection* collection_;
};

class ProcessorTreeItem : public QTreeWidgetItem, public StateItem {
public:
    ProcessorTreeItem(QTreeWidgetItem* parent, PropertyStateCollection* collection, const std::string& processorName);
    void applyState(ProcessorNetwork* network);

    PropertyStateCollection* getCollection() const;
    void setProcessorName(const std::string& processorName);
    const std::string& getProcessorName() const;

private:
    PropertyStateCollection* collection_;
    std::string processorName_;
};

class PropertyTreeItem : public QTreeWidgetItem, public StateItem {
public:
    PropertyTreeItem(QTreeWidgetItem* parent, PropertyState* state);

    void applyState(ProcessorNetwork* network);

    PropertyStateCollection* getCollection() const;
    PropertyState* getPropertyState() const;

private:
    PropertyState* state_;
};

class NoEditTreeDelegate : public QStyledItemDelegate {
public:
    NoEditTreeDelegate(QObject* parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class VRN_QT_API PropertyStateWidget : public QWidget {
Q_OBJECT
public:
    PropertyStateWidget(ProcessorNetwork* processorNetwork, QWidget* parent = 0);
    ~PropertyStateWidget();
    void setProcessorNetwork(ProcessorNetwork* network);

signals:
    void modifiedNetwork();

private slots:
    void createSnapshot();
    void deleteEntity();
    void deleteReferences();
    void exportEntities();
    void importEntities();
    void applyToOtherEntity();
    void restoreValue();
    void restoreValue(QTreeWidgetItem* item, int column);
    void referenceFiles();
    void referenceDirectory();

    void editorClosed(QWidget* editor);

private:
    void createTreeItems();
    void addPropertyStateCollectionItem(PropertyStateCollection* collection, bool referenced);
    CollectionTreeItem* createCollectionTreeItem(PropertyStateCollection* collection, bool referenced);
    Property* getProperty(const std::string& processorName, const std::string& propertyID) const;
    void addReferencedPropertyStateCollection(const std::string& reference);

    ProcessorNetwork* network_;
    QTreeWidget* treeWidget_;
    QStyledItemDelegate* treeDelegate_;
    NoEditTreeDelegate* noEditTreeDelegate_;

    bool isEditing_;
};


} // namespace

#endif // VRN_PROPERTYSTATEWIDGET_H

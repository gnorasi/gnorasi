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

#include <QCheckBox>
#include <QErrorMessage>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QToolButton>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QVBoxLayout>

#include <QApplication>

#include "tgt/matrix.h"

#include "voreen/qt/widgets/volumeloadbutton.h"
#include "voreen/qt/widgets/volumecontainerwidget.h"
#include "voreen/qt/widgets/volumeviewhelper.h"
#include "voreen/qt/widgets/rawvolumewidget.h"

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/io/progressbar.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/voreenapplication.h"

#include "voreen/qt/voreenapplicationqt.h"
#include "voreen/qt/progressdialog.h"

#ifdef _MSC_VER
#include "tgt/gpucapabilitieswindows.h"
#endif

#include "voreen/core/io/rawvolumereader.h"

namespace {
#ifdef __APPLE__
    int fontSize = 13;
#else
    int fontSize = 8;
#endif
}

namespace {
    QString whatsThisInfo = "<h3>Volume Container Widget</h3>This widget shows all the volumes which are currently loaded and provides \
                            a means to load more volumes into the current workspace.";
}

namespace voreen {

QRCTreeWidget::QRCTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setTextElideMode(Qt::ElideMiddle);
}

void QRCTreeWidget::contextMenuEvent(QContextMenuEvent* cmevent) {
    if (itemAt(cmevent->pos())){
        QMenu* menu = new QMenu();
        menu->addAction(QPixmap(":/voreenve/icons/refresh.png"), "Refresh");
        menu->addAction(QPixmap(":/voreenve/icons/export.png"), "Export...");
        menu->addAction(QPixmap(":/voreenve/icons/cancel.png"), "Remove");
        QAction* action = menu->exec(mapToGlobal(cmevent->pos()));
        if(action) {
            if(action->text() == "Export...")
                emit exportDat();
            if(action->text() == "Refresh")
                emit refresh(itemAt(cmevent->pos()));
            if(action->text() == "Remove")
                emit remove();
            }
    } else {
        QMenu* menu = new QMenu();
        menu->addAction(QPixmap(":/voreenve/icons/open-volume.png"), "Load Volume...");
        QAction* action = menu->exec(mapToGlobal(cmevent->pos()));
        if (action && action->text() == "Load Volume...") {
            emit add();
        }
    }
}

void QRCTreeWidget::keyPressEvent(QKeyEvent* event) {
    if ((event->key() == Qt::Key_Delete) || (event->key() == Qt::Key_Backspace)) {
        event->accept();
        emit remove();
    }

    QTreeWidget::keyPressEvent(event);
}

// ---------------------------------------------------------------------------

const std::string VolumeContainerWidget::loggerCat_("voreen.qt.VolumeContainerWidget");

VolumeContainerWidget::VolumeContainerWidget(VolumeContainer* volumeContainer, QWidget* parent)
    : QWidget(parent, Qt::Tool)
    , volumeIOHelper_(volumeContainer, parent)
    , volumeContainer_(volumeContainer)
{
    if (volumeContainer_)
        volumeContainer_->addObserver(this);
    else
        LWARNING("No volume container assigned");

    setFocusPolicy(Qt::StrongFocus);
    setWhatsThis(whatsThisInfo);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    volumeInfos_ = new QRCTreeWidget();
    QTreeWidgetItem* header = volumeInfos_->headerItem();
    header->setText(0, tr("Volumes"));
    volumeInfos_->header()->hide();
    volumeInfos_->setColumnCount(1);
    volumeInfos_->show();
    volumeInfos_->setIconSize(QSize(63,63));

    volumeLoadButton_ = new VolumeLoadButton(volumeContainer_, this);
    volumeLoadButton_->setMinimumWidth(110);
    containerInfo_ = new QLabel(this);

    connect(volumeInfos_, SIGNAL(refresh(QTreeWidgetItem*)), this, SLOT(volumeRefresh(QTreeWidgetItem*)));
    connect(volumeInfos_, SIGNAL(add()), &volumeIOHelper_, SLOT(showFileOpenDialog()));
    connect(volumeInfos_, SIGNAL(remove()), this, SLOT(removeVolume()));
    connect(volumeInfos_, SIGNAL(exportDat()), this, SLOT(exportDat()));

    buttonLayout->addWidget(volumeLoadButton_);

    buttonLayout->addStretch();
    buttonLayout->addWidget(containerInfo_);

    mainLayout->addWidget(volumeInfos_);
    mainLayout->addLayout(buttonLayout);

    setMinimumWidth(250);
    setMinimumHeight(125);
    update();
}

VolumeContainerWidget::~VolumeContainerWidget() {
}

void VolumeContainerWidget::resizeOnCollapse(bool) {
    adjustSize();
}

QSize VolumeContainerWidget::sizeHint() const {
    return QSize(300, 300);
}

void VolumeContainerWidget::setVolumeContainer(VolumeContainer* volumeContainer) {

    // do not deregister from previously assigned container,
    // since it may not be valid anymore!

    // assign new container
    volumeContainer_ = volumeContainer;
    volumeLoadButton_->setVolumeContainer(volumeContainer);
    volumeIOHelper_.setVolumeContainer(volumeContainer);

    // register at new container
    if (volumeContainer_)
        volumeContainer_->addObserver(this);

    update();

}

std::string VolumeContainerWidget::calculateSize() {

    size_t volumeSize = 0;

    if (!volumeContainer_ || volumeContainer_->empty()) {
        return "";
    }
    for(size_t i = 0 ; i < volumeContainer_->size(); ++i) {
        const Volume* volume = volumeContainer_->at(i)->getRepresentation<Volume>();
        volumeSize += VolumeViewHelper::getVolumeMemorySizeByte(volume);
    }
    int volumeCount = static_cast<int>(volumeContainer_->size());
    size_t bytes = volumeSize;
    std::stringstream out;
    if(volumeCount == 1) {
        out << volumeCount << " Volume (";
    }
    else if (volumeCount > 1) {
        out << volumeCount << " Volumes (";
    }
    float mb = tgt::round(bytes/104857.6f) / 10.f;    //calculate mb with 0.1f precision
    float kb = tgt::round(bytes/102.4f) / 10.f;
    if (mb >= 0.5f) {
        out << mb << " MB)";
    }
    else if (kb >= 0.5f) {
        out << kb << " kB)";
    }
    else {
        out << bytes << " bytes)";
    }
    return out.str();
}

void VolumeContainerWidget::update() {
    volumeInfos_->clear();
    volumeItems_.clear();
    if (!volumeContainer_ || volumeContainer_->empty()) {
        containerInfo_->setText("");
        return;
    }

    for(size_t i = 0 ; i < volumeContainer_->size(); ++i) {
        VolumeHandleBase* handle = volumeContainer_->at(i);
        const Volume* volume = volumeContainer_->at(i)->getRepresentation<Volume>();
        QTreeWidgetItem* qtwi = new QTreeWidgetItem(volumeInfos_);
        std::string name = VolumeViewHelper::getStrippedVolumeName(handle);
        std::string path = VolumeViewHelper::getVolumePath(handle);
        volumeItems_[handle->getRepresentation<Volume>()] = qtwi;

        QFontInfo fontInfo(qtwi->font(0));

        /*QLabel* infos = new QLabel(QString(name.c_str()) + " (" + QString(VolumeViewHelper::getVolumeType(volume).c_str()) + ")" + QString(QChar::LineSeparator)
                      + QString(path.c_str()) + QString(QChar::LineSeparator)
                      + "Dimension: " + QString(VolumeViewHelper::getVolumeDimension(volume).c_str()));
        infos->setWordWrap(true);*/

        qtwi->setFont(0, QFont(fontInfo.family(), fontSize));
        qtwi->setText(0, QString(name.c_str()) + QString::fromStdString(VolumeViewHelper::getVolumeTimestep(handle)) + " (" + QString(VolumeViewHelper::getVolumeType(volume).c_str()) + ")" + QString(QChar::LineSeparator)
                      + QString(path.c_str()) + QString(QChar::LineSeparator)
                      + "Dimension: " + QString(VolumeViewHelper::getVolumeDimension(handle).c_str()));

        qtwi->setIcon(0, QIcon(VolumeViewHelper::generateBorderedPreview(handle, 63, 0)));
        qtwi->setSizeHint(0,QSize(65,65));
        qtwi->setToolTip(0, QString::fromStdString(VolumeViewHelper::getStrippedVolumeName(handle)
            + " ("+VolumeViewHelper::getVolumeType(volume)+")"+"\n"+VolumeViewHelper::getVolumePath(handle) + VolumeViewHelper::getVolumeTimestep(handle)
            + "\nDimensions: " + VolumeViewHelper::getVolumeDimension(handle) + "\nVoxel Spacing: "
            + VolumeViewHelper::getVolumeSpacing(handle) +"\nMemory Size: "
            + VolumeViewHelper::getVolumeMemorySize(volume)));

        volumeInfos_->addTopLevelItem(qtwi);
        //volumeInfos_->setItemWidget(qtwi, 0, infos);
    }
    containerInfo_->setText(QString::fromStdString(calculateSize()));
}

void VolumeContainerWidget::removeVolume() {
    QList<QTreeWidgetItem *> allItems = volumeInfos_->selectedItems();
    QList<QTreeWidgetItem *>::iterator it = allItems.begin();
    VolumeCollection* volumes = new VolumeCollection();
    while(it != allItems.end()) {
        if ((*it)->parent() == 0) {
            volumeInfos_->removeItemWidget(*it, 0);
            volumes->add(volumeContainer_->at(volumeInfos_->indexOfTopLevelItem(*it)));
        }
        it++;
    }
    volumeContainer_->remove(volumes);
    delete volumes;
}

void VolumeContainerWidget::keyPressEvent(QKeyEvent* keyEvent) {
    if((keyEvent->key() == Qt::Key_Minus) || (keyEvent->key() == Qt::Key_Delete)) {
        removeVolume();
    }
}

void VolumeContainerWidget::volumeAdded(const VolumeCollection* /*source*/, const VolumeHandleBase* handleConst) {
    VolumeHandleBase* handle = const_cast<VolumeHandleBase*>(handleConst);
    QTreeWidgetItem* qtwi = new QTreeWidgetItem(volumeInfos_);
    std::string name = VolumeViewHelper::getStrippedVolumeName(handle);
    std::string path = VolumeViewHelper::getVolumePath(handle);
    volumeItems_[handle->getRepresentation<Volume>()] = qtwi;

    const Volume* volume = handle->getRepresentation<Volume>();

    QFontInfo fontInfo(qtwi->font(0));

    qtwi->setFont(0, QFont(fontInfo.family(), fontSize));
    qtwi->setText(0, QString(name.c_str()) + " (" + QString(VolumeViewHelper::getVolumeType(volume).c_str()) + ")" + QString(QChar::LineSeparator)
                      + QString(path.c_str()) + QString(QChar::LineSeparator)
                      + "Dimension: " + QString(VolumeViewHelper::getVolumeDimension(handleConst).c_str()));

    qtwi->setIcon(0, QIcon(VolumeViewHelper::generateBorderedPreview(handleConst, 63, 0)));
    qtwi->setSizeHint(0,QSize(65,65));
    qtwi->setToolTip(0, QString::fromStdString(VolumeViewHelper::getStrippedVolumeName(handle)
        + " ("+VolumeViewHelper::getVolumeType(volume)+")"+"\n"+VolumeViewHelper::getVolumePath(handle)
        +"\nDimensions: " + VolumeViewHelper::getVolumeDimension(handleConst) + "\nVoxel Spacing: "
        + VolumeViewHelper::getVolumeSpacing(handleConst) +"\nMemory Size: "
        + VolumeViewHelper::getVolumeMemorySize(volume)));

    volumeInfos_->addTopLevelItem(qtwi);

    containerInfo_->setText(QString::fromStdString(calculateSize()));
}

void VolumeContainerWidget::volumeRemoved(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/) {
    update();
}

void VolumeContainerWidget::volumeChanged(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/) {
    update();
}

void VolumeContainerWidget::volumeRefresh(QTreeWidgetItem* /*item*/) {
    qApp->processEvents();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QList<QTreeWidgetItem *> allItems = volumeInfos_->selectedItems();
    QList<QTreeWidgetItem *>::iterator it = allItems.begin();
    std::vector<VolumeHandle*> volumePointer;
    while(it != allItems.end())
    {
        VolumeHandle* vh = dynamic_cast<VolumeHandle*>(volumeContainer_->at(volumeInfos_->indexOfTopLevelItem(*it)));
        volumePointer.push_back(vh);
        it++;
    }
    std::vector<VolumeHandle*>::iterator vit = volumePointer.begin();
    while(vit != volumePointer.end())
    {
        (*vit)->reloadVolume();
        vit++;
    }
    QApplication::restoreOverrideCursor();
}

void VolumeContainerWidget::loadVolume() {
    volumeIOHelper_.showFileOpenDialog();
}

void VolumeContainerWidget::exportDat() {
    QList<QTreeWidgetItem *> allItems = volumeInfos_->selectedItems();
    QList<QTreeWidgetItem *>::iterator it = allItems.begin();
    while(it != allItems.end()) {
        volumeIOHelper_.showFileSaveDialog(volumeContainer_->at(volumeInfos_->indexOfTopLevelItem( (*it))));
        it++;
    }

}

} // namespace

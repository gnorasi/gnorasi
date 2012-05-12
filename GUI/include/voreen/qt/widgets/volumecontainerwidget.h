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

#ifndef VRN_VOLUMECONTAINERWIDGET_H
#define VRN_VOLUMECONTAINERWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>

#include "voreen/core/datastructures/volume/volumecontainer.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/qt/voreenqtdefine.h"
#include "voreen/qt/widgets/volumeiohelper.h"

class QPixmap;
class VolumeLoadButton;

namespace voreen {

class QRCTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    QRCTreeWidget(QWidget* parent = 0);

signals:
    void refresh(QTreeWidgetItem*);
    void remove();
    void add();
    void addRaw();
    void exportDat();

protected:
    void keyPressEvent(QKeyEvent* event);
    void contextMenuEvent(QContextMenuEvent*);
};

// ---------------------------------------------------------------------------

class VRN_QT_API VolumeContainerWidget : public QWidget, public VolumeCollectionObserver  {
Q_OBJECT
public:

    VolumeContainerWidget(VolumeContainer* container, QWidget* parent = 0);
    ~VolumeContainerWidget();

    void setVolumeContainer(VolumeContainer* volumeContainer);

    virtual QSize sizeHint() const;

    /// @see VolumeCollectionObserver
    void volumeAdded(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/);
    /// @see VolumeCollectionObserver
    void volumeRemoved(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/);
    /// @see VolumeCollectionObserver
    void volumeChanged(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/);

    /// Calls showFileOpenDialog() on the internal VolumeIOHelper.
    void loadVolume();

protected:
    void keyPressEvent(QKeyEvent*);
    std::string calculateSize();

    VolumeLoadButton* volumeLoadButton_;
    VolumeIOHelper volumeIOHelper_;

    QLabel* containerInfo_;
    std::map<const Volume*, QTreeWidgetItem*> volumeItems_;

private:
    void update();

    VolumeContainer* volumeContainer_;

    /**
    * QTreeWidget with custom context Menu is only applied
    * by the volumecontainer for now
    */
    QRCTreeWidget* volumeInfos_;

    static const std::string loggerCat_;

private slots:

    /// removes a volume
    void removeVolume();
    /// reloads a volume into memory
    void volumeRefresh(QTreeWidgetItem*);
    /// exports currently selected volumes
    void exportDat();

    void resizeOnCollapse(bool);

};

} // namespace

#endif

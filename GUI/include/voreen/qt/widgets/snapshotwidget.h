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

#ifndef VRN_SNAPSHOTWIDGET_H
#define VRN_SNAPSHOTWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QMap>

#include "voreen/qt/voreenqtdefine.h"

class QBoxLayout;
class QComboBox;
class QFrame;
class QPushButton;
class QSpinBox;
class QToolButton;

namespace voreen {

class CanvasRenderer;

class VRN_QT_API SnapshotElement : public QGroupBox {
Q_OBJECT
public:
    SnapshotElement(QWidget* parent = 0, CanvasRenderer* renderer = 0);

    virtual void updateFromProcessor();

protected slots:
    virtual void sizeComboChanged(int newIndex);
    void takeSnapshot();


protected:
    virtual void saveSnapshot(const QString& filename, int width, int height);

    QComboBox* sizeCombo_;
    QSpinBox* widthSpinBox_;
    QSpinBox* heightSpinBox_;
    QString savingPath_;

private:
    CanvasRenderer* canvasRenderer_;
    QPushButton* makeSnapshotButton_;
};

// ------------------------------------------------------------------------------------------------

class VRN_QT_API SnapshotWidget : public QWidget {
public:
    SnapshotWidget(QWidget* parent);

    void setCanvasRenderer(const std::vector<CanvasRenderer*>& canvasRenderer);
    void clearCanvasRenderer();

    void setPluginRenderer(const std::vector<SnapshotElement*>& pluginRenderer);
    void clearPluginRenderer();

private:
    void layoutElements();
    QBoxLayout* layout_;

    std::vector<CanvasRenderer*> canvasRenderer_;
    QFrame* line_;
    std::vector<SnapshotElement*> pluginRenderer_;
    QMap<CanvasRenderer*, SnapshotElement*> elements_;
};


} // namespace

#endif // VRN_SNAPSHOTWIDGET_H

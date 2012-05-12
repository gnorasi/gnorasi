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

#ifndef VRN_VOXELTYPEPROPERTYWIDGET_H
#define VRN_VOXELTYPEPROPERTYWIDGET_H

#include "voreen/qt/widgets/property/qpropertywidget.h"
#include <QWidget>

namespace voreen {

class VolumeHandleBase;
class VoxelTypeProperty;

class SliderSpinBoxWidget;
class DoubleSliderSpinBoxWidget;

class VoxelTypePropertyWidget : public QPropertyWidget {
Q_OBJECT;
public:
    VoxelTypePropertyWidget(VoxelTypeProperty* prop, QWidget* parent = 0, bool addVisibilityControl = true);
    virtual ~VoxelTypePropertyWidget();
    void updateFromProperty();
    
    void setVolume(const VolumeHandleBase* volume);

signals:
    void valueChanged(int);

protected:
    void createWidgets();
    void updateWidgetFromProperty();

    virtual void mousePressEvent(QMouseEvent*);

    VoxelTypeProperty* property_;
    const VolumeHandleBase* volumeHandle_;
    
    QWidget* widget_;
    std::vector<SliderSpinBoxWidget*> intWidgets_; 
    std::vector<DoubleSliderSpinBoxWidget*> floatWidgets_;
    
    QMenu* instantValueChangeMenu_;
    QAction* instantValueChangeAction_; 

protected slots:
    void updatePropertyFromWidget() const;

};

} // namespace

#endif // VRN_VOXELTYPEPROPERTYWIDGET_H

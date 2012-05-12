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

#ifndef VRN_VOLUMELOADBUTTON_H
#define VRN_VOLUMELOADBUTTON_H

#include <QPushButton>

#include "voreen/qt/widgets/volumeiohelper.h"

namespace voreen {

class VolumeContainer;

class VolumeLoadButton : public QPushButton {
    Q_OBJECT
public:
    VolumeLoadButton(VolumeContainer* vc, QWidget* parent = 0);
    ~VolumeLoadButton();

    void setVolumeContainer(VolumeContainer* vc);

signals:
    void volumeLoaded(const VolumeHandleBase* handle);

private slots:
    void loadActionTriggered(QAction*);

protected:
    QAction* loadDatAction_;
    VolumeIOHelper volumeIOHelper_;
};

} // namespace

#endif // VRN_VOLUMELOADBUTTON_H

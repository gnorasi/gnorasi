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

#ifndef VRN_VOLUMEVIEW_H
#define VRN_VOLUMEVIEW_H

#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumecontainer.h"
#include "voreen/qt/voreenqtdefine.h"

#include <QPixmap>

namespace voreen {

class VRN_QT_API VolumeViewHelper {
public:
    /**
     * Returns a Preview with the height of the given int
     * @param the height of the preview
     */
    static QPixmap generatePreview(const VolumeHandleBase*, int);

    /**
     * Returns a quadratic Preview with the height of the given int and white border
     * @param the height of the preview
     * @param the size of the border
     */
    static QPixmap generateBorderedPreview(const VolumeHandleBase*, int /*height*/, int /* border*/);

    /**
     * Returns a formatted std::string wich contains Volume Information
     */
    static std::string volumeInfoString(const VolumeHandleBase*);

    /**
     * Returns the Volumetype
     */
    static std::string getVolumeType(const Volume*);

    /**
     * Returns the Volumename (incl. path)
     */
    static std::string getVolumeName(const VolumeHandleBase*);

    /**
     * Returns the Volumename (excl. path)
     */
    static std::string getStrippedVolumeName(const VolumeHandleBase*);

    /**
     * Returns the Volumepath
     */
    static std::string getVolumePath(const VolumeHandleBase*);

    /**
     * Returns the Timestep
     */
    static std::string getVolumeTimestep(const VolumeHandleBase*);

    /**
     * Returns the dimension
     */
    static std::string getVolumeDimension(const VolumeHandleBase*);

    /**
     * Returns the spacing
     */
    static std::string getVolumeSpacing(const VolumeHandleBase*);

    /**
     * Returns a string desribing the amount of memory
     * that is actually occupied by the volume.
     */
    static std::string getVolumeMemorySize(const Volume*);

    static size_t getVolumeMemorySizeByte(const Volume*);

};

} // namespace

#endif

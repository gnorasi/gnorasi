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

#ifndef VRN_VVDVOLUMEWRITER_H
#define VRN_VVDVOLUMEWRITER_H

#include "voreen/core/io/volumewriter.h"
#include <sstream>

namespace voreen {

/**
 * Writes the volume into a .vvd and a .raw file (Voreen Volume Data, new Voreen format).
 */
class VRN_CORE_API VvdVolumeWriter : public VolumeWriter {
public:
    VvdVolumeWriter();
    virtual VolumeWriter* create(ProgressBar* progress = 0) const;

    virtual std::string getClassName() const   { return "VvdVolumeWriter"; }
    virtual std::string getFormatDescription() const { return "Voreen Volume Data (new Voreen format)"; }

    /**
     * Writes the data of a volume into a vvd- and a raw-file.
     *
     * @param fileName File name to be written
     * @param volume Volume dataset
     */
    virtual void write(const std::string& filename, const VolumeHandleBase* volumeHandle)
        throw (tgt::IOException);
private:
    static const std::string loggerCat_;
};

} // namespace voreen

#endif

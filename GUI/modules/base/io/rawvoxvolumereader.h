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

#ifndef VRN_RAWVOXVOLUMEREADER_H
#define VRN_RAWVOXVOLUMEREADER_H

#include "voreen/core/io/volumereader.h"

#include "tgt/vector.h"

namespace voreen {

/**
 * This is a reader for the .rawvox files exported by 3D coat (3d-coat.com).
 */
class RawVoxVolumeReader : public VolumeReader {
public:
    RawVoxVolumeReader() {
         extensions_.push_back("rawvox");
    }

    virtual VolumeReader* create(ProgressBar* progress = 0) const;

    virtual std::string getClassName() const   { return "RawVoxVolumeReader"; }
    virtual std::string getFormatDescription() const { return "3D coat .rawvox"; }

    virtual VolumeCollection* read(const std::string& url)
        throw (tgt::CorruptedFileException, tgt::IOException, std::bad_alloc);

private:
    static const std::string loggerCat_;
};

} // namespace voreen

#endif

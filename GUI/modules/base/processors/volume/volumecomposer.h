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

#ifndef VRN_IMAGESTACKER_H
#define VRN_IMAGESTACKER_H

#include "voreen/core/processors/volumeprocessor.h"

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/vectorproperty.h"

#include "voreen/core/ports/volumeport.h"

namespace voreen {

class ImageSequence;

/**
 * Constructs a volume from a sequence of 2D images.
 */
class VolumeComposer : public VolumeProcessor {
public:
    VolumeComposer();
    virtual Processor* create() const;

    virtual std::string getClassName() const  { return "VolumeComposer";  }
    virtual std::string getCategory() const   { return "Volume Processing"; }
    virtual CodeState getCodeState() const    { return CODE_STATE_EXPERIMENTAL;   }

    virtual bool isReady() const;

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    void stackImages();
    void stackVolumes();

    ImageSequencePort inportImages_;
    VolumeCollectionPort inportVolumes_;
    VolumePort outport_;

    FloatVec3Property voxelSpacing_;

    static const std::string loggerCat_; ///< category used in logging
};

}

#endif //VRN_IMAGESTACKER_H

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

#ifndef VRN_VOLUMESELECTOR_H
#define VRN_VOLUMESELECTOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/ports/volumeport.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/datastructures/volume/volumehandle.h"

namespace voreen {

class Volume;
class ProcessorWidgetFactory;

/**
 * Selects a single volume out of a input collection.
 */
class VolumeSelector : public Processor {

public:
    VolumeSelector();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "VolumeSelector";  }
    virtual std::string getCategory() const  { return "Utility";         }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }
    virtual bool isUtility() const           { return true; }

    virtual void invalidate(int inv = INVALID_RESULT);

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);

    IntProperty volumeID_;

    /// Inport for the volume collection.
    VolumeCollectionPort inport_;

    /// The volume port the selected volume is written to.
    VolumePort outport_;

    static const std::string loggerCat_;

private:
    void adjustToVolumeCollection();

};

} // namespace

#endif

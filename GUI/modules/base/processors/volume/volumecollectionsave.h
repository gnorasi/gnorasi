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

#ifndef VRN_VOLUMECOLLECTIONSAVE_H
#define VRN_VOLUMECOLLECTIONSAVE_H

#include "voreen/core/processors/volumeprocessor.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/ports/genericport.h"

#include <string>

namespace voreen {

class VolumeHandle;
class VolumeSerializer;
class VolumeSerializerPopulator;

class VolumeCollectionSave : public VolumeProcessor {
public:
    VolumeCollectionSave();
    virtual ~VolumeCollectionSave();
    virtual Processor* create() const;

    virtual std::string getClassName() const  { return "VolumeCollectionSave";  }
    virtual std::string getCategory() const   { return "Volume Processing";     }
    virtual CodeState getCodeState() const    { return CODE_STATE_EXPERIMENTAL; }
    virtual bool isEndProcessor() const       { return true; }

    virtual void saveCollection();

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);

private:
    VolumeCollectionPort inport_;

    FileDialogProperty outputDirectory_;
    BoolProperty continousSave_;
    ButtonProperty saveButton_;

    VolumeSerializerPopulator* volumeSerializerPopulator_;

    static const std::string loggerCat_; ///< category used in logging
};

} //namespace

#endif

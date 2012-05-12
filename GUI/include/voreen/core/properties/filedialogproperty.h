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

#ifndef VRN_FILEDIALOGPROPERTY_H
#define VRN_FILEDIALOGPROPERTY_H

#include "voreen/core/properties/stringproperty.h"

namespace voreen {

class VRN_CORE_API FileDialogProperty : public StringProperty {
public:

    enum FileMode {
        OPEN_FILE = 0,
        DIRECTORY = 1,
        SAVE_FILE = 2
    };

    FileDialogProperty(const std::string& id, const std::string& guiText, const std::string& dialogCaption,
                   const std::string& directory, const std::string& fileFilter = "", FileMode fileMode = OPEN_FILE,
                   Processor::InvalidationLevel invalidationLevel=Processor::INVALID_RESULT);
    FileDialogProperty();
    virtual ~FileDialogProperty() {}

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "FileDialogProperty"; }
    virtual std::string getTypeDescription() const { return "FileDialog"; }

    const std::string& getDialogCaption() const { return dialogCaption_; }
    const std::string& getDirectory() const { return directory_; }
    const std::string& getFileFilter() const { return fileFilter_; }
    FileMode getFileMode() const { return fileMode_; }

    /**
     * @see Property::serialize
     */
    virtual void serialize(XmlSerializer& s) const;

    /**
     * @see Property::deserialize
     */
    virtual void deserialize(XmlDeserializer& s);

protected:
    std::string dialogCaption_;
    std::string directory_;
    FileMode fileMode_;
    std::string fileFilter_;
};

}   // namespace

#endif

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

#include "geometrysave.h"

namespace voreen {

const std::string GeometrySave::loggerCat_("voreen.GeometrySave");

GeometrySave::GeometrySave()
    : Processor()
    , fileProp_("file", "Geometry file", "Save .vge file", "./", "Voreen geometry files (*.vge)", FileDialogProperty::SAVE_FILE)
    , saveButton_("save", "Save")
    , inport_(Port::INPORT, "inport")
{
    saveButton_.onChange(CallMemberAction<GeometrySave>(this, &GeometrySave::saveFile));

    addProperty(fileProp_);
    addProperty(saveButton_);

    addPort(inport_);
}

void GeometrySave::process() {}

void GeometrySave::saveFile() {
    XmlSerializer s;
    const Geometry* geometry = inport_.getData();
    s.serialize("Geometry", geometry);

    std::fstream stream(fileProp_.get().c_str(), std::ios::out);
    s.write(stream);
    stream.close();
}

} // namespace voreen

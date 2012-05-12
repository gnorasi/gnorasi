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

#ifndef VRN_GEOMETRYSAVE_H
#define VRN_GEOMETRYSAVE_H

#include "voreen/core/processors/processor.h"

#include "voreen/core/ports/geometryport.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/filedialogproperty.h"

#include <fstream>

namespace voreen {

class GeometrySave : public Processor {
public:
    GeometrySave();
    std::string getCategory() const { return "Geometry"; }
    std::string getClassName() const { return "GeometrySave"; }
    CodeState getCodeState() const { return Processor::CODE_STATE_TESTING; }
    Processor* create() const { return new GeometrySave(); }

    void process();

    bool isReady() const { return true; }

protected:
    void saveFile();

    // properties
    FileDialogProperty fileProp_;
    ButtonProperty saveButton_;

    GeometryPort inport_;

    static const std::string loggerCat_;
};

} // namespace voreen

#endif

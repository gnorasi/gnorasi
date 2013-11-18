/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef OTBCSVTEXTPORT_H
#define OTBCSVTEXTPORT_H

#include "voreen/core/voreencoreapi.h"
#include "voreen/core/ports/port.h"
#include "voreen/core/ports/textport.h"

namespace voreen {

class VRN_CORE_API OTBCSVTextPort : public TextPort {
public:
    explicit OTBCSVTextPort(PortDirection direction, const std::string& name,
                        bool allowMultipleConnections = false,
                        Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    ~OTBCSVTextPort();
    /**
     * Returns the file path
     */
    std::string DataPath() const;

    /**
     * Sets the file path, if it's an outport
     */
    void setDataPath(const std::string);

protected:

    std::string m_DataPath;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBCSVTEXTPORT_H

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
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

#ifndef VRN_OTBVECTORDATAPORT_H
#define VRN_OTBVECTORDATAPORT_H

#include "voreen/core/voreencoreapi.h"
#include "voreen/core/ports/port.h"

#include "otbVectorData.h"

namespace voreen {

class VRN_CORE_API OTBVectorDataPort : public Port {
public:
    explicit OTBVectorDataPort(PortDirection direction, const std::string& name,
                        bool allowMultipleConnections = false,
                        Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    ~OTBVectorDataPort();
    
    typedef otb::VectorData<double, 2> DataType;
    typedef DataType::Pointer DataSmartPointer;
    typedef DataType* DataPointer;

    virtual void setData(const DataPointer& pointer);

    virtual DataPointer getData() const;

    /// Returns true.
    virtual bool hasData() const;

    std::vector<const OTBVectorDataPort* > getConnected() const;

    /**
     * Returns true, if the port is connected
     */
    virtual bool isReady() const;

    /**
     * Returns the file path
     */
    std::string DataPath() const;

    /**
     * Sets the file path, if it's an outport
     */
    void setDataPath(const std::string);

protected:
    DataPointer portData_;

    std::string m_DataPath;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBVECTORDATAPORT_H

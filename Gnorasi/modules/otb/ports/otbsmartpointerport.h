/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

#ifndef VRN_OTBSMARTPOINTERPORT_H
#define VRN_OTBSMARTPOINTERPORT_H

#include "voreen/core/voreencoredefine.h"
#include "voreen/core/ports/port.h"
#include "itkSmartPointer.h"

namespace voreen {

template <class TObjectType> class VRN_CORE_API OTBSmartPointerPort : public Port {
public:
    typedef TObjectType ObjectType;
    explicit OTBSmartPointerPort(PortDirection direction, const std::string& name,
                        bool allowMultipleConnections = false,
                        Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    ~OTBSmartPointerPort();
    
    typedef itk::SmartPointer<ObjectType> SmartPointer;
    
    virtual void setData(const SmartPointer& pointer);

    virtual SmartPointer getData() const;

    /// Returns true.
    virtual bool hasData() const;

    //virtual std::vector<ImagePointer> getAllData() const;

    std::vector<const OTBSmartPointerPort* > getConnected() const;

    /**
     * Returns true, if the port is connected
     */
    virtual bool isReady() const;

protected:
    SmartPointer portData_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBSMARTPOINTERPORT_H

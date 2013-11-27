/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
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

#ifndef VRN_OTBSOMMAPPORT_H
#define VRN_OTBSOMMAPPORT_H

#include "voreen/core/voreencoreapi.h"
#include "voreen/core/ports/port.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "Statistics/itkEuclideanDistance.h"
#include "otbSOMMap.h"

namespace voreen {

class VRN_CORE_API OTBSOMMapPort : public Port {
public:
    explicit OTBSOMMapPort(PortDirection direction, const std::string& name,
                        bool allowMultipleConnections = false,
                        Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    ~OTBSOMMapPort();

    typedef double                          InputPixelType;

    typedef itk::VariableLengthVector<InputPixelType> PixelType;

    typedef itk::Statistics::EuclideanDistance<PixelType>  DistanceType;
    static const unsigned int VDimension = 2;
    typedef otb::SOMMap<PixelType, DistanceType, VDimension> SOMMapType;

    typedef SOMMapType::Pointer SOMMapSmartPointer;
    typedef SOMMapType* SOMMapPointer;


    virtual void setData(const SOMMapPointer& pointer);

    virtual SOMMapPointer getData() const;

    /// Returns true.
    virtual bool hasData() const;

    std::vector<const OTBSOMMapPort* > getConnected() const;

    /**
     * Returns true, if the port is connected
     */
    virtual bool isReady() const;

//    //!
//    std::string imagePath() const { return m_imagePath; }

//    //!
//    void setImagePath(const std::string p) { m_imagePath = p; }

protected:
    SOMMapPointer portData_;

    std::string m_imagePath;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace


#endif // VRN_OTBSOMMAPPORT_H

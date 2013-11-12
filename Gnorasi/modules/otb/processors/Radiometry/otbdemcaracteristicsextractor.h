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

#ifndef OTBDEMCARACTERISTICSEXTRACTOR_H
#define OTBDEMCARACTERISTICSEXTRACTOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "otbDEMCaracteristicsExtractor.h"
#include "voreen/core/properties/floatproperty.h"

namespace voreen {

class OTBDEMCaracteristicsExtractor : public OTBImageFilterProcessor
{
public:

    OTBDEMCaracteristicsExtractor();

    virtual ~OTBDEMCaracteristicsExtractor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Radiometry"; }
    virtual std::string getClassName() const { return "DEM Caracteristics Extractor"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::DEMCaracteristicsExtractor<ImageType, ImageType> DEMCaracteristicsExtractorFilter;
    DEMCaracteristicsExtractorFilter::Pointer extractor;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty solarAngle_;
    FloatProperty solarAzimut_;
    FloatProperty viewAngle_;
    FloatProperty viewAzimut_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBDEMCARACTERISTICSEXTRACTOR_H


/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#ifndef OTBDIMENSIONALITYREDUCTIONAPPLICATION_H
#define OTBDIMENSIONALITYREDUCTIONAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "itkImageToImageFilter.h"
#include "otbMaximumAutocorrelationFactorImageFilter.h"

#include "otbPCAImageFilter.h"
#include "otbNAPCAImageFilter.h"
#include "otbFastICAImageFilter.h"
#include "otbFastICAInternalOptimizerVectorImageFilter.h"

#include "otbLocalActivityVectorImageFilter.h"
#include "otbMaximumAutocorrelationFactorImageFilter.h"
//#include "otbVirtualDimensionality.h"
#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"



namespace voreen {

class OTBDimensionalityReductionApplication : public OTBImageFilterProcessor
{
public:
    OTBDimensionalityReductionApplication();

    virtual ~OTBDimensionalityReductionApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Dimensionality Reduction Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

//    typedef otb::VectorImage<double, 2> VectorImageType;




protected:

    virtual void setDescriptions() {
        setDescription("This application performs SAR calibration on input complex images.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

//    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

private:

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    BoolProperty noise_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBDIMENSIONALITYREDUCTIONAPPLICATION_H

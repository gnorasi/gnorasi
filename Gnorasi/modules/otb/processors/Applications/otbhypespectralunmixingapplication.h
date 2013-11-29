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

#ifndef OTBHYPESPECTRALUNMIXINGAPPLICATION_H
#define OTBHYPESPECTRALUNMIXINGAPPLICATION_H

#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/optionproperty.h"

#include "../BasicFilters/otbimagefilterprocessor.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbISRAUnmixingImageFilter.h"
#include "otbNCLSUnmixingImageFilter.h"
#include "otbMDMDNMFImageFilter.h"

#include "otbVectorImageToMatrixImageFilter.h"

namespace voreen {

class OTBHypespectralUnmixingApplication : public OTBImageFilterProcessor
{
public:
    OTBHypespectralUnmixingApplication();

    virtual ~OTBHypespectralUnmixingApplication();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Applications"; }
    virtual std::string getClassName() const { return "Hypespectral Unmixing Application"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;
    typedef otb::VectorImage<double, Dimension> VectorImageType;


    typedef otb::StreamingStatisticsVectorImageFilter<VectorImageType> StreamingStatisticsVectorImageFilterType;

    typedef otb::UnConstrainedLeastSquareImageFilter<VectorImageType, VectorImageType, double> UCLSUnmixingFilterType;
    UCLSUnmixingFilterType::Pointer UCLSunmixer;

    typedef otb::ISRAUnmixingImageFilter<VectorImageType, VectorImageType, double> ISRAUnmixingFilterType;
    ISRAUnmixingFilterType::Pointer ISRAunmixer;

    typedef otb::NCLSUnmixingImageFilter<VectorImageType, VectorImageType, double> NCLSUnmixingFilterType;
    NCLSUnmixingFilterType::Pointer NCLSunmixer;

    typedef otb::MDMDNMFImageFilter<VectorImageType, VectorImageType> MDMDNMFUnmixingFilterType;
    MDMDNMFUnmixingFilterType::Pointer MDMDNMFunmixer;

    typedef otb::VectorImageToMatrixImageFilter<VectorImageType> VectorImageToMatrixImageFilterType;
    VectorImageToMatrixImageFilterType::Pointer endMember2Matrix;

protected:

    virtual void setDescriptions() {
        setDescription("Estimate abundance maps from an hyperspectral image and a set of endmembers");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

private:

    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;
    OTBVectorImagePort endmembersInPort_;

    StringOptionProperty filter_; ///< Select unimixing algorithm

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBHYPESPECTRALUNMIXINGAPPLICATION_H

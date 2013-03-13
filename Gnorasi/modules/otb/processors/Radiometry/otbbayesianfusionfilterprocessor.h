/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.				*
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

#ifndef OTBBAYESIANFUSIONFILTERPROCESSOR_H
#define OTBBAYESIANFUSIONFILTERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbBayesianFusionFilter.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

namespace voreen {

class OTBBayesianFusionFilterProcessor : public Processor
{
public:
    OTBBayesianFusionFilterProcessor();

    virtual ~OTBBayesianFusionFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Radiometry"; }
    virtual std::string getClassName() const { return "Calculate Bayesian Fusion Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    //The image types are now defined using pixel types and particular dimension.
    //The panchromatic image is defined as an otb::Image and the multispectral one as otb::VectorImage.
    typedef double InternalPixelType;
    static const unsigned int Dimension = 2;
    typedef otb::Image<InternalPixelType, Dimension> PanchroImageType;
    typedef otb::VectorImage<InternalPixelType, Dimension> MultiSpecImageType;

    typedef double OutputPixelType;
    typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

    typedef otb::BayesianFusionFilter<MultiSpecImageType,MultiSpecImageType,
         PanchroImageType, OutputImageType> BayesianFusionFilterType;

    BayesianFusionFilterType::Pointer filter;

protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    void setLambda();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBVectorImagePort inMultiSpecPort_;
    OTBVectorImagePort inMultiSpecInterpPort_;
    OTBImagePort inPanchroPort_;

    OTBVectorImagePort outPort_;

    FloatProperty lambda_;	     //Typical λ value range in [0.5,1], where higher values yield sharper details. by default λ is set at 0.9999.


    static const std::string loggerCat_; ///< category used in logging

};

} // namespace

#endif // OTBBAYESIANFUSIONFILTERPROCESSOR_H







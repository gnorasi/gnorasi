/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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

#ifndef OTBIMAGETOSURFKEYPOINTSETFILTERPROCESSOR_H
#define OTBIMAGETOSURFKEYPOINTSETFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "otbimagefilterprocessor.h"
#include "otbImageToSURFKeyPointSetFilter.h"

namespace voreen {

class OTBImageToSURFKeyPointSetFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBImageToSURFKeyPointSetFilterProcessor();
    virtual ~OTBImageToSURFKeyPointSetFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Feature Extraction: SURF DETECTOR"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    //typedef otb::ImageToSURFKeyPointSetFilterProcessor<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> FilterType;

    //CHECK whether PointSetType is already declared somewhere else
    static const unsigned int Dimension = 2;
    typedef double        RealType;
    typedef itk::VariableLengthVector<RealType>      RealVectorType;
    typedef itk::PointSet<RealVectorType, Dimension> PointSetType;

    typedef otb::ImageToSURFKeyPointSetFilter<OTBImageFilterProcessor::ImageType, PointSetType> FilterType;

    FilterType::Pointer filter;

protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    //Gaussian Second order derivative Hessian images are computed in each
    //level and each octave for the input image.
    //For each octave, an extremum search is launched on each 3 adjacent scales.
    //The Key points detected are the ones extremum in the current , previous and
    //next scale of reserach. 3 scales are needed for the computation (NumberOfScales >=3).
    //Orientation and descriptors are computed for each key point detected.
    IntProperty octavesNumber_;
    IntProperty scalesNumber_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;


    static const std::string loggerCat_; ///< category used in logging
};

} //namespace

#endif // OTBIMAGETOSURFKEYPOINTSETFILTERPROCESSOR_H










/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
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

#ifndef OTBIMAGETOSURFKEYPOINTSETFILTERPROCESSOR_H
#define OTBIMAGETOSURFKEYPOINTSETFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbkeypointsetport.h" //CHECK HERE TO INCLUDE.
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

namespace voreen {

class OTBImageToSURFKeyPointSetFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBImageToSURFKeyPointSetFilterProcessor();
    virtual ~OTBImageToSURFKeyPointSetFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "SURF Key Point Detector"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef double RealType;
    static const unsigned int Dimension = 2;

    typedef otb::Image<RealType, Dimension>                             ImageType;
    typedef itk::VariableLengthVector<RealType>                         RealVectorType;
    typedef itk::PointSet<RealVectorType,Dimension>                     PointSetType;

    typedef PointSetType::PointsContainer                               PointsContainerType;
    typedef PointsContainerType::Iterator                               PointsIteratorType;
    typedef PointSetType::PointDataContainer                            PointDataContainerType;
    typedef PointDataContainerType::Iterator                            PointDataIteratorType;

protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    void update();

private:

    OTBImagePort inPort_;
//    OTBImagePort outPort_;
    OTBKeyPointSetPort outPort_;

    IntProperty                                                         m_octavesProperty;
    IntProperty                                                         m_scalesProperty;


    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBIMAGETOSURFKEYPOINTSETFILTERPROCESSOR_H

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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
 
#ifndef VRN_OTBIMAGETOSIFTKEYPOINTSETFILTERPROCESSOR_H
#define VRN_OTBIMAGETOSIFTKEYPOINTSETFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbkeypointsetport.h"
//#include "otbImageToSIFTKeyPointSetFilter.h"
//#include "otbKeyPointDensityImageFilter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

namespace voreen {
  
class OTBImageToSIFTKeyPointSetFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBImageToSIFTKeyPointSetFilterProcessor();
    virtual ~OTBImageToSIFTKeyPointSetFilterProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Implements the SIFT key point detector "; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    typedef double RealType;
    static const unsigned int Dimension = 2;

    typedef otb::Image<RealType, Dimension>                             ImageType;
    typedef itk::VariableLengthVector<RealType>                         RealVectorType;
    typedef itk::PointSet<RealVectorType,Dimension>                     PointSetType;

    //typedef itk::VariableLengthVector<double>                           RealVectorType;
    //typedef itk::PointSet<double,VDimension>                            PointSetType;

   //  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType,PointSetType>   ImageToSIFTKeyPointSetFilterType;

    //typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, ImageToSIFTKeyPointSetFilterType> DensFilterType;

    typedef PointSetType::PointsContainer                               PointsContainerType;
    typedef PointsContainerType::Iterator                               PointsIteratorType;
    typedef PointSetType::PointDataContainer                            PointDataContainerType;
    typedef PointDataContainerType::Iterator                            PointDataIteratorType;
    
//    ImageToSIFTKeyPointSetFilterType::Pointer                           filter;
//    DensFilterType::Pointer                                             densfilter;
        

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
    //OTBImagePort outPort_;
    OTBKeyPointSetPort outPort_;

    IntProperty                                                         m_octavesProperty;
    IntProperty                                                         m_scalesProperty;
    FloatProperty                                                       m_thresholdProperty;
    FloatProperty                                                       m_ratioProperty;
    IntProperty                                                         m_neighborhoodProperty;

    ButtonProperty                                                      m_updateProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBIMAGETOSIFTKEYPOINTSETFILTERPROCESSOR_H

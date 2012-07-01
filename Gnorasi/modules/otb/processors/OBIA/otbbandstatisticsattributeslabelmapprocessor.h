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

#ifndef VRN_OTBBANDSTATISTICSATTRIBUTESLABELMAPPPROCESSOR_H
#define VRN_OTBBANDSTATISTICSATTRIBUTESLABELMAPPPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../../ports/otblabelmapport.h"
#include "../../ports/otbvectorimageport.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"

namespace voreen {
  
class OTBBandStatisticsAttributesLabelMapProcessor : public Processor {
public:
    OTBBandStatisticsAttributesLabelMapProcessor();
    virtual ~OTBBandStatisticsAttributesLabelMapProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Calculate Band Statistics for Objects"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    virtual bool isReady() const;
    
    typedef unsigned long           LabelType;
    typedef double                  PixelType;
    typedef otb::Image<PixelType, 2>  ImageType;
    typedef otb::VectorImage<PixelType, 2>  VectorImageType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    typedef otb::BandsStatisticsAttributesLabelMapFilter<LabelMapType, VectorImageType>
							StatisticsLabelMapFilterType;

    StatisticsLabelMapFilterType::Pointer statisticsLabelMapFilter;
    
protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();
    void update();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    BoolProperty reducedProperties_; ///< Calculate reduced number of shape properties
    ButtonProperty update_;      ///< Executes clearImage().

private:

    OTBLabelMapPort inPort_;
    OTBVectorImagePort inVImage_;
    OTBLabelMapPort outPort_;
    OTBLabelMapPort outPort2_;
    OTBVectorImagePort outVImage_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBBANDSTATISTICSATTRIBUTESLABELMAPPPROCESSOR_H

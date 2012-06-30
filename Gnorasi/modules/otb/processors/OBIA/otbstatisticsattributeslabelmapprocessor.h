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

#ifndef VRN_OTBSTATISTICSATTRIBUTESLABELMAPPPROCESSOR_H
#define VRN_OTBSTATISTICSATTRIBUTESLABELMAPPPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "../../ports/otblabelmapport.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"
#include "otbStatisticsAttributesLabelMapFilter.h"

namespace voreen {
  
class OTBStatisticsAttributesLabelMapProcessor : public Processor {
public:
    OTBStatisticsAttributesLabelMapProcessor();
    virtual ~OTBStatisticsAttributesLabelMapProcessor();
    
    virtual Processor* create() const { return new OTBStatisticsAttributesLabelMapProcessor(); }
    
    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Calculate New Feature for Objects"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;
    virtual bool isReady() const;    
    
    typedef unsigned long           LabelType;
    typedef double                  PixelType;
    typedef otb::Image<PixelType, 2>  ImageType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    typedef otb::StatisticsAttributesLabelMapFilter<LabelMapType, ImageType>
							StatisticsLabelMapFilterType;

    StatisticsLabelMapFilterType::Pointer statisticsLabelMapFilter;
    
protected:
    void process();
    void update();
    void setFeatureName();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    BoolProperty reducedProperties_; ///< Calculate reduced number of shape properties
    StringProperty featureName_; ///< Set the name of the attribute added to the Object Map
    ButtonProperty update_;      ///< Executes update().
    
    std::string featureName;
    

private:

    OTBLabelMapPort inPort_;
    OTBImagePort inImage_;
    OTBLabelMapPort outPort_;
    OTBImagePort outImage_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBSTATISTICSATTRIBUTESLABELMAPPPROCESSOR_H

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

#ifndef VRN_OTBSHAPEATTRIBUTESLABELMAPPPROCESSOR_H
#define VRN_OTBSHAPEATTRIBUTESLABELMAPPPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../../ports/otblabelmapport.h"
#include "otbImage.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"
#include "otbShapeAttributesLabelMapFilter.h"

namespace voreen {
  
class OTBShapeAttributesLabelMapProcessor : public Processor {
public:
    OTBShapeAttributesLabelMapProcessor();
    virtual ~OTBShapeAttributesLabelMapProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Calculate Shape Properties for Objects"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    typedef unsigned long           LabelType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    typedef otb::ShapeAttributesLabelMapFilter<LabelMapType> ShapeLabelMapFilterType;

    ShapeLabelMapFilterType::Pointer shapeLabelMapFilter;

    /*!
     * \brief forceUpdate
     */
    void forceUpdate();
    
    
protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();
    void update();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    BoolProperty reducedProperties_; ///< Calculate reduced number of shape properties
    ButtonProperty update_;      ///< Updates the chain

    //! test
    BoolProperty automaticUpdate;

private:

    OTBLabelMapPort inPort_;
    OTBLabelMapPort outPort_;
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBSHAPEATTRIBUTESLABELMAPPPROCESSOR_H

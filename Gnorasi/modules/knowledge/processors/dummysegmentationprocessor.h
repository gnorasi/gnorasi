/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#ifndef VRN_DUMMYSEGMENTATIONPROCESSOR_H
#define VRN_DUMMYSEGMENTATIONPROCESSOR_H

#include "voreen/core/processors/renderprocessor.h"
#include "voreen/core/ports/textport.h"
//#include "voreen/core/processors/processorwidgetfactory.h"

//#include "voreen/core/properties/floatproperty.h"


namespace voreen {

class DummySegmentationProcessor : public RenderProcessor {
public:
    DummySegmentationProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Dummy Segmentation"; }
    virtual std::string getCategory() const  { return "Segmentation"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;

protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    RenderPort inport_;
    TextPort outport_;
    //FloatProperty saturation_;
};

} // namespace

#endif // VRN_VRN_DUMMYSEGMENTATIONPROCESSOR_H

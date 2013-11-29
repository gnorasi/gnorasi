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

#ifndef VRN_OTBIMAGEFILTERPROCESSOR_H
#define VRN_OTBIMAGEFILTERPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"



namespace voreen {

/**
 * Base class for image filter processors.
 *
 */
  
class OTBImageFilterProcessor : public Processor {
public:
    OTBImageFilterProcessor();
    virtual ~OTBImageFilterProcessor();
    
    static const unsigned int Dimension = 2;
    typedef double                   PixelType;
    typedef otb::Image<PixelType, Dimension> ImageType;
    typedef ImageType* 		     ImagePointer;
    
    virtual std::string getProcessorInfo() const;

protected:
    virtual void bypass(OTBImagePort *inport, OTBImagePort *outport); ///< Passes the image from inport to outport without changes.
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    BoolProperty enableSwitch_; ///< Should be used to control if bypass or actual image processing is applied

private:

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBIMAGEFILTERPROCESSOR_H

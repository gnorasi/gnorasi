/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
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

#ifndef VRN_OTBORTHORECTIFYPROCESSOR_H
#define VRN_OTBORTHORECTIFYPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/intproperty.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

#include "voreen/core/properties/buttonproperty.h"

namespace voreen {
  
class OTBOrthoRectifyProcessor : public Processor {
public:
    OTBOrthoRectifyProcessor();
    virtual ~OTBOrthoRectifyProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Geometry"; }
    virtual std::string getClassName() const { return "Ortho Rectification"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    
    typedef double PixelType;
    typedef otb::Image<PixelType> ImageType;
    typedef otb::VectorImage<PixelType> VectorImageType;

    void updateData();

    /// returns the link to the TextData object (inport data actually...)
    const std::string getCommandName() const { return pTextData; }

protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();
    void setRedBandIndex();
    void setNirBandIndex();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBVectorImagePort      inPort_;
    OTBVectorImagePort      outPort_;

    ButtonProperty      updateButton; /// saves the geospatial classification data in an rdf file

    std::string         pTextData;

//    IntProperty redBand_;	     ///< Input RED band selector
//    IntProperty nirBand_;	     ///< Input RED band selector

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBORTHORECTIFYPROCESSOR_H

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

#ifndef VRN_OTBSIMPLEVIEWERPROCESSOR_H
#define VRN_OTBSIMPLEVIEWERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "otbStandardImageViewer.h"

namespace voreen {

class OTBSimpleViewerProcessor : public Processor {
public:
    OTBSimpleViewerProcessor();
    virtual ~OTBSimpleViewerProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "SimpleImageViewer"; }
    virtual std::string getCategory() const  { return "Visualization"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;
    
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    
    typedef double                   DoublePixelType;
    typedef otb::Image<DoublePixelType, 2> ImageType;
    typedef ImageType* 		     ImagePointer;
    typedef otb::StandardImageViewer<ImageType> ViewerType;
    ViewerType::Pointer viewer;
    
     /**
     * Shows the image.
     *
     * @note The processor must have been initialized
     *       before calling this function.
     */
    void showImage();
    
    
protected:
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
  
    OTBImagePort inPort_;
    OTBImagePort outPort_;		///< passes the image directly to a next processor
    ButtonProperty showImageButton_;    ///< shows the image.
    
    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBSIMPLEVIEWERPROCESSOR_H

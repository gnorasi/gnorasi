/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.				*
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

#ifndef VRN_OTBCBAMICHANGEDETECTORPROCESSOR_H
#define VRN_OTBCBAMICHANGEDETECTORPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "otbCBAMIChangeDetector.h"

namespace voreen {
  
class OTBCBAMIChangeDetectorProcessor : public OTBImageFilterProcessor {
public:
    OTBCBAMIChangeDetectorProcessor();
    virtual ~OTBCBAMIChangeDetectorProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Change Detection"; }
    virtual std::string getClassName() const { return "Cumulant Approximation Mutual Information"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    // Define the dimension of the images
    static const unsigned int Dimension = 2;
        
    typedef double                                          InputPixelType;
    typedef double                                          OutputPixelType;
    typedef otb::Image<InputPixelType, Dimension>           InputImageType;
    typedef otb::Image<OutputPixelType, Dimension>          OutputImageType;

    typedef otb::CBAMIChangeDetector<InputImageType,
            InputImageType, OutputImageType>                CBAMINDetectorType;

    CBAMINDetectorType::Pointer                             cbamidetector;


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
    OTBImagePort inPort1_;
    OTBImagePort outPort_;

    IntProperty m_radiusProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBCBAMICHANGEDETECTORPROCESSOR_H

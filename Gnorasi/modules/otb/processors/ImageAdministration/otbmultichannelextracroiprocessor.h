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

#ifndef VRN_OTBMULTICHANNELEXTRACTROIPROCESSOR_H
#define VRN_OTBMULTICHANNELEXTRACTROIPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "otbMultiChannelExtractROI.h"

namespace voreen {
  
class OTBMultiChannelExtractROIProcessor : public OTBImageFilterProcessor {
public:
    OTBMultiChannelExtractROIProcessor();
    virtual ~OTBMultiChannelExtractROIProcessor();
    
    virtual Processor* create() const;
    
    virtual std::string getCategory() const { return "Image Administration"; }

    virtual std::string getClassName() const { return "Multi Channel Extract ROI"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL
    
    virtual std::string getProcessorInfo() const;

    // Define the dimension of the images
    static const unsigned int Dimension = 2;
        
    typedef double                                          InputPixelType;
    typedef double                                          OutputPixelType;
    typedef otb::VectorImage<InputPixelType, Dimension>     InputImageType;
    typedef otb::VectorImage<OutputPixelType, Dimension>    OutputImageType;

    typedef otb::MultiChannelExtractROI<InputPixelType,
        InputPixelType>                                     MultiChannelExtractROIType;


protected:
    virtual void setDescriptions() {
	setDescription("processor.");
    }
    void process();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    void update();

    void updateUseSingleChannel();
    void updateUseMultipleChannel();

private:

    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;

    IntProperty         m_singleChannelProperty;
    IntProperty         m_channelFromProperty;
    IntProperty         m_channelEndProperty;
    BoolProperty        m_useSingleChannelProperty;
    BoolProperty        m_useMultipleChannelProperty;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBMULTICHANNELEXTRACTROIPROCESSOR_H

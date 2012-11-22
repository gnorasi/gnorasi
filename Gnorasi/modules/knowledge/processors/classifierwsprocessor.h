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

#ifndef VRN_CLASSIFIERWSPROCESSOR_H
#define VRN_CLASSIFIERWSPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/ports/textport.h"


namespace voreen {

class ClassifierWSProcessor : public Processor {
public:
    ClassifierWSProcessor();
    virtual ~ClassifierWSProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "ClassifierWS"; }
    virtual std::string getCategory() const  { return "Classification"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }
    //virtual bool usesExpensiveComputation() const { return true; }
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    virtual std::string getProcessorInfo() const;

    /// sets the link to the TextData object after calling WS (outport data actually)
    void setTextDataOut(std::string);
    /// returns the link to the TextData object (inport data actually...)
    const std::string getTextData() const;



protected:
    virtual void setDescriptions() {
	setDescription("Web Service Classification processor.");
    }
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:
    void setOutPortData();
    void readData();
    void updateView();

    TextPort inPort_;
    TextPort outPort_;

    std::string pTextData_;
    std::string pTextDataOut_;

    StringProperty serverURLupdate_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_CLASSIFIERWSPROCESSOR_H


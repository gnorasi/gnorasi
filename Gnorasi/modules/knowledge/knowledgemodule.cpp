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

#include "knowledgemodule.h"
#include "processors/dummysegmentationprocessor.h"
#include "processors/classifierwsprocessor.h"
#include "processors/retrieveclassificationdataprocessor.h"

#include "processors/ontologycreatorprocessor.h"
#include "processors/geospatialclassificationprocessor.h"

#include "processors/knowledgewebserviceprocessor.h"

#include "processors/fuzzyprocessor.h"
#include "processors/fuzzydefinitionprocessor.h"

namespace voreen {

KnowledgeModule::KnowledgeModule(const std::string& moduleName) 
    : VoreenModule(moduleName)
{
    // module name to be used in the GUI
    setName("Knowledge");
    
    // each module processor needs to be registered
    registerProcessor(new DummySegmentationProcessor());
    registerProcessor(new ClassifierWSProcessor());
    registerProcessor(new RetrieveClassificationDataProcessor());

    registerProcessor(new OntologyCreatorProcessor());
    registerProcessor(new GeospatialClassificationProcessor());
    registerProcessor(new KnowledgeWebServiceProcessor());

    registerProcessor(new FuzzyProcessor());
    registerProcessor(new FuzzyDefinitionProcessor());

    // adds the module source glsl dir to the shader search path
    //addShaderPath(getModulesPath("knowledge/processors/glsl"));
}

} // namespace

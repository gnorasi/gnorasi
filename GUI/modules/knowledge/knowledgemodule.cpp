/**********************************************************************
 *	GNORASI                                                           *
 *                                                                    *
 **********************************************************************/

#include "knowledgemodule.h"
#include "processors/dummysegmentationprocessor.h"
#include "processors/classifierwsprocessor.h"

namespace voreen {

KnowledgeModule::KnowledgeModule() 
    : VoreenModule()
{
    // module name to be used in the GUI
    setName("Knowledge");
    
    // module description file
    setXMLFileName("knowledge/knowledgemodule.xml");

    // each module processor needs to be registered
    addProcessor(new DummySegmentationProcessor());
    addProcessor(new ClassifierWSProcessor());

    // adds the module source glsl dir to the shader search path
    //addShaderPath(getModulesPath("knowledge/processors/glsl"));
}

} // namespace

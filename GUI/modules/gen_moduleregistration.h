// WARNING: This file is auto-generated!
#include "voreen/core/voreenapplication.h"
// module class headers
#include "modules/knowledge/knowledgemodule.h"
#include "modules/otb/otbmodule.h"
// instantiate module classes
namespace voreen {
void addAllModules(VoreenApplication* vapp) {
(*vapp).addModule(new KnowledgeModule());
(*vapp).addModule(new OTBModule());
}}

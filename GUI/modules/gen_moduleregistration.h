// WARNING: This file is auto-generated!
#include "voreen/core/voreenapplication.h"
// module class headers
#include "modules/base/basemodule.h"
#include "modules/zip/zipmodule.h"
#include "modules/devil/devilmodule.h"
#include "modules/knowledge/knowledgemodule.h"
// instantiate module classes
namespace voreen {
void addAllModules(VoreenApplication* vapp) {
(*vapp).addModule(new BaseModule());
(*vapp).addModule(new ZIPModule());
(*vapp).addModule(new DevILModule());
(*vapp).addModule(new KnowledgeModule());
}}

// WARNING: This file is auto-generated!
#include "voreen/qt/voreenapplicationqt.h"
// module class headers
#include "modules/base/basemoduleqt.h"
#include "modules/knowledge/knowledgemoduleqt.h"
// instantiate Qt module classes
namespace voreen {
void addAllQtModules(VoreenApplicationQt* vappQt) {
(*vappQt).addQtModule(new BaseModuleQt());
(*vappQt).addQtModule(new KnowledgeModuleQt());
}}

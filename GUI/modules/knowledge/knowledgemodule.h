/**********************************************************************
 * GNORASI                                                            *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_KNOWLEDGEMODULE_H
#define VRN_KNOWLEDGEMODULE_H

#include "voreen/core/voreenmodule.h"

namespace voreen {

class KnowledgeModule : public VoreenModule {

public:
    KnowledgeModule();

    virtual std::string getDescription() const { 
        return "GNORASI Knowledge Module."; 
    }
};

} // namespace

#endif // VRN_TESTMODULE_H

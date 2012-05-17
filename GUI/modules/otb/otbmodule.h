/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_OTBMODULE_H
#define VRN_OTBMODULE_H

#include "voreen/core/voreenmodule.h"

namespace voreen {

class OTBModule : public VoreenModule {

public:
    OTBModule();

    virtual std::string getDescription() const { 
        return "GNORASI Remote Sensing Module."; 
    }
};

} // namespace

#endif // VRN_OTBMODULE_H

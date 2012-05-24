/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "otbmodule.h"
//#include "processors/dummysegmentationprocessor.h"
#include "processors/imagereaderprocessor.h"
#include "processors/imagewriterprocessor.h"

namespace voreen {

OTBModule::OTBModule() 
    : VoreenModule()
{
    // module name to be used in the GUI
    setName("Orfeo Toolbox");
    
    // module description file
    setXMLFileName("otb/otbmodule.xml");

    // each module processor needs to be registered
    //addProcessor(new DummySegmentationProcessor());
    addProcessor(new ImageReaderProcessor());
    addProcessor(new ImageWriterProcessor());
}

} // namespace

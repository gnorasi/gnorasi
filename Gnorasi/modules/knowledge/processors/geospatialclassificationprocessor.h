#ifndef VRN_GEOSPATIALCLASSIFICATIONPROCESSOR_H
#define VRN_GEOSPATIALCLASSIFICATIONPROCESSOR_H

#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/floatproperty.h"
#include "tgt/shadermanager.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

//#include "voreen/core/ports/textport.h"
#include "../ports/ontologyport.h"
#include "../ports/georuleport.h"

namespace voreen {

class VRN_CORE_API GeospatialClassificationProcessor : public Processor {
public:
    GeospatialClassificationProcessor();

    // virtual constructor
    virtual Processor* create() const;

    // documentary functions
    virtual std::string getClassName() const { return "GeospatialClassificationProcessor";       }
    virtual std::string getCategory() const  { return "Classification";      }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

    void setOutportText(std::string d) { outPort_.setData(d); }

    //
    void openXML();

    //
    void writeXML();

    //
    void fetchOntologyData();


    /// returns the link to the TextData object (inport data actually...)
    const std::string getCommandName() const { return pTextData_; }

    /// returns the link to the TextData object (inport data actually...)
    const std::string getFilePath() const { return pFilePath_; }

protected:
    virtual void setDescriptions() {
        setDescription("Geospatial classification processor");
    }

    // actual rendering code
    virtual void process();

    /// loads the shader
    virtual void initialize() throw (VoreenException); 
    
    /// frees the shader
    virtual void deinitialize() throw (VoreenException);

private:
    void updateView();

    // ports and properties
    bool hasFileName;

    GeoRulePort outPort_;
    OntologyPort inPort_;

    ButtonProperty      saveFile_; /// saves the geospatial classification data in an rdf file

    FileDialogProperty  openFile_; /// open an xml file , opens a dialgo

    std::string pTextData_;

    std::string pFilePath_;

    static const std::string loggerCat_; ///< category used in loggin
};

} // namespace

#endif // VRN_GEOSPATIALCLASSIFICATIONPROCESSOR_H

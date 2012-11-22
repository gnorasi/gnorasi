#ifndef VRN_FEATURECLASSIFICATIONPROCESSOR_H
#define VRN_FEATURECLASSIFICATIONPROCESSOR_H

#include "voreen/core/processors/processor.h"

#include "voreen/core/properties/floatproperty.h"
#include "tgt/shadermanager.h"

#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"

#include "voreen/core/ports/textport.h"

namespace voreen {

class VRN_CORE_API FeatureClassificationProcessor : public Processor {
public:
    FeatureClassificationProcessor();

    // virtual constructor
    virtual Processor* create() const;

    // documentary functions
    virtual std::string getClassName() const { return "FeatureClassificationProcessor";       }
    virtual std::string getCategory() const  { return "Ontology";      }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

    /// sets the link to the TextData object after calling WS (outport data actually)
//    void setTextDataOut(std::string);

    /// returns the link to the TextData object (inport data actually...)
    const std::string getFilePath() const;

    //
    void loadRDF();

    void writeRDF();

    void newOntologyContent();

    void setOutportText(std::string d) { outPort_.setData(d); }

protected:
    virtual void setDescriptions() {
        setDescription("Ontology creator processor");
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

    TextPort outPort_;

    std::string pTextData_;

    FileDialogProperty OWLFile_;  ///< Path of the RDF file.

    ButtonProperty saveFile_; /// saves the ontolgy data in an rdf file
    ButtonProperty newOntology_; /// new ontolgy data in an rdf file

    static const std::string loggerCat_; ///< category used in loggin
};

} // namespace

#endif // VRN_FEATURECLASSIFICATIONPROCESSOR_H

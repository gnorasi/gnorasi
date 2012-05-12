/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "voreen/core/voreenmodule.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/core/ports/port.h"
#include "voreen/core/processors/processor.h"
#include "voreen/core/processors/processorwidgetfactory.h"
#include "voreen/core/properties/property.h"
#include "voreen/core/properties/propertyfactory.h"
#include "voreen/core/properties/propertywidget.h"
#include "voreen/core/properties/propertywidgetfactory.h"
#include "voreen/core/properties/link/linkevaluatorfactory.h"
#include "voreen/core/io/volumereader.h"
#include "voreen/core/io/volumewriter.h"
#include "voreen/core/io/serialization/serializablefactory.h"

#include "tgt/shadermanager.h"
#include "tinyxml/tinyxml.h"

namespace voreen {

const std::string VoreenModule::loggerCat_("voreen.VoreenModule");

VoreenModule::VoreenModule()
    : name_("undefined")
    , documentationFileName_("undefined")
    , initialized_(false)
{}

VoreenModule::~VoreenModule() {

    if (isInitialized()) {
        LWARNING("Module '" << getName() << "' has not been deinitialized before destruction");
    }

    // free resources
    for (size_t i=0; i<processors_.size(); i++)
        delete processors_[i];
    processors_.clear();

    for (size_t i=0; i<properties_.size(); i++)
        delete properties_[i];
    properties_.clear();

    for (size_t i=0; i<volumeReaders_.size(); i++)
        delete volumeReaders_[i];
    volumeReaders_.clear();

    for (size_t i=0; i<volumeWriters_.size(); i++)
        delete volumeWriters_[i];
    volumeWriters_.clear();

    for (size_t i=0; i<serializerFactories_.size(); i++)
        delete serializerFactories_[i];
    serializerFactories_.clear();

    for (size_t i=0; i<processorWidgetFactories_.size(); i++)
        delete processorWidgetFactories_[i];
    processorWidgetFactories_.clear();

    for (size_t i=0; i<propertyWidgetFactories_.size(); i++)
        delete propertyWidgetFactories_[i];
    propertyWidgetFactories_.clear();

    // each link evaluator factory is also registered as serializable factory
    // and has thus already been deleted!
    /*for (size_t i=0; i<linkEvaluatorFactories_.size(); i++)
        delete linkEvaluatorFactories_[i];
    linkEvaluatorFactories_.clear(); */
}

std::string voreen::VoreenModule::getName() const {
    return name_;
}

const std::vector<Processor*>& VoreenModule::getProcessors() const {
    return processors_;
}

const std::vector<Property*>& VoreenModule::getProperties() const {
    return properties_;
}

const std::vector<VolumeReader*>& VoreenModule::getVolumeReaders() const {
    return volumeReaders_;
}

const std::vector<VolumeWriter*>& VoreenModule::getVolumeWriters() const {
    return volumeWriters_;
}

const std::vector<SerializableFactory*>& VoreenModule::getSerializerFactories() const {
    return serializerFactories_;
}

const std::vector<ProcessorWidgetFactory*>& VoreenModule::getProcessorWidgetFactories() const {
    return processorWidgetFactories_;
}

const std::vector<PropertyWidgetFactory*>& VoreenModule::getPropertyWidgetFactories() const {
    return propertyWidgetFactories_;
}

const std::vector<LinkEvaluatorFactory*>& VoreenModule::getLinkEvaluatorFactories() const {
    return linkEvaluatorFactories_;
}

const std::vector<std::string>& VoreenModule::getShaderPaths() const {
    return shaderPaths_;
}


// protected

void VoreenModule::initialize() throw (tgt::Exception) {
    for (size_t i=0; i<shaderPaths_.size(); i++) {
        ShdrMgr.addPath(shaderPaths_.at(i));
    }

    // create and register property factory
    if (!properties_.empty()) {
        PropertyFactory* propFactory = new PropertyFactory();
        for (size_t i=0; i<properties_.size(); i++)
            propFactory->registerProperty(properties_[i]);
        addSerializerFactory(propFactory);
    }
}

void voreen::VoreenModule::deinitialize() throw (tgt::Exception)
{ }

void VoreenModule::setName(const std::string& name) {
    name_ = name;

    if (documentationFileName_ == "undefined")
        documentationFileName_ = name_ + ".xml";
}

void VoreenModule::setXMLFileName(const std::string& xmlFileName, bool absolutePath) {
    if (absolutePath)
        documentationFileName_ = xmlFileName;
    else
        documentationFileName_ = getModulesPath() + "/" + xmlFileName;
}

void VoreenModule::addProcessor(Processor* processor) {
    tgtAssert(processor, "null pointer passed");
    if (name_ == "undefined") {
        LWARNING("addProcessor(): module name is undefined");
    }
    processor->setModuleName(name_);
    processors_.push_back(processor);
}

void VoreenModule::addProperty(Property* property) {
    tgtAssert(property, "null pointer passed");
    properties_.push_back(property);
}

void VoreenModule::addVolumeReader(VolumeReader* reader) {
    tgtAssert(reader, "null pointer passed");
    volumeReaders_.push_back(reader);
}

void VoreenModule::addVolumeWriter(VolumeWriter* writer) {
    tgtAssert(writer, "null pointer passed");
    volumeWriters_.push_back(writer);
}

void VoreenModule::addSerializerFactory(SerializableFactory* factory) {
    tgtAssert(factory, "null pointer passed");
    serializerFactories_.push_back(factory);
}

void VoreenModule::addShaderPath(const std::string& path) {
    shaderPaths_.push_back(path);
}

void VoreenModule::addProcessorWidgetFactory(ProcessorWidgetFactory* factory) {
    tgtAssert(factory, "null pointer passed");
    processorWidgetFactories_.push_back(factory);
}

void VoreenModule::addPropertyWidgetFactory(PropertyWidgetFactory* factory) {
    tgtAssert(factory, "null pointer passed");
    propertyWidgetFactories_.push_back(factory);
}

void VoreenModule::addLinkEvaluatorFactory(LinkEvaluatorFactory* factory) {
    tgtAssert(factory, "null pointer passed");
    linkEvaluatorFactories_.push_back(factory);

    // link evaluator factories are also serializable factories
    serializerFactories_.push_back(factory);
}

std::string VoreenModule::getModulesPath(const std::string& suffix) const {
    if (!VoreenApplication::app()) {
        LERROR("getModulesPath(): VoreenApplication not instantiated");
        return "";
    }
    return VoreenApplication::app()->getModulePath(suffix);
}

bool VoreenModule::isInitialized() const {
    return initialized_;
}

TiXmlElement* getProcessorElement(TiXmlDocument& doc, const std::string& className) {
    TiXmlElement* processorElem = doc.FirstChildElement("Processor");
    const std::string* name;

    while (processorElem) {
        name = processorElem->Attribute(std::string("name"));
        if (*name == className)
            return processorElem;
        processorElem = processorElem->NextSiblingElement();
    }
    return 0;
}

TiXmlElement* getPropertyElement(TiXmlElement* processorElem, const std::string& propertyID) {
    tgtAssert(processorElem, "passed null pointer");
    TiXmlElement* propertyElem = processorElem->FirstChildElement(std::string("Property"));

    while (propertyElem) {
        const std::string* propertyName = propertyElem->Attribute(std::string("id"));
        if (*propertyName == propertyID)
            return propertyElem;

        propertyElem = propertyElem->NextSiblingElement("Property");
    }
    return 0;
}

TiXmlElement* getPortElement(TiXmlElement* processorElem, const std::string& portName) {
    tgtAssert(processorElem, "passed null pointer");
    TiXmlElement* portElem = processorElem->FirstChildElement(std::string("Port"));

    while (portElem) {
        const std::string* iterPortName = portElem->Attribute(std::string("name"));
        if (*iterPortName == portName)
            return portElem;

        portElem = portElem->NextSiblingElement("Port");
    }
    return 0;
}

std::string toLowerCase(const std::string& input) {
    std::string result = input;
    for (size_t i = 0; i < input.size(); ++i)
        result[i] = tolower(input[i]);
    return result;
}

void insertNewProcessorElement(TiXmlDocument& doc, const std::string& className) {
    TiXmlElement* processorElem = new TiXmlElement("Processor");
    processorElem->SetAttribute("name", className);

    TiXmlElement* iteratorElem = doc.FirstChildElement("Processor");
    if (!iteratorElem) {
        // Not a single item exists in the list
        doc.InsertEndChild(*processorElem);
        return;
    }

    const std::string* iteratorName;
    bool inserted = false;

    while (iteratorElem) {
        iteratorName = iteratorElem->Attribute(std::string("name"));
        int comparison = toLowerCase(className).compare(toLowerCase(*iteratorName));
        if (comparison < 0) {
            doc.InsertBeforeChild(iteratorElem, *processorElem);
            inserted = true;
            break;
        }
        iteratorElem = iteratorElem->NextSiblingElement("Processor");
    }
    if (!inserted)
        doc.InsertEndChild(*processorElem);
}

void insertNewPropertyElement(TiXmlElement* processorElem, const std::string& propertyID) {
    TiXmlElement* propertyElem = new TiXmlElement("Property");
    propertyElem->SetAttribute("id", propertyID);

    TiXmlElement* iteratorElem = processorElem->FirstChildElement("Property");
    if (!iteratorElem) {
        // Not a single item exists in the list
        processorElem->InsertEndChild(*propertyElem);
        return;
    }

    const std::string* iteratorID;
    bool inserted = false;

    while (iteratorElem) {
        iteratorID = iteratorElem->Attribute(std::string("id"));
        int comparison = toLowerCase(propertyID).compare(toLowerCase(*iteratorID));
        if (comparison < 0) {
            processorElem->InsertBeforeChild(iteratorElem, *propertyElem);
            inserted = true;
            break;
        }
        iteratorElem = iteratorElem->NextSiblingElement("Property");
    }

    if (!inserted)
        processorElem->InsertEndChild(*propertyElem);
}

void insertNewPortElement(TiXmlElement* processorElem, const std::string& portName) {
    TiXmlElement* portElem = new TiXmlElement("Port");
    portElem->SetAttribute("name", portName);

    TiXmlElement* iteratorElem = processorElem->FirstChildElement("Port");
    if (!iteratorElem) {
        // Not a single item exists in the list
        processorElem->InsertEndChild(*portElem);
        return;
    }

    const std::string* iteratorName;
    bool inserted = false;

    while (iteratorElem) {
        iteratorName = iteratorElem->Attribute(std::string("name"));
        int comparison = toLowerCase(portName).compare(toLowerCase(*iteratorName));
        if (comparison < 0) {
            processorElem->InsertBeforeChild(iteratorElem, *portElem);
            inserted = true;
            break;
        }
        iteratorElem = iteratorElem->NextSiblingElement("Port");
    }

    if (!inserted)
        processorElem->InsertEndChild(*portElem);
}

std::string VoreenModule::getDescription() const {
    TiXmlDocument doc(documentationFileName_);
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* descriptionElem = doc.FirstChildElement("ModuleDescription");
        if (descriptionElem)
            return descriptionElem->GetText();
        else
            return "";
    }
    else {
        if (doc.ErrorId() != 13) // no warning if the document is empty
            LWARNING("Could not open XML documentation file for module '" + getName() + "': " + doc.ErrorDesc());
        return "";
    }
}

void VoreenModule::setDescription(const std::string& description) const {
    TiXmlDocument doc(documentationFileName_);
    doc.LoadFile();

    TiXmlElement* descriptionElem = doc.FirstChildElement("ModuleDescription");
    if (descriptionElem)
        doc.RemoveChild(descriptionElem);

    if (description != "") {
        TiXmlElement* newDescriptionElem = new TiXmlElement("ModuleDescription");
        TiXmlText* descriptionText = new TiXmlText(description);
        newDescriptionElem->LinkEndChild(descriptionText);

        if (doc.NoChildren())
            doc.LinkEndChild(newDescriptionElem);
        else
            doc.InsertBeforeChild(doc.FirstChildElement(), *newDescriptionElem);
    }
    doc.SaveFile();
}

std::string VoreenModule::getDocumentationDescription(const std::string& className) const {
    TiXmlDocument doc(documentationFileName_);
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* processorElem = getProcessorElement(doc, className);
        if (processorElem) {
            TiXmlElement* descriptionElem = processorElem->FirstChildElement(std::string("Description"));
            if (descriptionElem) {
                const char* description = descriptionElem->GetText();
                if (description)
                    return std::string(description);
            }
        }
        return "";
    }
    else {
        if (doc.ErrorId() != 13) // no warning if the document is empty
            LWARNING("Could not open XML documentation file for module '" + getName() + "': " + doc.ErrorDesc());
        return "";
    }
}

void VoreenModule::setDocumentationDescription(const std::string& className, const std::string& description) const {
    TiXmlDocument doc(documentationFileName_);
    doc.LoadFile();
    TiXmlElement* processorElem = getProcessorElement(doc, className);

    // If the description is empty, we only want to delete an existing description element
    if (description == "") {
        if (processorElem) {
            TiXmlElement* descriptionElem = processorElem->FirstChildElement(std::string("Description"));
            if (descriptionElem)
                processorElem->RemoveChild(descriptionElem);

            if (processorElem->NoChildren())
                doc.RemoveChild(processorElem);
            doc.SaveFile();
        }
        return;
    }

    if (!processorElem)
        insertNewProcessorElement(doc, className);
    processorElem = getProcessorElement(doc, className);

    if (processorElem) {
        TiXmlElement* descriptionElem = processorElem->FirstChildElement(std::string("Description"));
        if (descriptionElem)
            processorElem->RemoveChild(descriptionElem);
        descriptionElem = new TiXmlElement("Description");
        TiXmlText* text = new TiXmlText(description);
        descriptionElem->LinkEndChild(text);
        if (processorElem->NoChildren())
            processorElem->LinkEndChild(descriptionElem);
        else
            processorElem->InsertBeforeChild(processorElem->FirstChild(), *descriptionElem);
    }
    else {
        insertNewProcessorElement(doc, className);
        processorElem = getProcessorElement(doc, className);
        TiXmlElement* descriptionElem = new TiXmlElement("Description");
        TiXmlText* text = new TiXmlText(description);
        descriptionElem->LinkEndChild(text);
        processorElem->LinkEndChild(descriptionElem);
    }
    doc.SaveFile();
}

std::vector<std::pair<std::string, std::string> > VoreenModule::getDocumentationPorts(const std::string& className) const {
    std::vector<std::pair<std::string, std::string> > result;

    TiXmlDocument doc(documentationFileName_);
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* processorElem = doc.FirstChildElement("Processor");
        const std::string* name;

        while (processorElem) {
            name = processorElem->Attribute(std::string("name"));
            if (*name == className) {
                TiXmlElement* portElem = processorElem->FirstChildElement(std::string("Port"));

                while (portElem) {
                    const std::string portName = *(portElem->Attribute(std::string("name")));
                    const char* portDescription = portElem->GetText();
                    if (portDescription)
                        result.push_back(std::make_pair(portName, std::string(portDescription)));
                    portElem = portElem->NextSiblingElement(std::string("Port"));
                }
                break;
            }
            processorElem = processorElem->NextSiblingElement("Processor");
        }

        return result;
    }
    else {
        if (doc.ErrorId() != 13) // no warning if the document is empty
            LWARNING("Could not open XML documentation file for module '" + getName() + "': " + doc.ErrorDesc());
        return std::vector<std::pair<std::string, std::string> >();
    }
}

std::string VoreenModule::getDocumentationPort(const std::string& className, const std::string& portName) const {
    TiXmlDocument doc(documentationFileName_);
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* processorElem = getProcessorElement(doc, className);
        if (processorElem) {
            TiXmlElement* portElem = getPortElement(processorElem, portName);
            if (portElem) {
                const char* portText = portElem->GetText();
                if (portText)
                    return std::string(portText);
            }
        }
        return "";
    }
    else {
        if (doc.ErrorId() != 13) // no warning if the document is empty
            LWARNING("Could not open XML documentation file for module '" + getName() + "': " + doc.ErrorDesc());
        return "";
    }}

void VoreenModule::setDocumentationPort(const std::string& className, const std::string& portName, const std::string& description) const {
    TiXmlDocument doc(documentationFileName_);
    doc.LoadFile();
    TiXmlElement* processorElem = getProcessorElement(doc, className);

    // If the description is empty, we only want to delete an existing description element
    if (description == "") {
        if (processorElem) {
            TiXmlElement* portElem = getPortElement(processorElem, portName);
            if (portElem)
                processorElem->RemoveChild(portElem);

            if (processorElem->NoChildren())
                doc.RemoveChild(processorElem);
            doc.SaveFile();
        }
        return;
    }

    if (!processorElem) {
        insertNewProcessorElement(doc, className);
        processorElem = getProcessorElement(doc, className);
    }

    TiXmlElement* portElem = getPortElement(processorElem, portName);
    if (!portElem) {
        insertNewPortElement(processorElem, portName);
        portElem = getPortElement(processorElem, portName);
    }
    else
        portElem->RemoveChild(portElem->FirstChild());

    TiXmlText* portText = new TiXmlText(description);
    portElem->LinkEndChild(portText);
    doc.SaveFile();
}

std::vector<std::pair<std::string, std::string> > VoreenModule::getDocumentationProperties(const std::string& className) const {
    std::vector<std::pair<std::string, std::string> > result;

    TiXmlDocument doc(documentationFileName_);
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* processorElem = doc.FirstChildElement("Processor");
        const std::string* name;

        while (processorElem) {
            name = processorElem->Attribute(std::string("name"));
            if (*name == className) {
                TiXmlElement* propElem = processorElem->FirstChildElement(std::string("Property"));

                while (propElem) {
                    const std::string propName = *(propElem->Attribute(std::string("id")));
                    const char* propDescription = propElem->GetText();
                    if (propDescription)
                        result.push_back(std::make_pair(propName, std::string(propDescription)));
                    propElem = propElem->NextSiblingElement(std::string("Property"));
                }
                break;
            }
            processorElem = processorElem->NextSiblingElement("Processor");
        }

        return result;
    }
    else {
        if (doc.ErrorId() != 13) // no warning if the document is empty
            LWARNING("Could not open XML documentation file for module '" + getName() + "': " + doc.ErrorDesc());
        return std::vector<std::pair<std::string, std::string> >();
    }
}

std::string VoreenModule::getDocumentationProperty(const std::string& className, const std::string& propertyID) const {
    TiXmlDocument doc(documentationFileName_);
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* processorElem = getProcessorElement(doc, className);
        if (processorElem) {
            TiXmlElement* propertyElem = getPropertyElement(processorElem, propertyID);
            if (propertyElem) {
                const char* propertyText = propertyElem->GetText();
                if (propertyText)
                    return std::string(propertyText);
            }
        }
        return "";
    }
    else {
        if (doc.ErrorId() != 13) // no warning if the document is empty
            LWARNING("Could not open XML documentation file for module '" + getName() + "': " + doc.ErrorDesc());
        return "";
    }
}

void VoreenModule::setDocumentationProperty(const std::string& className, const std::string& propertyID, const std::string& description) const {
    TiXmlDocument doc(documentationFileName_);
    doc.LoadFile();
    TiXmlElement* processorElem = getProcessorElement(doc, className);

    // If the description is empty, we only want to delete an existing description element
    if (description == "") {
        if (processorElem) {
            TiXmlElement* propertyElem = getPropertyElement(processorElem, propertyID);
            if (propertyElem)
                processorElem->RemoveChild(propertyElem);

            if (processorElem->NoChildren())
                doc.RemoveChild(processorElem);
            doc.SaveFile();
        }
        return;
    }

    if (!processorElem) {
        insertNewProcessorElement(doc, className);
        processorElem = getProcessorElement(doc, className);
    }

    TiXmlElement* propertyElem = getPropertyElement(processorElem, propertyID);
    if (!propertyElem) {
        insertNewPropertyElement(processorElem, propertyID);
        propertyElem = getPropertyElement(processorElem, propertyID);
    }
    else
        propertyElem->RemoveChild(propertyElem->FirstChild());

    TiXmlText* propertyText = new TiXmlText(description);
    propertyElem->LinkEndChild(propertyText);
    doc.SaveFile();
}

} // namespace

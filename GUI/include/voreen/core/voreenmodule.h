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

#ifndef VRN_VOREENMODULE_H
#define VRN_VOREENMODULE_H

#include "voreen/core/voreencoredefine.h"
#include <string>
#include <vector>
#include "voreen/core/utils/exception.h"

namespace voreen {

class VoreenApplication;

class Processor;
class ProcessorWidget;
class ProcessorWidgetFactory;

class Property;
class PropertyWidget;
class PropertyWidgetFactory;
class LinkEvaluatorFactory;

class Port;
class SerializableFactory;
class VolumeReader;
class VolumeWriter;

/**
 * Abstract base class for module classes.
 *
 * A module class is responsible for registering the
 * module's resources at runtime:
 *  - Processors
 *  - VolumeReaders and VolumeWriters
 *  - SerializableFactories
 *  - Shader search path
 *
 * The registration is to be done in the constructor.
 *
 * @see http://www.voreen.org/402-Adding-a-Module.html
 *
 */
class VRN_CORE_API VoreenModule {

    friend class VoreenApplication;

public:
    /**
     * Instantiate and register the module's classes
     * in the derived class's constructor.
     */
    VoreenModule();

    /**
     * Deletes the registered objects.
     */
    virtual ~VoreenModule();

    /**
     * Returns true, if the module has been successfully initialized,
     * and has not yet been deinitialized.
     */
    bool isInitialized() const;

    /**
     * Module documentation that is intended
     * to be presented to the user.
     */
    std::string getDescription() const;

    /// Sets the description and overwrites the saved one in the XML if it exists
    void setDescription(const std::string& description) const;

    /**
     * Returns the module's name.
     *
     * @see setName
     */
    std::string getName() const;

    /**
     * Returns the module's processors.
     *
     * @see addProcessor
     */
    const std::vector<Processor*>& getProcessors() const;

    /**
     * Returns the module's properties.
     *
     * @see addProperty
     */
    const std::vector<Property*>& getProperties() const;

    /**
     * Returns the module's VolumeReaders.
     *
     * @see addVolumeReader
     */
    const std::vector<VolumeReader*>& getVolumeReaders() const;

    /**
     * Returns the module's VolumeWriters.
     *
     * @see addVolumeWriter
     */
    const std::vector<VolumeWriter*>& getVolumeWriters() const;

    /**
     * Returns the module's SerializerFactories.
     *
     * @see addSerializerFactory
     */
    const std::vector<SerializableFactory*>& getSerializerFactories() const;

    /**
     * Returns the module's ProcessorWidgetFactories.
     *
     * @see addProcessorWidgetFactory
     */
    const std::vector<ProcessorWidgetFactory*>& getProcessorWidgetFactories() const;

    /**
     * Returns the module's PropertyWidgetFactories.
     *
     * @see addPropertyWidgetFactory
     */
    const std::vector<PropertyWidgetFactory*>& getPropertyWidgetFactories() const;

    /**
     * Returns the module's LinkEvaluatorFactories.
     *
     * @see addLinkEvaluatorFactory
     */
    const std::vector<LinkEvaluatorFactory*>& getLinkEvaluatorFactories() const;

    /**
     * Returns the GLSL shader search used by the module.
     *
     * @see addShaderPath
     */
    const std::vector<std::string>& getShaderPaths() const;

    // --- Documentation ---

    /**
     * Returns the description of the passed \sa Processor. The documentation is
     * fetched from the according XML file in the module directory
     * \param className The processor class whose description is wanted
     * \return The description of the processor
     */
    std::string getDocumentationDescription(const std::string& className) const;

    /**
     * Sets the description for the processor denoted by <code>className</code> and
     * overwrites the old description in the XML file.
     * \param className The processor class whose description should be replaced
     * \param description The new description for the processor
     */
    void setDocumentationDescription(const std::string& className, const std::string& description) const;

    /**
     * Returns the descriptions for the \sa Ports of the passed class. The return will be of the form
     * <portname,description> and the vector will contain only those ports which actually do have a
     * description.
     * \param className The class name of the \sa Processor for which the documentation is wanted
     */
    std::vector<std::pair<std::string, std::string> > getDocumentationPorts(const std::string& className) const;

    /**
     * Returns the description for a specific \sa Port of the given \sa Processor. The return value will be the
     * description of the port.
     * \param className The class name of the processor which has the port
     * \param portName The name of the port
     * \return The description of the port
     */
    std::string getDocumentationPort(const std::string& className, const std::string& portName) const;

    /**
     * Sets the description for a specific \sa Port of a specific \sa Processor. The method replaces
     * the old description if such a value exists in the appropriate XML file.
     * \param className The \sa Processor class which hosts the \sa Port
     * \param portName The name of the \sa Port which is unique within a \sa Processor
     * \param description The new description for the \sa Port in question
     */
    void setDocumentationPort(const std::string& className, const std::string& portName, const std::string& description) const;

    /**
     * Returns the descriptions for the \sa Properties of the passed class. The return will be of the form
     * <propertyname,description> and the vector will contain only those properties which actually do have a
     * description.
     * \param className The class name of the \sa Processor for which the documentation is wanted
     */
    std::vector<std::pair<std::string, std::string> > getDocumentationProperties(const std::string& className) const;

    /**
     * Returns the description for a specific \sa Property of the given \sa Processor. The return value will be the
     * description of the Property.
     * \param className The class name of the processor which has the property
     * \param propertyID The ID of the property
     * \return The description of the property
     */
    std::string getDocumentationProperty(const std::string& className, const std::string& propertyID) const;

    /**
     * Sets the description for a specific \sa Property of a specific \sa Processor. The method replaces
     * the old description if such a value exists in the appropriate XML file.
     * \param className The \sa Processor class which hosts the \sa Property
     * \param propertyID The ID of the \sa Property which is unique within a \sa Processor
     * \param description The new description for the \sa Property in question
     */
    void setDocumentationProperty(const std::string& className, const std::string& propertyID, const std::string& description) const;

protected:
    /**
     * OpenGL-dependent or time-consuming initializations
     * should be performed here. However, it is usually
     * not necessary to override this function.
     *
     * @throw tgt::Exception to indicate an initialization failure
     */
    virtual void initialize()
        throw (tgt::Exception);

    /**
     * OpenGL-dependent or time-consuming deinitializations
     * should be performed here. However, it is usually
     * not necessary to override this function.
     *
     * @throw tgt::Exception to indicate a deinitialization failure
     */
    virtual void deinitialize()
        throw (tgt::Exception);

    /**
     * Specifies the module's name. To be called in the
     * derived class's constructor.
     *
     * @note Setting a name is mandatory.
     */
    void setName(const std::string& name);

    /**
     * Specifies the name of the XML file containing the documentation for this
     * module. To be called in the derived class's constructor. 
     *
     * @note Setting a XML filename is mandatory
     *
     * @param xmlFileName The XML filename prefixed by the name of the module directory,
     *  i.e., if the XML filename is "mymodule_doc.xml" and the module name is "mymodule", 
     *  the derived class should call <verbatim>setXMLFileName("mymodule/mymodule_doc.xml").
     * @param absolutePath if true, the passed xmlFileName is interpreted as absolute path 
     *  rather than as path relative to the modules directory. This option should usually
     *  <em>not</em> be used by ordinary modules. 
     *
     */
    void setXMLFileName(const std::string& xmlFileName, bool absolutePath = false);

    /**
     * Registers the passed processor. To be called in the
     * derived class's constructor.
     */
    void addProcessor(Processor* processor);

    /**
     * Registers the passed property. To be called in the
     * derived class's constructor.
     */
    void addProperty(Property* property);

    /**
     * Registers the passed VolumeReader. To be called in the
     * derived class's constructor.
     */
    void addVolumeReader(VolumeReader* reader);

    /**
     * Registers the passed VolumeWriter. To be called in the
     * derived class's constructor.
     */
    void addVolumeWriter(VolumeWriter* writer);

    /**
     * Registers the passed SerializableFactory. To be called in the
     * derived class's constructor.
     */
    void addSerializerFactory(SerializableFactory* factory);

    /**
     * Registers the passed ProcessorWidgetFactory. To be called in the
     * derived class's constructor.
     */
    void addProcessorWidgetFactory(ProcessorWidgetFactory* factory);

    /**
     * Registers the passed PropertyWidgetFactory. To be called in the
     * derived class's constructor.
     */
    void addPropertyWidgetFactory(PropertyWidgetFactory* factory);

    /**
     * Registers the passed LinkEvaluatorFactory. To be called in the
     * derived class's constructor.
     */
    void addLinkEvaluatorFactory(LinkEvaluatorFactory* factory);

    /**
     * Adds the passed directory to the shader search path.
     * To be called in the derived class's constructor.
     *
     * @note Use getModulesPath() for determining the module's directory.
     */
    void addShaderPath(const std::string& path);

    /**
     * Constructs an absolute path consisting of the modules source directory
     * (typically \<VOREEN_ROOT\>/modules) and the given path suffix.
     *
     * @note You have to pass the name of the module's directory,
     *  e.g. getModulesPath("mymodule") yields \<VOREEN_ROOT\>/modules/mymodule
     */
    std::string getModulesPath(const std::string& suffix = "") const;

    static const std::string loggerCat_;

private:
    std::string name_;
    std::string documentationFileName_;
    std::vector<Processor*> processors_;
    std::vector<Property*> properties_;
    std::vector<VolumeReader*> volumeReaders_;
    std::vector<VolumeWriter*> volumeWriters_;
    std::vector<SerializableFactory*> serializerFactories_;
    std::vector<std::string> shaderPaths_;

    std::vector<ProcessorWidgetFactory*> processorWidgetFactories_;
    std::vector<PropertyWidgetFactory*> propertyWidgetFactories_;
    std::vector<LinkEvaluatorFactory*> linkEvaluatorFactories_;

    bool initialized_;  ///< set by VoreenApplication
};

} // namespace

#endif // VRN_VOREENMODULE_H

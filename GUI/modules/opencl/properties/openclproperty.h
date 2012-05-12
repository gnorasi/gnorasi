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

#ifndef VRN_OPENCLPROPERTY_H
#define VRN_OPENCLPROPERTY_H

#include "voreen/core/properties/templateproperty.h"
#include "modules/opencl/utils/clwrapper.h"

#include "modules/opencl/openclmoduledefine.h"

namespace voreen {

class VRN_MODULE_OPENCL_API OpenCLSource : public Serializable {
public:

    // Default ctor
    OpenCLSource() :
        programFilename_(""),
        programModified_(false)
       {}

    OpenCLSource(std::string programFilename) :
        programFilename_(programFilename),
        programModified_(false)
       {}

    std::string programFilename_;
    std::string programSource_;

    bool programModified_;

    /**
     * Operator to compare two OpenCLSource objects.
     *
     * @param programSource OpenCLSource instance that is compared to this instance
     * @return true when both OpenCLSource instances are equal
     */
    bool operator==(const OpenCLSource& programSource) const {
        return     (strcmp(programSource.programFilename_.c_str(), programFilename_.c_str()) == 0)
                && (strcmp(programSource.programSource_.c_str(), programSource_.c_str()) == 0)
                && (programSource.programModified_ == programModified_)
        ;
    }

    /**
     * Operator to compare two OpenCLSource objects.
     *
     * @param programSource OpenCLSource instance that is compared to this instance
     * @return true when both OpenCLSource instances are not equal
     */
     bool operator!=(const OpenCLSource& programSource) const { return !(*this == programSource); }

     virtual void serialize(XmlSerializer& s) const;
     virtual void deserialize(XmlDeserializer& s);
};

#ifdef DLL_TEMPLATE_INST
template class VRN_MODULE_OPENCL_API TemplateProperty<OpenCLSource>;
#endif

class VRN_MODULE_OPENCL_API OpenCLProperty : public TemplateProperty<OpenCLSource> {
public:
    OpenCLProperty(const std::string& id, const std::string& guiText,
                   const std::string& programFilename,
                   Processor::InvalidationLevel invalidationLevel=Processor::INVALID_PROGRAM);
    OpenCLProperty();
    ~OpenCLProperty();

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "OpenCLProperty"; }
    virtual std::string getTypeDescription() const { return "OpenCL Program"; }

    void initialize() throw (tgt::Exception);
    void deinitialize() throw (tgt::Exception);

    /**
     * @see Property::serialize
     */
    virtual void serialize(XmlSerializer& s) const;

    /**
     * @see Property::deserialize
     */
    virtual void deserialize(XmlDeserializer& s);

    void setDefines(std::string defs);
    std::string getDefines() const;

    cl::Program* getProgram();
    void rebuild();

    void setProgramSource(const std::string& programSource);
    void resetProgramSource();
    void setProgramFilename(const std::string& programFilename);
    void resetProgramFilename();

private:
    std::string getProgramAsString(std::string filename);

    std::string programDefines_;
    std::string originalProgramFilename_;

    cl::Program* program_;
};

}   // namespace

#endif

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

#ifndef VRN_GEOMETRYSOURCE_H
#define VRN_GEOMETRYSOURCE_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/ports/geometryport.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/intproperty.h"

namespace voreen {

/**
 * Reads a point list or point segment list from a text file
 * and provides it as geometry through its outport.
 */
class GeometrySource : public Processor {
public:
    GeometrySource();
    virtual ~GeometrySource();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "GeometrySource";  }
    virtual std::string getCategory() const  { return "Data Source";     }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);

private:
    void readGeometry();

    void readPointList();

    /** 
     * Attempts to read the geometry without specifying the concrete subtype.
     * This does only work, if the serialized XML contains type information
     * about the serialized geometry.
     *
     * @return the deserialized Geometry or 0, if deserialization failed
     */
    Geometry* readAbstractGeometry(XmlDeserializer& deserializer) const;

    /**
     * Tries to deserialize the geometry as MeshListGeometry.
     *
     * @return the deserialized Geometry or 0, if deserialization failed
     *
     * @todo obsolete?
     */
    Geometry* readMeshListGeometry(XmlDeserializer& deserializer) const;

    /**
     * Tries to deserialize the geometry as PointListGeometry<tgt::vec3>
     *
     * @return the deserialized Geometry or 0, if deserialization failed
     *
     * @todo obsolete?
     */
    Geometry* readPointListGeometry(XmlDeserializer& deserializer) const;

    /**
     * Tries to deserialize the geometry as PointSegmentListGeometry<tgt::vec3>.
     *
     * @return the deserialized Geometry or 0, if deserialization failed
     *
     * @todo obsolete?
     */
    Geometry* readPointSegmentListGeometry(XmlDeserializer& deserializer) const;

    FileDialogProperty geometryFile_;   ///< filename of the file containing the point information
    StringOptionProperty geometryType_; ///< Determines whether the geometry is read as point or segment list.
    IntProperty skipItemCount_;         ///< data item to skip in the file after reading each point

    GeometryPort outport_;

    static const std::string loggerCat_;
};

}   //namespace

#endif // VRN_GEOMETRYSOURCE_H

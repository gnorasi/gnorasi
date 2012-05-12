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

#ifndef VRN_POINTLISTGEOMETRY_H
#define VRN_POINTLISTGEOMETRY_H

#include "voreen/core/datastructures/geometry/geometry.h"
#include "tgt/glmath.h"
#include <vector>

#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"

namespace voreen {

template<class T>
class PointListGeometry : public Geometry {
public:
    virtual void render() const {
        glBegin(GL_POINTS);
        for (size_t i=0; i < points_.size(); ++i){
            // assuming data type stored in the point list is compatible to tgt::vertex
            // if not: template instantiation will fail (compile error)
            tgt::vertex(points_[i]);
        }
        glEnd();
    }

    void addPoint(T point) {points_.push_back(point);}
    void removeLast() {points_.pop_back();}
    void clear() {points_.clear();}
    const std::vector<T>& getData() const { return points_; }
    void setData(std::vector<T> points) { points_ = points; }
    size_t getNumPoints() const { return points_.size(); }
    T getPoint(size_t i) const { return points_[i]; }

    typename std::vector<T>::iterator begin() { return points_.begin(); };
    typename std::vector<T>::const_iterator begin() const { return points_.begin(); };
    typename std::vector<T>::iterator end() { return points_.end(); };
    typename std::vector<T>::const_iterator end() const { return points_.end(); };

    T& operator[](size_t index) { return points_[index]; };
    const T& operator[](size_t index) const { return points_[index]; };

    virtual void serialize(XmlSerializer& s) const {
        s.serialize("points", points_);
    }

    virtual void deserialize(XmlDeserializer& s) {
        s.deserialize("points", points_);
    }

protected:
    std::vector<T> points_;

};

// -------------------------------------------------------------------------

class PointListGeometryVec3 : public PointListGeometry<tgt::vec3> {
public:
    void render() const {
        glBegin(GL_POINTS);
        for ( size_t i = 0; i < points_.size(); i++) {
            const tgt::vec3& v = this->points_.at(i);
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }
};

} // namespace

#endif // VRN_POINTLISTGEOMETRY_H

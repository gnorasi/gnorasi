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

#ifndef VRN_NETWORKANNOTATION_H
#define VRN_NETWORKANNOTATION_H

#include "voreen/core/io/serialization/serialization.h"

namespace voreen {

class VRN_CORE_API NetworkAnnotation : public Serializable {
public:
    NetworkAnnotation();
    void serialize(XmlSerializer& s) const;
    void deserialize(XmlDeserializer& s);

    void setPosition(int x, int y);
    std::pair<int,int> getPosition() const;

    bool hasFence() const;
    std::pair<int,int> getFencePosition() const;
    void setFencePosition(int x, int y);

    void setText(const std::string& text);
    const std::string& getText() const;

private:
    std::string text_;
    int positionX_;
    int positionY_;

    bool hasFence_;
    int positionFenceX_;
    int positionFenceY_;
};

class VRN_CORE_API NetworkAnnotationContainer : public MetaDataBase {
public:
    NetworkAnnotationContainer();
    std::string getClassName() const { return "NetworkAnnotationContainer"; };
    Serializable* create() const;
    MetaDataBase* clone() const;
    std::string toString() const;

    void serialize(XmlSerializer& s) const;
    void deserialize(XmlDeserializer& s);

    void addNetworkAnnotation(NetworkAnnotation* annotation);
    void removeNetworkAnnotation(NetworkAnnotation* annotation);
    void clearNetworkAnnotations();
    bool isEmpty() const;

    bool getShowAnnotations() const;
    void setShowAnnotations(bool v);

    const std::vector<NetworkAnnotation*>& getAnnotations() const;

private:
    std::vector<NetworkAnnotation*> annotations_;
    bool showAnnotations_;
};


} // namespace

#endif // VRN_NETWORKANNOTATION_H

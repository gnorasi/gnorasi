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

#ifndef VRN_VOLUMEFACTORY_H
#define VRN_VOLUMEFACTORY_H

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include <algorithm>
#include <cctype>
#include <typeinfo> 

namespace {
    struct ToLower {
        char operator() (char c) const { return std::tolower(c); }
    };
}

namespace voreen {

class VRN_CORE_API VolumeGeneratorBase {
public:
    virtual std::string getType() const = 0;
    virtual Volume* create(tgt::svec3 dimensions) const = 0;
    virtual bool isType(const Volume* v) const = 0;
    virtual int getNumChannels() const = 0;
};

class VRN_CORE_API VolumeFactory {
public:
    VolumeFactory();
    ~VolumeFactory();

    std::string getType(const Volume* v) const {
        for (size_t i = 0; i < generators_.size(); ++i) {
            if (generators_[i]->isType(v))
                return generators_[i]->getType();
        }
        return "";
    }

    Volume* create(const std::string& type, tgt::svec3 dimensions) const {
        std::string smallType = type;
        std::transform(type.begin(), type.end(), smallType.begin(), ToLower());
        for (size_t i = 0; i < generators_.size(); ++i) {
            if ((generators_[i]->getType() == type) || (generators_[i]->getType() == smallType))
                return generators_[i]->create(dimensions);
        }
        LERROR("Failed to create volume of type '" << type << "'");
        return 0;
    }

    int getNumChannels(const std::string& type) const {
        std::string smallType = type;
        std::transform(type.begin(), type.end(), smallType.begin(), ToLower());
        for (size_t i = 0; i < generators_.size(); ++i) {
            if (generators_[i]->getType() == type || (generators_[i]->getType() == smallType))
                return generators_[i]->getNumChannels();
        }
        LERROR("Failed to get number of channels for '" << type << "'");
        return 0;
    }
    
private:
    std::vector<VolumeGeneratorBase*> generators_;

    static const std::string loggerCat_;
};

template<class T>
class VRN_CORE_API VolumeGeneratorGeneric : public VolumeGeneratorBase {
public:
    virtual std::string getType() const = 0;

    virtual Volume* create(tgt::svec3 dimensions) const {
        return new VolumeAtomic<T>(dimensions);
    }

    virtual bool isType(const Volume* v) const {
        if(typeid(*v) == typeid(VolumeAtomic<T>))
            return true;
        else
            return false;
    }

    virtual int getNumChannels() const = 0;
};

//--------------------------------------------------------------------------------
//Scalar types:

class VRN_CORE_API VolumeGeneratorUInt8 : public VolumeGeneratorGeneric<uint8_t> {
public:
    std::string getType() const { return "uint8"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorInt8 : public VolumeGeneratorGeneric<int8_t> {
public:
    std::string getType() const { return "int8"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorUInt16 : public VolumeGeneratorGeneric<uint16_t> {
public:
    std::string getType() const { return "uint16"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorInt16 : public VolumeGeneratorGeneric<int16_t> {
public:
    std::string getType() const { return "int16"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorUInt32 : public VolumeGeneratorGeneric<uint32_t> {
public:
    std::string getType() const { return "uint32"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorInt32 : public VolumeGeneratorGeneric<int32_t> {
public:
    std::string getType() const { return "int32"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorFloat : public VolumeGeneratorGeneric<float> {
public:
    std::string getType() const { return "float"; }
    int getNumChannels() const { return 1; }
};

class VRN_CORE_API VolumeGeneratorDouble : public VolumeGeneratorGeneric<double> {
public:
    std::string getType() const { return "double"; }
    int getNumChannels() const { return 1; }
};

//--------------------------------------------------------------------------------
//Vector2 types:

class VRN_CORE_API VolumeGenerator2xFloat : public VolumeGeneratorGeneric<tgt::Vector2<float> > {
public:
    std::string getType() const { return "Vector2(float)"; }
    int getNumChannels() const { return 2; }
};

class VRN_CORE_API VolumeGenerator2xDouble : public VolumeGeneratorGeneric<tgt::Vector2<double> > {
public:
    std::string getType() const { return "Vector2(double)"; }
    int getNumChannels() const { return 2; }
};

class VRN_CORE_API VolumeGenerator2xUInt8 : public VolumeGeneratorGeneric<tgt::Vector2<uint8_t> > {
public:
    std::string getType() const { return "Vector2(uint8)"; }
    int getNumChannels() const { return 2; }
};

class VRN_CORE_API VolumeGenerator2xInt8 : public VolumeGeneratorGeneric<tgt::Vector2<int8_t> > {
public:
    std::string getType() const { return "Vector2(int8)"; }
    int getNumChannels() const { return 2; }
};

class VRN_CORE_API VolumeGenerator2xUInt16 : public VolumeGeneratorGeneric<tgt::Vector2<uint16_t> > {
public:
    std::string getType() const { return "Vector2(uint16)"; }
    int getNumChannels() const { return 2; }
};

class VRN_CORE_API VolumeGenerator2xInt16 : public VolumeGeneratorGeneric<tgt::Vector2<int16_t> > {
public:
    std::string getType() const { return "Vector2(int16)"; }
    int getNumChannels() const { return 2; }
};

//--------------------------------------------------------------------------------
//Vector3 types:

class VRN_CORE_API VolumeGenerator3xFloat : public VolumeGeneratorGeneric<tgt::Vector3<float> > {
public:
    std::string getType() const { return "Vector3(float)"; }
    int getNumChannels() const { return 3; }
};

class VRN_CORE_API VolumeGenerator3xDouble : public VolumeGeneratorGeneric<tgt::Vector3<double> > {
public:
    std::string getType() const { return "Vector3(double)"; }
    int getNumChannels() const { return 3; }
};

class VRN_CORE_API VolumeGenerator3xUInt8 : public VolumeGeneratorGeneric<tgt::Vector3<uint8_t> > {
public:
    std::string getType() const { return "Vector3(uint8)"; }
    int getNumChannels() const { return 3; }
};

class VRN_CORE_API VolumeGenerator3xInt8 : public VolumeGeneratorGeneric<tgt::Vector3<int8_t> > {
public:
    std::string getType() const { return "Vector3(int8)"; }
    int getNumChannels() const { return 3; }
};

class VRN_CORE_API VolumeGenerator3xUInt16 : public VolumeGeneratorGeneric<tgt::Vector3<uint16_t> > {
public:
    std::string getType() const { return "Vector3(uint16)"; }
    int getNumChannels() const { return 3; }
};

class VRN_CORE_API VolumeGenerator3xInt16 : public VolumeGeneratorGeneric<tgt::Vector3<int16_t> > {
public:
    std::string getType() const { return "Vector3(int16)"; }
    int getNumChannels() const { return 3; }
};

//--------------------------------------------------------------------------------
//Vector4 types:

class VRN_CORE_API VolumeGenerator4xFloat : public VolumeGeneratorGeneric<tgt::Vector4<float> > {
public:
    std::string getType() const { return "Vector4(float)"; }
    int getNumChannels() const { return 4; }
};

class VRN_CORE_API VolumeGenerator4xDouble : public VolumeGeneratorGeneric<tgt::Vector4<double> > {
public:
    std::string getType() const { return "Vector4(double)"; }
    int getNumChannels() const { return 4; }
};

class VRN_CORE_API VolumeGenerator4xUInt8 : public VolumeGeneratorGeneric<tgt::Vector4<uint8_t> > {
public:
    std::string getType() const { return "Vector4(uint8)"; }
    int getNumChannels() const { return 4; }
};

class VRN_CORE_API VolumeGenerator4xInt8 : public VolumeGeneratorGeneric<tgt::Vector4<int8_t> > {
public:
    std::string getType() const { return "Vector4(int8)"; }
    int getNumChannels() const { return 4; }
};

class VRN_CORE_API VolumeGenerator4xUInt16 : public VolumeGeneratorGeneric<tgt::Vector4<uint16_t> > {
public:
    std::string getType() const { return "Vector4(uint16)"; }
    int getNumChannels() const { return 4; }
};

class VRN_CORE_API VolumeGenerator4xInt16 : public VolumeGeneratorGeneric<tgt::Vector4<int16_t> > {
public:
    std::string getType() const { return "Vector4(int16)"; }
    int getNumChannels() const { return 4; }
};

//--------------------------------------------------------------------------------
//Matrix types:

class VRN_CORE_API VolumeGeneratorMat3Float : public VolumeGeneratorGeneric<tgt::Matrix3<float> > {
public:
    std::string getType() const { return "Matrix3(float)"; }
    int getNumChannels() const { return 9; }
};

class VRN_CORE_API VolumeGeneratorMat3Double : public VolumeGeneratorGeneric<tgt::Matrix3<double> > {
public:
    std::string getType() const { return "Matrix3(double)"; }
    int getNumChannels() const { return 9; }
};

class VRN_CORE_API VolumeGeneratorMat4Float : public VolumeGeneratorGeneric<tgt::Matrix4<float> > {
public:
    std::string getType() const { return "Matrix4(float)"; }
    int getNumChannels() const { return 16; }
};

class VRN_CORE_API VolumeGeneratorMat4Double : public VolumeGeneratorGeneric<tgt::Matrix4<double> > {
public:
    std::string getType() const { return "Matrix4(double)"; }
    int getNumChannels() const { return 16; }
};

//--------------------------------------------------------------------------------
//Tensor types:

class VRN_CORE_API VolumeGeneratorTensor2Float : public VolumeGeneratorGeneric<Tensor2<float> > {
public:
    std::string getType() const { return "Tensor2(float)"; }
    int getNumChannels() const { return 6; }
};
    
} // namespace voreen

#endif // VRN_VOLUMEFACTORY_H

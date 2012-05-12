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

#include "voreen/core/datastructures/volume/volumegl.h"

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volumeoperator.h"
#include "tgt/gpucapabilities.h"

#include <algorithm>
#include <typeinfo>
#include <sstream>

#ifdef VRN_MODULE_FLOWREEN
#include "modules/flowreen/datastructures/volumeflow3d.h"
#endif

using tgt::vec3;
using tgt::bvec3;
using tgt::ivec3;
using tgt::svec3;
using tgt::col4;
using tgt::mat4;

namespace {

// calculates next power of two if it is not already power of two
static int fitPowerOfTwo(int i) {
    int result = 1;

    while (result < i)
        result <<= 1;

    return result;
}

// same as above but for each component of a ivec3
static ivec3 fitPowerOfTwo(const ivec3& v) {
    ivec3 result;

    result.x = fitPowerOfTwo(v.x);
    result.y = fitPowerOfTwo(v.y);
    result.z = fitPowerOfTwo(v.z);

    return result;
}

// returns true, if dims.x, dims.y and dims.z are power of two
bool isPowerOfTwo(const ivec3& dims) {
    return dims == fitPowerOfTwo(dims);
}

} // namespace

namespace voreen {

const std::string VolumeGL::loggerCat_("voreen.VolumeGL");

VolumeGL::VolumeGL(const Volume* volume) throw (VoreenException, std::bad_alloc)
  : VolumeRepresentation(volume->getDimensions())
  , texture_(0)
{
    tgtAssert(volume, "No volume");
    generateTexture(volume);
}

VolumeGL::~VolumeGL() {
    destroy();
}

void VolumeGL::destroy() {
    delete texture_;
}

const VolumeTexture* VolumeGL::getTexture() const {
    return texture_;
}

VolumeTexture* VolumeGL::getTexture() {
    return texture_;
}
         
void VolumeGL::generateTexture(const Volume* volume) 
        throw (VoreenException, std::bad_alloc) 
{
    if (!tgt::hand(tgt::greaterThan(volume->getDimensions(), svec3(1)))) {
        std::stringstream message;
        message << "OpenGL volumes must have a size greater than one in all dimensions. Actual size: " << volume->getDimensions();
        LERROR(message.str());
        throw VoreenException(message.str());
    }
    
    if (!GpuCaps.is3DTexturingSupported()) {
        std::string message = "3D textures apparently not supported by the OpenGL driver";
        LERROR(message);
        throw VoreenException(message);
    }

    if (!GpuCaps.isNpotSupported() && !isPowerOfTwo(getDimensions())) {
        std::string message = "Non-power-of-two textures apparently not supported by the OpenGL driver";
        LERROR(message);
        throw VoreenException(message);
    }

    //
    // Determine GL format
    //
    GLint format;         ///< The format of textures which will are created.
    GLint internalFormat; ///< The internal format of the textures which are created.
    GLenum dataType;      ///< The data type of the textures which are created.
    // scalar
    if (dynamic_cast<const VolumeAtomic<uint8_t>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA8;
        dataType = GL_UNSIGNED_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<int8_t>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA8;
        dataType = GL_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<uint16_t>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA16;
        dataType = GL_UNSIGNED_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<int16_t>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA16;
        dataType = GL_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<uint32_t>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA;
        dataType = GL_UNSIGNED_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<int32_t>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA;
        dataType = GL_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<uint64_t>*>(volume)) {
        std::string message = "VolumeUInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<int64_t>*>(volume)) {
        std::string message = "VolumeInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<float>*>(volume)) {
        format = GL_ALPHA;
        internalFormat = GL_ALPHA;
        dataType = GL_FLOAT;
    }
    else if (dynamic_cast<const VolumeAtomic<double>*>(volume)) {
        std::string message = "VolumeDouble not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    // vec2
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint8_t> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_UNSIGNED_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int8_t> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint16_t> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_UNSIGNED_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int16_t> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint32_t> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_UNSIGNED_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int32_t> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint64_t> >*>(volume)) {
        std::string message = "Volume2xUInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int64_t> >*>(volume)) {
        std::string message = "Volume2xInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<float> >*>(volume)) {
        format = GL_LUMINANCE_ALPHA;
        internalFormat = GL_LUMINANCE_ALPHA;
        dataType = GL_FLOAT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<double> >*>(volume)) {
        std::string message = "Volume2xDouble not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    // vec3
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint8_t> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB8;
        dataType = GL_UNSIGNED_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int8_t> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB8;
        dataType = GL_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint16_t> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB16;
        dataType = GL_UNSIGNED_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int16_t> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB16;
        dataType = GL_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint32_t> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB;
        dataType = GL_UNSIGNED_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int32_t> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB;
        dataType = GL_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint64_t> >*>(volume)) {
        std::string message = "Volume3xUInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int64_t> >*>(volume)) {
        std::string message = "Volume3xInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<float> >*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB;
        dataType = GL_FLOAT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<double> >*>(volume)) {
        std::string message = "Volume3xDouble not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    // vec4
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint8_t> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
        dataType = GL_UNSIGNED_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int8_t> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
        dataType = GL_BYTE;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint16_t> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA16;
        dataType = GL_UNSIGNED_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int16_t> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA16;
        dataType = GL_SHORT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint32_t> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
        dataType = GL_UNSIGNED_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int32_t> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
        dataType = GL_INT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint64_t> >*>(volume)) {
        std::string message = "Volume4xUInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int64_t> >*>(volume)) {
        std::string message = "Volume4xInt64 not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<float> >*>(volume)) {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
        dataType = GL_FLOAT;
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<double> >*>(volume)) {
        std::string message = "Volume4xDouble not supported as OpenGL volume.";
        LERROR(message);
        throw VoreenException(message);
    }
    // special types (TODO: extract from here)
#ifdef VRN_MODULE_FLOWREEN
    else if (dynamic_cast<const VolumeFlow3D*>(volume)) {
        format = GL_RGB;
        internalFormat = GL_RGB16;
        dataType = GL_FLOAT;
    }
#endif
    else {
        LERROR("unknown or unsupported volume type");
    }


    //
    // Create texture
    //
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    VolumeTexture* vTex = 0;

    tgt::vec3* temp2 = 0;
#ifdef VRN_MODULE_FLOWREEN
    // Textures containing flow data need to contain data
    // within range [0.0, 1.0], so the values have to be mapped
    const VolumeFlow3D* flowTex = dynamic_cast<const VolumeFlow3D*>(volume);
    if (flowTex) {
        const float minValue = flowTex->getMinValue();
        const float maxValue = flowTex->getMaxValue();
        const float range = (maxValue - minValue);

        const tgt::vec3* const voxels = flowTex->voxel();
        temp2 = new tgt::vec3[volume->getNumVoxels()];
        for (size_t i = 0; i < volume->getNumVoxels(); ++i) {
            if (voxels[i] != tgt::vec3::zero) {
                temp2[i].x = (voxels[i].x - minValue) / range;
                temp2[i].y = (voxels[i].y - minValue) / range;
                temp2[i].z = (voxels[i].z - minValue) / range;
            }
        }
    }
#endif  


    // use temp data if this was created
    if (temp2) {
        vTex = new VolumeTexture(reinterpret_cast<GLubyte*>(temp2), volume->getDimensionsWithBorder(),
        format, internalFormat, dataType, tgt::Texture::LINEAR);
    }
    else {
        vTex = new VolumeTexture(static_cast<const GLubyte*>(volume->getData()),
            volume->getDimensionsWithBorder(),
            format, internalFormat, dataType, tgt::Texture::LINEAR);
    }

    LGL_ERROR;

    vTex->bind();
    if (volume->getData())
        vTex->uploadTexture();

    // set texture wrap to clamp
    vTex->setWrapping(tgt::Texture::CLAMP);

    LGL_ERROR;

    // delete temporary data that has eventually be created
    delete[] temp2;

    // prevent deleting twice
    vTex->setPixelData(0);

    // append to internal data structure
    texture_ = vTex;

    LGL_ERROR;
}

int VolumeGL::getNumChannels() const {
    switch(getTexture()->getFormat()) {
        case GL_ALPHA: return 1;
        case GL_LUMINANCE_ALPHA: return 2;
        case GL_RGB: return 3;
        case GL_RGBA: return 4;
        default:
            tgtAssert(false, "unsupported volume format");
            return 0;
    }
}

//--------------------------------------------------------

bool RepresentationConverterUploadGL::canConvert(const VolumeRepresentation* source) const {
    //We can only convert from RAM volumes:
    if(dynamic_cast<const Volume*>(source))
        return true;
    else
        return false;
}

VolumeRepresentation* RepresentationConverterUploadGL::convert(const VolumeRepresentation* source) const {
    const Volume* v = dynamic_cast<const Volume*>(source);

    if (!v) {
        //should have checked before...
        //LERROR("Failed to convert!");
        return 0;
    }

    VolumeGL* vgl = 0;
    try {
        vgl = new VolumeGL(v);
        LGL_ERROR;
    }
    catch (VoreenException& e) {
        LERRORC("voreen.RepresentationConverterUploadGL", "Failed to create VolumeGL: " << e.what());
    }
    catch (std::bad_alloc& /*e*/) {
        LERRORC("voreen.RepresentationConverterUploadGL", "Bad allocation during creation of VolumeGL");
    }
    return vgl;
}

//--------------------------------------------------------

bool RepresentationConverterDownloadGL::canConvert(const VolumeRepresentation* source) const {
    //We can only convert from GL volumes:
    if(dynamic_cast<const VolumeGL*>(source))
        return true;
    else
        return false;
}

VolumeRepresentation* RepresentationConverterDownloadGL::convert(const VolumeRepresentation* source) const {
    const VolumeGL* vgl = dynamic_cast<const VolumeGL*>(source);

    if(vgl) {
        const VolumeTexture* tex = vgl->getTexture();
        tgt::svec3 dims = tex->getDimensions();
        Volume* v = 0;

        switch(tex->getFormat()) {
            case GL_ALPHA:
                switch(tex->getDataType()) {
                    // VolumeUIntX
                    case GL_UNSIGNED_BYTE:
                        v = new VolumeUInt8(dims);
                        //internalFormat = GL_ALPHA8;
                        break;
                    case GL_UNSIGNED_SHORT:
                        v = new VolumeUInt16(dims);
                        //internalFormat = GL_ALPHA16;
                        break;
                    case GL_UNSIGNED_INT:
                        v = new VolumeUInt32(dims);
                        //internalFormat = GL_ALPHA;
                        break;
                        // VolumeIntX
                    case GL_BYTE:
                        v = new VolumeInt8(dims);
                        //internalFormat = GL_ALPHA8;
                        break;
                    case GL_SHORT:
                        v = new VolumeInt16(dims);
                        //internalFormat = GL_ALPHA16;
                        break;
                    case GL_INT:
                        v = new VolumeInt32(dims);
                        //internalFormat = GL_ALPHA;
                        break;
                        // VolumeFloat and VolumeDouble
                    case GL_FLOAT:
                        v = new VolumeFloat(dims);
                        //internalFormat = GL_ALPHA;
                        break;
                    default:
                        //v = new VolumeDouble(dims);
                        LERRORC("voreen.RepresentationConverterDownloadGL", "Unsupported texture format! (GL_ALPHA)");
                }
                break;
            case GL_LUMINANCE_ALPHA:
                switch(tex->getDataType()) {
                    // Volume2x with int16 types
                    case GL_UNSIGNED_SHORT:
                        v = new Volume2xUInt16(dims);
                        //internalFormat = GL_LUMINANCE16_ALPHA16;
                        break;
                    case GL_SHORT:
                        v = new Volume2xInt16(dims);
                        //internalFormat = GL_LUMINANCE16_ALPHA16;
                        break;
                    default:
                        LERRORC("voreen.RepresentationConverterDownloadGL", "Unsupported texture format! (GL_LUMINANCE_ALPHA)");
                        //v = new VolumeDouble(dims);
                }
                break;
            case GL_RGB:
                switch(tex->getDataType()) {
                    // Volume3x with int8 types
                    case GL_UNSIGNED_BYTE:
                        v = new Volume3xUInt8(dims);
                        //internalFormat = GL_RGB8;
                        break;
                    case GL_BYTE:
                        v = new Volume3xInt8(dims);
                        //internalFormat = GL_RGB8;
                        break;
                        // Volume3x with int16 types
                    case GL_UNSIGNED_SHORT:
                        v = new Volume3xUInt16(dims);
                        //internalFormat = GL_RGB16;
                        break;
                    case GL_SHORT:
                        v = new Volume3xInt16(dims);
                        //internalFormat = GL_RGB16;
                        break;
                        // Volume3x with real types
                    case GL_FLOAT:
                        v = new Volume3xFloat(dims);
                        //internalFormat = GL_RGB;
                        break;
                    default:
                        LERRORC("voreen.RepresentationConverterDownloadGL", "Unsupported texture format! (GL_RGB)");
                        //v = new Volume3xDouble(dims);
                }
                break;
            case GL_RGBA:
                switch(tex->getDataType()) {
                    // Volume4x with int8 types
                    case GL_UNSIGNED_BYTE:
                        v = new Volume4xUInt8(dims);
                        //internalFormat = GL_RGBA8;
                        break;
                    case GL_BYTE:
                        v = new Volume4xInt8(dims);
                        //internalFormat = GL_RGBA8;
                        break;
                        // Volume4x with int16 types
                    case GL_UNSIGNED_SHORT:
                        v = new Volume4xUInt16(dims);
                        //internalFormat = GL_RGBA16;
                        break;
                    case GL_SHORT:
                        v = new Volume4xInt16(dims);
                        //internalFormat = GL_RGBA16;
                        break;
                        // Volume4x with real types
                    case GL_FLOAT:
                        v = new Volume4xFloat(dims);
                        //internalFormat = GL_RGBA;
                        break;
                    default:
                        LERRORC("voreen.RepresentationConverterDownloadGL", "Unsupported texture format! (GL_RGBA)");
                        //v = new Volume4xDouble(dims);
                }
                break;
            default:
                tgtAssert(false, "unsupported volume format");
        }

        if(v) {
            tex->downloadTextureToBuffer(static_cast<GLubyte*>(v->getData()), v->getNumBytes());
        }
        return v;
    }
    else {
        //should have checked before...
        //LERROR("Failed to convert!");
        return 0;
    }
}

} // namespace voreen

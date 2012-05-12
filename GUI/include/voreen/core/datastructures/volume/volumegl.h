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

#ifndef VRN_VOLUMEGL_H
#define VRN_VOLUMEGL_H

#include <typeinfo>
#include <vector>

#include "tgt/tgt_gl.h"

#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumetexture.h"
#include "voreen/core/utils/exception.h"

namespace voreen {

/**
 * This class is the OpenGL interface for volume objects.
 * One or several 3D textures, which hold the complete data set, are created.
 */
class VolumeGL : public VolumeRepresentation {
public:

    /**
     * Creates the VolumeTexture instance.
     *
     * @note The volume must have a size greater one in all dimensions
     *
     * @throw VoreenException if the volume type is not supported by OpenGL
     * @throw std::bad_alloc
     */
    VolumeGL(const Volume* volume) 
        throw (VoreenException, std::bad_alloc);

    virtual ~VolumeGL();

    /**
     * Returns a const pointer to the texture with index \p i.
     * The VolumeTexture pointers \e DO \e NOT have any special order. If you
     * want to sort them by distance use \a getSortedTextures
     *
     * @param i The index of the desired VolumeTexture
     * @return The desired VolumeTexture
     */
    const VolumeTexture* getTexture() const;

    /// @overload
    VolumeTexture* getTexture();

    virtual int getNumChannels() const;
protected:
    /**
     * Determines the volume type and creates the according OpenGL texture. 
     * Internally called by the constructor.
     *
     * @throw VoreenException if the volume type is not supported by OpenGL
     * @throw std::bad_alloc
     */
    void generateTexture(const Volume* vol) 
        throw (VoreenException, std::bad_alloc);
        
    VolumeTexture* texture_;

    static const std::string loggerCat_;

private:
    /// Used internally for destruction of the data.
    void destroy();
};

class RepresentationConverterUploadGL : public RepresentationConverter<VolumeGL> {
public:
    virtual bool canConvert(const VolumeRepresentation* source) const;
    virtual VolumeRepresentation* convert(const VolumeRepresentation* source) const;
};

class RepresentationConverterDownloadGL : public RepresentationConverter<Volume> {
public:
    virtual bool canConvert(const VolumeRepresentation* source) const;
    virtual VolumeRepresentation* convert(const VolumeRepresentation* source) const;
};

} // namespace voreen

#endif // VRN_VOLUMEGL_H

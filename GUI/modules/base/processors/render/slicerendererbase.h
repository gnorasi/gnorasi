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

#ifndef VRN_SLICERENDERERBASE_H
#define VRN_SLICERENDERERBASE_H

#include <cstring>

#include "tgt/vector.h"
#include "tgt/matrix.h"
#include "tgt/shadermanager.h"

#include "voreen/core/processors/processor.h"
#include "voreen/core/processors/volumerenderer.h"
#include "voreen/core/datastructures/transfunc/transfunc.h"

#include "voreen/core/properties/transfuncproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/boolproperty.h"

#include "voreen/core/ports/volumeport.h"

namespace tgt {
    class TextureUnit;
}

namespace voreen {

/**
 * Base class for all SliceRendering sub classes.
 * Provides basic functionality.
 */
class SliceRendererBase : public VolumeRenderer {

public:
    enum TextureMode {
        TEXTURE_2D, 
        TEXTURE_3D
    };

    enum SliceAlignment {
        YZ_PLANE = 0,
        XZ_PLANE = 1,
        XY_PLANE = 2
    };

    SliceRendererBase();

protected:
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    /**
     * \see Processor::beforeProcess()
     */
    virtual void beforeProcess();
    
    /**
     * Initializes the shader if applicable and sets all uniforms.
     */
    virtual bool setupVolumeShader(const VolumeHandleBase* volumeHandle = 0, tgt::TextureUnit* volUnit = 0, tgt::TextureUnit* transferUnit = 0,
        const tgt::Camera* camera = 0, const tgt::vec4& lightPosition = tgt::vec4(0.f));

    virtual bool setupSliceShader(tgt::TextureUnit* texUnit, tgt::TextureUnit* transferUnit, float bitScale);

    virtual bool bindSliceTexture(const tgt::Texture* sliceTexture, tgt::TextureUnit* texUnit);

    /**
     * Generates the header for the shader depending on the choice of
     * features to be used.
     */
    virtual std::string buildShaderHeader();

    /**
     * Recompiles the shader.
     */
    bool rebuildShader();

    /**
     * Deactivates the shader
     */
    void deactivateShader();

    /**
     * Creates a 2D texture from an axis-aligned slice of the passed volume, using nearest filtering.
     *
     * @note The passed texture coordinates must represent an axis-aligned slice,
     *      i.e., they must be equal in one dimension.
     *
     * @param volumeHandle the volume to create the slice texture from, 
     *      its data type has to be OpenGL compatible.
     * @param texLowerLeft lower left of the slice in 3D texture coordinates (normalized)
     * @param texUpperRight upper right of the slice in 3D texture coordinates (normalized)

     * @return the created slice texture, whose dimensions and data type matches the input volume's properties.
     *      If the slice texture could not be created, 0 is returned.
     *
     */
    tgt::Texture* generateAlignedSliceTexture(const VolumeHandleBase* volumeHandle, const tgt::vec3& texLowerLeft, const tgt::vec3& texUpperRight) const;

    RenderPort outport_;
    VolumePort inport_;

    TransFuncProperty transferFunc_;

    OptionProperty<TextureMode> texMode_;     ///< use 2D slice textures or 3D volume texture?
    IntOptionProperty texFilterMode_;         ///< texture filtering mode to use for volume access
    GLEnumOptionProperty texClampMode_;       ///< texture clamp mode to use for the volume
    FloatProperty texBorderIntensity_;        ///< clamp border intensity
    BoolProperty applyColorModulation_;       ///< apply color modulation in case of RGB volumes

    tgt::Shader* sliceShader_;

    static const std::string transFuncTexUnit_;
    static const std::string volTexUnit_;
        
    static const std::string loggerCat_;

private:
    void adjustPropertyVisibilities();

};

// ----------------------------------------------------------------------------

} // namespace voreen

#endif // VRN_SLICERENDERERBASE_H

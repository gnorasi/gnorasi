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

#ifndef VRN_GLSLTRANSLATION_H
#define VRN_GLSLTRANSLATION_H

#include "voreen/core/utils/GLSLparser/glsl/glslexternaldeclaration.h"
#include <vector>

namespace voreen {

namespace glslparser {

class GLSLTranslation : public GLSLNode
{
public:
    GLSLTranslation(GLSLExternalDeclaration* const extDecl)
        : GLSLNode(GLSLTerminals::ID_UNKNOWN)
    {
        addExternalDeclaration(extDecl);
    }

    virtual ~GLSLTranslation() {
        for (size_t i = 0; i < extDecls_.size(); ++i)
            delete extDecls_[i];
    }

    virtual int getNodeType() const { return GLSLNodeTypes::NODE_TRANSLATION; }

    void addExternalDeclaration(GLSLExternalDeclaration* const extDecl) {
        if (extDecl)
            extDecls_.push_back(extDecl);
    }

    const std::vector<GLSLExternalDeclaration*>& getExternalDeclarations() {
        return extDecls_;
    }

protected:
    std::vector<GLSLExternalDeclaration*> extDecls_;
};  // clss GLSLTranslation

}   // namespace glslparser

}   // namespace voreen

#endif  // VRN_GLSLTRANSLATION_H

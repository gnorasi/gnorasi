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

#ifndef VRN_VOREENBLASCL_H
#define VRN_VOREENBLASCL_H

#include "voreen/core/utils/voreenblas/voreenblas.h"
#include "voreen/core/voreencoredefine.h"

#include "modules/opencl/utils/clwrapper.h"

#include <string>

namespace voreen {

class VRN_CORE_API VoreenBlasCL : public VoreenBlas {

public:

    VoreenBlasCL();
    virtual ~VoreenBlasCL();

    virtual void initialize() throw(VoreenException);
    bool isInitialized() const;

    void sAXPY(size_t vecSize, const float* vecx, const float* vecy, float alpha, float* result) const;

    float sDOT(size_t vecSize, const float* vecx, const float* vecy) const;

    float sNRM2(size_t vecSize, const float* vecx) const;

    void sSpMVEll(const EllpackMatrix<float>& mat, const float* vec, float* result) const;

    void hSpMVEll(const EllpackMatrix<int16_t>& mat, const float* vec, float* result) const;

    float sSpInnerProductEll(const EllpackMatrix<float>& mat, const float* vecx, const float* vecy) const;

    int sSpConjGradEll(const EllpackMatrix<float>& mat, const float* vec, float* result,
        float* initial = 0, VoreenBlas::ConjGradPreconditioner precond = VoreenBlas::NoPreconditioner, 
        float threshold = 1e-4f, int maxIterations = 1000) const;

    int hSpConjGradEll(const EllpackMatrix<int16_t>& mat, const float* vec, float* result,
        float* initial = 0, float threshold = 1e-4f, int maxIterations = 1000) const;

private:
    void clearCL();

    bool initialized_;

    cl::OpenCL* opencl_;
    cl::Context* context_;
    cl::Device* device_;
    cl::CommandQueue* queue_;
    cl::Program* prog_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace voreen

#endif

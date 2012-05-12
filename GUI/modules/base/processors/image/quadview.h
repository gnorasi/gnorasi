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

#ifndef VRN_MULTIVIEW_H
#define VRN_MULTIVIEW_H

#include "voreen/core/processors/renderprocessor.h"
#include "voreen/core/properties/eventproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/vectorproperty.h"

namespace voreen {

class QuadView : public RenderProcessor {
public:
    QuadView();
    ~QuadView();

    virtual bool isReady() const;
    virtual void process();

    virtual std::string getCategory() const { return "View"; }
    virtual std::string getClassName() const { return "QuadView"; }
    virtual Processor::CodeState getCodeState() const { return CODE_STATE_STABLE; }
    virtual Processor* create() const;

    virtual void initialize() throw (tgt::Exception);

    virtual void invalidate(int inv = INVALID_RESULT);

    virtual void portResized(RenderPort* p, tgt::ivec2 newsize);
    virtual void sizeOriginChanged(RenderPort* p);
    bool testSizeOrigin(const RenderPort* /*p*/, void* /*so*/) const;

    virtual void onEvent(tgt::Event* e);
protected:
    void toggleMaximization(tgt::MouseEvent* me);
    void updateSizes();

    void mouseMove(tgt::MouseEvent* e);

    BoolProperty showGrid_;
    FloatVec4Property gridColor_;
    IntProperty maximized_;
    BoolProperty maximizeOnDoubleClick_;
    EventProperty<QuadView> maximizeEventProp_;
    EventProperty<QuadView> mouseMoveEventProp_;

    /// Inport whose rendering is mapped to the frame buffer.
    RenderPort outport_;

    RenderPort inport1_;
    RenderPort inport2_;
    RenderPort inport3_;
    RenderPort inport4_;

    int currentPort_;

    bool isDragging_;
};

} // namespace voreen

#endif // VRN_MULTIVIEW_H

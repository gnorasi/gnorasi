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

#include "voreen/qt/widgets/property/corepropertywidgetfactory.h"

#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/cameraproperty.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/fontproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/matrixproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/propertyvector.h"
#include "voreen/core/properties/shaderproperty.h"
#include "voreen/core/properties/stringexpressionproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/transfuncproperty.h"
#include "voreen/core/properties/vectorproperty.h"
#include "voreen/core/properties/volumecollectionproperty.h"
#include "voreen/core/properties/volumehandleproperty.h"
#include "voreen/core/properties/voxeltypeproperty.h"

#include "voreen/qt/widgets/property/boolpropertywidget.h"
#include "voreen/qt/widgets/property/buttonpropertywidget.h"
#include "voreen/qt/widgets/property/camerapropertywidget.h"
#include "voreen/qt/widgets/property/colorpropertywidget.h"
#include "voreen/qt/widgets/property/filedialogpropertywidget.h"
#include "voreen/qt/widgets/property/floatmat2propertywidget.h"
#include "voreen/qt/widgets/property/floatmat3propertywidget.h"
#include "voreen/qt/widgets/property/floatmat4propertywidget.h"
#include "voreen/qt/widgets/property/floatpropertywidget.h"
#include "voreen/qt/widgets/property/floatvec2propertywidget.h"
#include "voreen/qt/widgets/property/floatvec3propertywidget.h"
#include "voreen/qt/widgets/property/floatvec4propertywidget.h"
#include "voreen/qt/widgets/property/fontpropertywidget.h"
#include "voreen/qt/widgets/property/intpropertywidget.h"
#include "voreen/qt/widgets/property/intvec2propertywidget.h"
#include "voreen/qt/widgets/property/intvec3propertywidget.h"
#include "voreen/qt/widgets/property/intvec4propertywidget.h"
#include "voreen/qt/widgets/property/lightpropertywidget.h"
#include "voreen/qt/widgets/property/optionpropertywidget.h"
#include "voreen/qt/widgets/property/propertyvectorwidget.h"
#include "voreen/qt/widgets/property/shaderpropertywidget.h"
#include "voreen/qt/widgets/property/stringexpressionpropertywidget.h"
#include "voreen/qt/widgets/property/stringpropertywidget.h"
#include "voreen/qt/widgets/property/grouppropertywidget.h"
#include "voreen/qt/widgets/property/transfuncpropertywidget.h"
#include "voreen/qt/widgets/property/volumecollectionpropertywidget.h"
#include "voreen/qt/widgets/property/volumehandlepropertywidget.h"
#include "voreen/qt/widgets/property/voxeltypepropertywidget.h"

namespace voreen {

PropertyWidget* CorePropertyWidgetFactory::createWidget(Property* prop) const {

    if (dynamic_cast<BoolProperty*>(prop))
        return new BoolPropertyWidget(static_cast<BoolProperty*>(prop), 0);

    if (dynamic_cast<ButtonProperty*>(prop))
        return new ButtonPropertyWidget(static_cast<ButtonProperty*>(prop), 0);

    if (dynamic_cast<CameraProperty*>(prop))
        return new CameraPropertyWidget(static_cast<CameraProperty*>(prop), 0);

    if (dynamic_cast<FileDialogProperty*>(prop))
        return new FileDialogPropertyWidget(static_cast<FileDialogProperty*>(prop), 0);

    if (dynamic_cast<FloatProperty*>(prop))
        return new FloatPropertyWidget(static_cast<FloatProperty*>(prop), 0);

    if (dynamic_cast<FloatVec2Property*>(prop))
        return new FloatVec2PropertyWidget(static_cast<FloatVec2Property*>(prop), 0);

    if (dynamic_cast<FloatVec3Property*>(prop))
        return new FloatVec3PropertyWidget(static_cast<FloatVec3Property*>(prop), 0);

    if (dynamic_cast<FloatVec4Property*>(prop)) {
        FloatVec4Property* vec4Prop = static_cast<FloatVec4Property*>(prop);
        if (prop->getViews() == Property::DEFAULT)
            return new FloatVec4PropertyWidget(vec4Prop, 0);
        else if (prop->getViews() == Property::LIGHT_POSITION)
            return new LightPropertyWidget(vec4Prop, 0);
        else if (prop->getViews() == (Property::LIGHT_POSITION | Property::DEFAULT)) {
            GroupPropertyWidget* tab = new GroupPropertyWidget(vec4Prop, true, "");
            tab->addWidget(new LightPropertyWidget(vec4Prop, 0), "Widget");
            tab->addWidget(new FloatVec4PropertyWidget(vec4Prop, 0), "Vector");
            return tab;
        }
        else if (prop->getViews() == Property::COLOR) {
            return new ColorPropertyWidget(vec4Prop, 0);
        }
        else
            return new FloatVec4PropertyWidget(vec4Prop, 0);
    }

    if (dynamic_cast<FontProperty*>(prop))
        return new FontPropertyWidget(static_cast<FontProperty*>(prop), 0);

    if (dynamic_cast<IntProperty*>(prop))
        return new IntPropertyWidget(static_cast<IntProperty*>(prop), 0);

    if (dynamic_cast<IntVec2Property*>(prop))
        return new IntVec2PropertyWidget(static_cast<IntVec2Property*>(prop), 0);

    if (dynamic_cast<IntVec3Property*>(prop))
        return new IntVec3PropertyWidget(static_cast<IntVec3Property*>(prop), 0);

    if (dynamic_cast<IntVec4Property*>(prop))
        return new IntVec4PropertyWidget(static_cast<IntVec4Property*>(prop), 0);

    if (dynamic_cast<FloatMat2Property*>(prop))
        return new FloatMat2PropertyWidget(static_cast<FloatMat2Property*>(prop), 0);

    if (dynamic_cast<FloatMat3Property*>(prop))
        return new FloatMat3PropertyWidget(static_cast<FloatMat3Property*>(prop), 0);

    if (dynamic_cast<FloatMat4Property*>(prop))
        return new FloatMat4PropertyWidget(static_cast<FloatMat4Property*>(prop), 0);

    if (dynamic_cast<OptionPropertyBase*>(prop))
        return new OptionPropertyWidget(static_cast<OptionPropertyBase*>(prop), 0);

    if (dynamic_cast<PropertyVector*>(prop))
        return new PropertyVectorWidget(static_cast<PropertyVector*>(prop), 0);

    if (dynamic_cast<ShaderProperty*>(prop))
        return new ShaderPropertyWidget(static_cast<ShaderProperty*>(prop), 0);

    if (dynamic_cast<StringExpressionProperty*>(prop))
        return new StringExpressionPropertyWidget(static_cast<StringExpressionProperty*>(prop), 0);

    if (dynamic_cast<StringProperty*>(prop))
        return new StringPropertyWidget(static_cast<StringProperty*>(prop), 0);

    if (dynamic_cast<TransFuncProperty*>(prop))
        return new TransFuncPropertyWidget(static_cast<TransFuncProperty*>(prop), 0);

    if (dynamic_cast<VolumeHandleProperty*>(prop))
        return new VolumeHandlePropertyWidget(static_cast<VolumeHandleProperty*>(prop), 0);

    if (dynamic_cast<VolumeCollectionProperty*>(prop))
        return new VolumeCollectionPropertyWidget(static_cast<VolumeCollectionProperty*>(prop), 0);

    if (dynamic_cast<VoxelTypeProperty*>(prop))
        return new VoxelTypePropertyWidget(static_cast<VoxelTypeProperty*>(prop), 0);

    return 0;
}

} // namespace voreen

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

#include "knowledgeprocessorwidgetfactory.h"

#include "../../processors/dummysegmentationprocessor.h"
#include "dummysegmentationwidget.h"

#include "../../processors/classifierwsprocessor.h"
#include "classifierwswidget.h"

#include "voreen/qt/voreenapplicationqt.h"
#include <QWidget>
#include <QMainWindow>

namespace voreen {

ProcessorWidget* KnowledgeProcessorWidgetFactory::createWidget(Processor* processor) const {

    if (!VoreenApplicationQt::qtApp()) {
        LERRORC("voreen.KnowledgeProcessorWidgetFactory", "VoreenApplicationQt not instantiated");
        return 0;
    }
    QWidget* parent = VoreenApplicationQt::qtApp()->getMainWindow();

    if (dynamic_cast<DummySegmentationProcessor*>(processor))
        return new DummySegmentationWidget(parent, static_cast<DummySegmentationProcessor*>(processor));

    if (dynamic_cast<ClassifierWSProcessor*>(processor))
        return new ClassifierWSWidget(parent, static_cast<ClassifierWSProcessor*>(processor));

    return 0;
}
} // namespace voreen

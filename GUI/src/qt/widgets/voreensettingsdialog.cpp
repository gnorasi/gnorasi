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

#include "voreen/qt/widgets/voreensettingsdialog.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/core/voreenmodule.h"
#include "voreen/core/properties/property.h"
#include "voreen/qt/widgets/customlabel.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>

namespace voreen {

VoreenSettingsDialog::VoreenSettingsDialog(QWidget* parent)
    : QDialog(parent, Qt::Tool | Qt::Window)
{
    setWindowTitle(tr("Voreen settings editor"));

    // vertical layout of the widget
    QVBoxLayout* widgetLayout = new QVBoxLayout();
    setLayout(widgetLayout);

    // layout containing the property widgets
    QGridLayout* propertiesLayout = new QGridLayout();

    // scrollarea surrounding the property widgets
    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(propertiesLayout);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    widgetLayout->addWidget(scrollArea);

    std::vector<voreen::Property*> properties = VoreenApplication::app()->getProperties();

    // add property widgets to properties layout
    if (!VoreenApplication::app()) {
        LERRORC("voreen.qt.InputMappingDialog", "VoreenApplication not instantiated");
        return;
    }
    for (size_t i=0; i<properties.size(); ++i) {
        Property* prop = properties.at(i);
        PropertyWidget* propWidget = VoreenApplication::app()->createPropertyWidget(prop);
        if (propWidget) 
            prop->addWidget(propWidget);
        QPropertyWidget* qPropertyWidget = dynamic_cast<QPropertyWidget*>(propWidget);
        if (qPropertyWidget) {
            qPropertyWidget->hideLODControls();
            qPropertyWidget->setMinimumWidth(250);
            CustomLabel* nameLabel = qPropertyWidget->getNameLabel();
            int row = propertiesLayout->rowCount();
            propertiesLayout->addWidget(nameLabel, row, 1);
            propertiesLayout->addWidget(qPropertyWidget, row, 2);
        }
        else {
            LERRORC("voreen.qt.VoreenSettingsDialog", "Unable to create property widget");
        }
    }
}

} // namespace

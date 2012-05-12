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

#include "settingsdialog.h"
#include "tgt/gpucapabilities.h"
#include "voreen/qt/voreenapplicationqt.h"

#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QVBoxLayout>

namespace voreen {

const std::string SettingsDialog::loggerCat_ = "voreenve.SettingsDialog";

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent, Qt::Tool | Qt::Window)
{
    setWindowTitle(tr("Settings Editor"));
    setMinimumSize(QSize(475, 265));
    setMaximumSize(QSize(475, 265));
    

    QVBoxLayout* widgetLayout = new QVBoxLayout;

    QVBoxLayout* globalLayout = new QVBoxLayout(this);
    QWidget* scrollWidget = new QWidget(this);
    scrollWidget->setLayout(widgetLayout);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(scrollWidget);
    scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    scrollArea->setWidgetResizable(true);
    globalLayout->addWidget(scrollArea);

    QHBoxLayout* startupLayout = new QHBoxLayout;
    QLabel* startupLabel = new QLabel(tr("Show Startup Screen"));
    startupLayout->addWidget(startupLabel);
    startupLayout->addStretch(1);
    startupBox_ = new QCheckBox;
    startupBox_->setChecked(true);
    connect(startupBox_, SIGNAL(clicked(bool)), this, SLOT(changedSettings()));
    startupLayout->addWidget(startupBox_);
    widgetLayout->addLayout(startupLayout);
    widgetLayout->addStretch(1);

    QHBoxLayout* currentGPUMemoryLayout = new QHBoxLayout;
    QLabel* gpuMemoryLabel = new QLabel(tr("Currently available Graphics Memory"));
    currentGPUMemoryLayout->addWidget(gpuMemoryLabel);
    currentGPUMemoryLayout->addStretch(1);
    curGPUMemoryLabel_ = new QLabel(tr("1024.00 MB"));
    currentGPUMemoryLayout->addWidget(curGPUMemoryLabel_);
    refreshGPUMemoryButton_ = new QPushButton(tr("Refresh"));
    refreshGPUMemoryButton_->setToolTip(tr("Displays currently available graphics memory"));
    connect(refreshGPUMemoryButton_, SIGNAL(clicked(bool)), this, SLOT(updateGPUMemoryDisplay()));
    currentGPUMemoryLayout->addWidget(refreshGPUMemoryButton_);
    widgetLayout->addLayout(currentGPUMemoryLayout);
    widgetLayout->addStretch(1);

    QHBoxLayout* constraintGPUMemoryLayout = new QHBoxLayout;
    QLabel* constraintGPUMemoryLabel = new QLabel(tr("Constraint Graphics Memory Usage"));
    constraintGPUMemoryLayout->addWidget(constraintGPUMemoryLabel);
    constraintGPUMemoryLayout->addStretch(1);
    constraintGPUMemoryEnabled_ = new QCheckBox();
    connect(constraintGPUMemoryEnabled_, SIGNAL(clicked(bool)), this, SLOT(constraintGPUMemoryChecked(bool)));
    connect(constraintGPUMemoryEnabled_, SIGNAL(clicked(bool)), this, SLOT(changedSettings()));
    constraintGPUMemoryLayout->addWidget(constraintGPUMemoryEnabled_);
    widgetLayout->addLayout(constraintGPUMemoryLayout);
    widgetLayout->addStretch(1);

    QHBoxLayout* gpuMemoryLayout = new QHBoxLayout;
    QLabel* maxGPUMemoryLabel = new QLabel(tr("Maximal used Graphics Memory (MB)"));
    gpuMemoryLayout->addWidget(maxGPUMemoryLabel);
    gpuMemoryLayout->addStretch(1);
    gpuMemoryBox_ = new QSpinBox();
    gpuMemoryBox_->setMinimum(0);
    gpuMemoryBox_->setMaximum(96000);
    connect(gpuMemoryBox_, SIGNAL(valueChanged(int)), this, SLOT(changedSettings()));
    gpuMemoryLayout->addWidget(gpuMemoryBox_);
    widgetLayout->addLayout(gpuMemoryLayout);
    widgetLayout->addStretch(1);

    QSpacerItem* scrollStretchItem_ = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    globalLayout->addItem(scrollStretchItem_);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    resetButton_ = new QPushButton(tr("Reset"));
    resetButton_->setToolTip(tr("Clears all saved settings and will not save anything in this session"));
    connect(resetButton_, SIGNAL(clicked(bool)), this, SLOT(resetSettings()));
    bottomLayout->addWidget(resetButton_);
    bottomLayout->addStretch(1);
    okButton_ = new QPushButton(tr("OK"));
    okButton_->setToolTip(tr("Applies all changes and closes"));
    okButton_->setEnabled(false);
    connect(okButton_, SIGNAL(clicked(bool)), this, SLOT(acceptChanges()));
    bottomLayout->addWidget(okButton_);
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setToolTip(tr("Discard all changes and closes"));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    bottomLayout->addWidget(cancelButton);
    globalLayout->addLayout(bottomLayout);
}

void SettingsDialog::constraintGPUMemoryChecked(bool value) {
    gpuMemoryBox_->setEnabled(value);
}

void SettingsDialog::updateGPUMemoryDisplay() {
    std::stringstream features;
    features.precision(6);
    features.clear();
    features.str("");
    features << static_cast<float>(GpuCaps.getCurrentAvailableTextureMem(false))/1024.f << " MB";
    curGPUMemoryLabel_->setText(QString::fromStdString(features.str()));
}

void SettingsDialog::resetSettings() {
    startupBox_->setChecked(true);
    constraintGPUMemoryEnabled_->setChecked(false);
    gpuMemoryBox_->setValue(0);
    gpuMemoryBox_->setEnabled(false);

    okButton_->setEnabled(true);
    resetButton_->setEnabled(false);
    settingsReset_ = true;
}

void SettingsDialog::changedSettings() {
    okButton_->setEnabled(true);
    resetButton_->setEnabled(true);
    settingsReset_ = false;
}

void SettingsDialog::acceptChanges() {
    applySettings();
    close();
}

void SettingsDialog::applySettings() {
    QSettings settings;
    if(settingsReset_){
        settings.clear();
    }
    else{
        settings.setValue("showStartupScreen", startupBox_->isChecked());
        settings.setValue("maximumGPUMemory", gpuMemoryBox_->value());
        GpuCaps.setCurrentAvailableTextureMem(gpuMemoryBox_->value());
        settings.setValue("constraintGPUMemoryEnabled", constraintGPUMemoryEnabled_->isChecked());
        GpuCaps.setRetrieveAvailableTextureMem(!constraintGPUMemoryEnabled_->isChecked());
    }
    okButton_->setEnabled(false);
}

void SettingsDialog::showEvent(QShowEvent* e) {
    QSettings settings;
    if(!settings.contains("showStartupScreen"))
        resetButton_->setEnabled(false);

    startupBox_->setChecked(settings.value("showStartupScreen", QVariant(true)).toBool());
    constraintGPUMemoryEnabled_->setChecked(settings.value("constraintGPUMemoryEnabled", QVariant(false)).toBool());
    gpuMemoryBox_->setValue(settings.value("maximumGPUMemory", QVariant(0)).toInt());
    gpuMemoryBox_->setEnabled(settings.value("constraintGPUMemoryEnabled", QVariant(false)).toBool());

    okButton_->setEnabled(false);
    updateGPUMemoryDisplay();
    QDialog::showEvent(e);
}

} // namespace

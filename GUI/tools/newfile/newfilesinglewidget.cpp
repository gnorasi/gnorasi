/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Copyright (C) 2005-2010 The Voreen Team. <http://www.voreen.org>   *
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

#include "newfilesinglewidget.h"

#include "functions.h"
#include "mainwindow.h"

#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QTextEdit>
#include <QTextStream>

using std::runtime_error;

#define COPYFILESKEY "copyFilesOtherSingle"

NewFileSingleWidget::NewFileSingleWidget(MainWindow* parent)
    : NewWidget(parent)
    , fileName_(0)
    , location_(0)
    , copyFiles_(0)
{
    QGridLayout* layout = new QGridLayout(this);

    QLabel* fileLabel = new QLabel("File Name");
    layout->addWidget(fileLabel, 0, 0);
    fileName_ = new QLineEdit;
    fileName_->setMinimumWidth(180);
    layout->addWidget(fileName_, 0, 1, 1, 2);

    QLabel* headerLocationLabel = new QLabel("Location");
    layout->addWidget(headerLocationLabel, 1, 0);
    location_ = new QLineEdit;
    location_->setMinimumWidth(300);
    layout->addWidget(location_, 1, 1);
    QPushButton* headerFindButton = new QPushButton("...");
    headerFindButton->setToolTip("Select the future location of the header file");
    connect(headerFindButton, SIGNAL(clicked(bool)), this, SLOT(findPathButtonClicked()));
    layout->addWidget(headerFindButton, 1, 2, 1, 1, Qt::AlignRight);

    QSettings* settings = getSettings();
    QLabel* copyText = new QLabel("Copy new files to destination?");
    layout->addWidget(copyText, 5, 0);
    copyFiles_ = new QCheckBox;
    bool copyFilesChecked = settings->value(COPYFILESKEY, QVariant(true)).toBool();
    copyFiles_->setChecked(copyFilesChecked);
    layout->addWidget(copyFiles_, 5, 1);

    layout->setRowStretch(6, 1);

    layout->addWidget(controlWidget_, 7, 0, 1, 3);
    delete settings;
}

NewFileSingleWidget::~NewFileSingleWidget() {
    QSettings* settings = getSettings();
    settings->setValue(COPYFILESKEY, copyFiles_->isChecked());
    delete settings;
}

void NewFileSingleWidget::createFiles() {
    setDestinationDirectory("");
    QString fileName = fileName_->text();
    QString location = location_->text();

    if (!location.isEmpty() &&!location.endsWith("/"))
        location += "/";

    if (fileName.isEmpty()) {
        QMessageBox::critical(0, "Empty Filename", "A filename must be set");
        return;
    }

    try {
        QString destination = QDir::currentPath();
        if (fileName.endsWith(".h")) {
            //fileName = fileName.left(fileName.size() - 2); // 2 == length(".h")
            createHeaderFile(fileName.left(fileName.size() - 2));
            if (copyFiles_->isChecked())
                destination = copyFileToLocationPath(fileName, location);
        } else if (fileName.endsWith(".cpp")) {
            //fileName = fileName.left(fileName.size() - 4); // 4 == length(".cpp")
            createSourceFile(fileName.left(fileName.size() - 4), location);
            if (copyFiles_->isChecked())
                destination = copyFileToLocationPath(fileName, location);
        }
        else {
            throw runtime_error("Please provide an ending out of (.h .cpp)");
        }

        setDestinationDirectory(destination);
    }
    catch (std::exception& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
        return;
    }

    QMessageBox::information(this, "Success", "File creation was successful");
}

void NewFileSingleWidget::findPathButtonClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select the future location of the header file", getVoreenMainDirectory());

    if (dir == "")
        return;
    else {
        dir = dir.replace('\\', '/');
        location_->setText(dir);
    }
}

QString NewFileSingleWidget::getHelpText() const {
    return "<p><b>File Name:</b> The file name must be entered with the appropriate extension (currently only .h and .cpp\
           are supported).</p>\
           <p><b>Location:</b> The location is only needed if the 'Copy' checkbox is selected aswell.</p>";
}

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

#include "newfilepairwidget.h"

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

#define ADDTOPROJECTKEY "addToProjectOtherPair"
#define COPYFILESKEY "copyFilesOtherPair"

NewFilePairWidget::NewFilePairWidget(MainWindow* parent)
    : NewWidget(parent)
    , fileName_(0)
    , headerLocation_(0)
    , sourceLocation_(0)
    , includeEntry_(0)
    , addToProject_(0)
    , copyFiles_(0)
{
    QGridLayout* layout = new QGridLayout(this);

    QLabel* fileLabel = new QLabel(tr("File Name"));
    layout->addWidget(fileLabel, 0, 0);
    fileName_ = new QLineEdit;
    fileName_->setMinimumWidth(180);
    layout->addWidget(fileName_, 0, 1, 1, 2);

    QLabel* headerLocationLabel = new QLabel("Header Path");
    layout->addWidget(headerLocationLabel, 1, 0);
    headerLocation_ = new QLineEdit;
    headerLocation_->setMinimumWidth(300);
    layout->addWidget(headerLocation_, 1, 1);
    QPushButton* headerFindButton = new QPushButton("...");
    headerFindButton->setToolTip("Select the future location of the header file");
    connect(headerFindButton, SIGNAL(clicked(bool)), this, SLOT(findHeaderPathButtonClicked()));
    layout->addWidget(headerFindButton, 1, 2, 1, 1, Qt::AlignRight);

    QLabel* sourceLocationLabel = new QLabel("Source Path");
    layout->addWidget(sourceLocationLabel, 2, 0);
    sourceLocation_ = new QLineEdit;
    sourceLocation_->setMinimumWidth(300);
    layout->addWidget(sourceLocation_, 2, 1);
    QPushButton* sourceFindButton = new QPushButton("...");
    sourceFindButton->setToolTip("Select the future location of the source file");
    connect(sourceFindButton, SIGNAL(clicked(bool)), this, SLOT(findSourcePathButtonClicked()));
    layout->addWidget(sourceFindButton, 2, 2, 1, 1, Qt::AlignRight);

    QLabel* includePathLabel = new QLabel("Include Entry");
    layout->addWidget(includePathLabel, 3, 0);
    includeEntry_ = new QLineEdit;
    layout->addWidget(includeEntry_, 3, 1);
    QPushButton* includePathButton = new QPushButton("Generate");
    connect(includePathButton, SIGNAL(clicked(bool)), this, SLOT(generateIncludePath()));
    layout->addWidget(includePathButton, 3, 2, 1, 1, Qt::AlignRight);

    QSettings* settings = getSettings();
    QLabel* addText = new QLabel(tr("Add entry in project\nfile for new files?"));
    layout->addWidget(addText, 4, 0);
    addToProject_ = new QCheckBox;
    bool addToProjectChecked = settings->value(ADDTOPROJECTKEY, QVariant(true)).toBool();
    addToProject_->setChecked(addToProjectChecked);
    layout->addWidget(addToProject_, 4, 1);

    QLabel* copyText = new QLabel(tr("Copy new files to destination?"));
    layout->addWidget(copyText, 5, 0);
    copyFiles_ = new QCheckBox;
    bool copyFilesChecked = settings->value(COPYFILESKEY, QVariant(true)).toBool();
    copyFiles_->setChecked(copyFilesChecked);
    layout->addWidget(copyFiles_, 5, 1);

    layout->setRowStretch(6, 1);

    layout->addWidget(controlWidget_, 7, 0, 1, 3);
    delete settings;
}

NewFilePairWidget::~NewFilePairWidget() {
    QSettings* settings = getSettings();
    settings->setValue(ADDTOPROJECTKEY, addToProject_->isChecked());
    settings->setValue(COPYFILESKEY, copyFiles_->isChecked());
    delete settings;
}

void NewFilePairWidget::createFiles() {
    const QString& fileName = fileName_->text();
    if (fileName.isEmpty()) {
        QMessageBox::critical(0, "Empty Filename", "A filename name must be set");
        return;
    }

    if (includeEntry_->text().isEmpty() && !headerLocation_->text().isEmpty() && !sourceLocation_->text().isEmpty()) {
        try {
            generateIncludePath();
        }
        catch (std::exception& e) {
            QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
            return;
        }
    }

    setDestinationDirectory("");
    QString includePath = includeEntry_->text();
    QString headerPath = headerLocation_->text();
    QString sourcePath = sourceLocation_->text();

    if (includePath.startsWith('"'))
        includePath = includePath.remove(0, 1);
    if (includePath.endsWith('"'))
        includePath.chop(1);

    if (!includePath.isEmpty() && !includePath.endsWith("/"))
        includePath += "/";
    if (!headerPath.isEmpty() && !headerPath.endsWith("/"))
        headerPath += "/";
    if (!sourcePath.isEmpty() && !sourcePath.endsWith("/"))
        sourcePath += "/";

    try {
        QString destination = QDir::currentPath();
        createHeaderFile(fileName);
        createSourceFile(fileName, includePath);
        if (addToProject_->isChecked()) {
            if (includeEntry_->text() != USINGSAMEDIRECTORY) {
                addFileToProject(fileName.toLower() + ".h", includePath, FileTypeHeader);
                addFileToProject(fileName.toLower() + ".cpp", includePath, FileTypeSource);
            }
            else {
                QMessageBox::critical(this, "Error", "Automatic entry in project files doesn't work for identical directories.");
            }
        }

        if (copyFiles_->isChecked()) {
            if (!headerLocation_->text().isEmpty() && !sourceLocation_->text().isEmpty()) {
                copyFileToLocationPath(fileName + ".h", headerPath);
                destination = copyFileToLocationPath(fileName + ".cpp", sourcePath);
            } else if (!includeEntry_->text().isEmpty()) {
                copyFileToLocationInclude(fileName + ".h", includePath, FileTypeHeader);
                destination = copyFileToLocationInclude(fileName + ".cpp", includePath, FileTypeSource);
            }
            else {
                QMessageBox::critical(this, "Error", "No Include Path was given");
            }
        }

        setDestinationDirectory(destination);
    }
    catch (std::exception& e) {
        QMessageBox::critical(this, tr("Error"), QString::fromStdString(e.what()));
        return;
    }

    QMessageBox::information(this, tr("Success"), tr("File creation was successful"));
}

void NewFilePairWidget::findHeaderPathButtonClicked() {
    QString dir;
    if (sourceLocation_->text().isEmpty())
        dir = QFileDialog::getExistingDirectory(this, "Select the future location of the header file", getVoreenMainDirectory());
    else
        dir = QFileDialog::getExistingDirectory(this, "Select the future location of the header file", sourceLocation_->text());

    if (dir == "")
        return;
    else {
        dir = dir.replace('\\', '/');
        headerLocation_->setText(dir);
    }
}

void NewFilePairWidget::findSourcePathButtonClicked() {
    QString dir;
    if (headerLocation_->text().isEmpty())
        dir = QFileDialog::getExistingDirectory(this, "Select the future location of the source file", getVoreenMainDirectory());
    else
        dir = QFileDialog::getExistingDirectory(this, "Select the future location of the source file", headerLocation_->text());

    if (dir == "")
        return;
    else {
        dir = dir.replace('\\', '/');
        sourceLocation_->setText(dir);
    }
}

void NewFilePairWidget::generateIncludePath() {
    if (headerLocation_->text().isEmpty() && sourceLocation_->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Select destination folders for header and source files first");
        return;
    }
    else {
        if (headerLocation_->text().isEmpty()) {
            QMessageBox::critical(this, "Error", "Select destination folder for header file first");
            return;
        }
        else if (sourceLocation_->text().isEmpty()) {
            QMessageBox::critical(this, "Error", "Select destination folder for source file first");
            return;
        }
        else {
            try {
                QString includeText = convertDirectoryToInclude(headerLocation_->text(), sourceLocation_->text());
                includeEntry_->setText(includeText);
            }
            catch (std::exception& e) {
                QMessageBox::critical(this, "Error", e.what());
                return;
            }
        }
    }
}

QString NewFilePairWidget::getHelpText() const {
    return "<p><b>File Name:</b> The file name must be entered without an extension, as both .h and .cpp files will be created.</p>\
           <p><b>Include Entry:</b> The entry that will appear in the '#include' part of the source file. Must be provided either\
           by entering or by choosing 'Header Path' and 'Source Path' and clicking 'Generate'.</p>\
           <p><b>Super Class:</b> The super class of the new processor. New super classes can be added in the 'processors.ini'</p>\
           <p><b>Add Entries:</b> Tries to determine the correct project file and enters the new files into it. Doesn't work if\
           'Header Path' and 'Source Path' are the same.</p>";
}

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

#include "newprocessorwidget.h"

#include "functions.h"
#include "mainwindow.h"

#include <QCheckBox>
#include <QComboBox>
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

#define ADDTOPROJECTKEY "addToProjectProcessor"
#define COPYFILESKEY "copyFilesProcessor"

NewProcessorWidget::NewProcessorWidget(MainWindow* parent)
    : NewWidget(parent)
    , className_(0)
    , headerLocation_(0)
    , sourceLocation_(0)
    , includeEntry_(0)
    , superClass_(0)
    , addToProject_(0)
    , copyFiles_(0)
    , parent_(parent)
{
    QGridLayout* layout = new QGridLayout(this);

    QLabel* classLabel = new QLabel("Processor Classname");
    layout->addWidget(classLabel, 0, 0);
    className_ = new QLineEdit;
    className_->setMinimumWidth(180);
    layout->addWidget(className_, 0, 1, 1, 2);

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

    QLabel* superLabel = new QLabel("Super Class");
    layout->addWidget(superLabel, 4, 0);
    superClass_ = new QComboBox;
    initSuperClasses();
    layout->addWidget(superClass_, 4, 1, 1, 2);

    QSettings* settings = getSettings();
    QLabel* addText = new QLabel("Add entries in project file?");
    layout->addWidget(addText, 5, 0);
    addToProject_ = new QCheckBox;
    bool addToProjectChecked = settings->value(ADDTOPROJECTKEY, QVariant(true)).toBool();
    addToProject_->setChecked(addToProjectChecked);
    layout->addWidget(addToProject_, 5, 1, 1, 2);

    QLabel* copyText = new QLabel("Copy new files to destinations?");
    layout->addWidget(copyText, 6, 0);
    copyFiles_ = new QCheckBox;
    bool copyFilesChecked = settings->value(COPYFILESKEY, QVariant(true)).toBool();
    copyFiles_->setChecked(copyFilesChecked);
    layout->addWidget(copyFiles_, 6, 1);
    delete settings;

    layout->setRowStretch(7, 1);

    layout->addWidget(controlWidget_, 8, 0, 1, 3);
}

NewProcessorWidget::~NewProcessorWidget() {
    QSettings* settings = getSettings();
    settings->setValue(ADDTOPROJECTKEY, addToProject_->isChecked());
    settings->setValue(COPYFILESKEY, copyFiles_->isChecked());
    delete settings;
}

void NewProcessorWidget::initSuperClasses() {
    QSettings settings(getProcessorsFile(), QSettings::IniFormat);
    QStringList processors = settings.allKeys();
    superClass_->addItems(processors);

    int processorIndex = 0;
    for (int i = 0; i < processors.size(); ++i) {
        const QString& processor = processors[i];
        if (processor == "Processor") {
            processorIndex = i;
            break;
        }
    }
    superClass_->setCurrentIndex(processorIndex);
}

void NewProcessorWidget::createFiles() {
    const QString& className = className_->text();
    if (className.isEmpty() || className[0].isLower()) {
        QMessageBox::critical(0, "Empty Filename", "A class name must be set and it must start with a capital letter.");
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
        createProcessorHeaderFile(className, superClass_->currentText());
        createProcessorSourceFile(className, includePath, superClass_->currentText());
        if (addToProject_->isChecked()) {
            if (includeEntry_->text() != USINGSAMEDIRECTORY) {
                addFileToProject(className.toLower() + ".h", includePath, FileTypeHeader);
                addFileToProject(className.toLower() + ".cpp", includePath, FileTypeSource);
            }
            else {
                QMessageBox::critical(this, "Error", "Automatic entry in project files doesn't work for identical directories.");
            }
        }

        if (copyFiles_->isChecked()) {
            if (!headerLocation_->text().isEmpty() && !sourceLocation_->text().isEmpty()) {
                copyFileToLocationPath(className + ".h", headerPath);
                destination = copyFileToLocationPath(className + ".cpp", sourcePath);
            } else if (!includeEntry_->text().isEmpty()) {
                copyFileToLocationInclude(className + ".h", includePath, FileTypeHeader);
                destination = copyFileToLocationInclude(className + ".cpp", includePath, FileTypeSource);
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

void NewProcessorWidget::findHeaderPathButtonClicked() {
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

void NewProcessorWidget::findSourcePathButtonClicked() {
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

void NewProcessorWidget::generateIncludePath() {
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

QString NewProcessorWidget::getHelpText() const {
    return  "<p><b>ClassName:</b> The class name of the future processor. Must be given and must start with a capital letter.</p>\
            <p><b>Include Entry:</b> The entry that will appear in the '#include' part of the source file. Must be provided either\
            by entering or by choosing 'Header Path' and 'Source Path' and clicking 'Generate'.</p>\
            <p><b>Super Class:</b> The super class of the new processor. New super classes can be added in the 'processors.ini'</p>\
            <p><b>Add Entries:</b> Tries to determine the correct project file and enters the new files into it. Doesn't work if\
            'Header Path' and 'Source Path' are the same.</p>";
}

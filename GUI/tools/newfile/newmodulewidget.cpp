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

#include "newmodulewidget.h"

#include "functions.h"
#include "mainwindow.h"

#include <QCheckBox>
#include <QDir>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>

#include <stdexcept>

NewModuleWidget::NewModuleWidget(MainWindow* parent)
    : NewWidget(parent)
    , moduleName_(0)
    , internal_(0)
{
    QGridLayout* layout = new QGridLayout(this);

    QLabel* moduleName = new QLabel("Module Name");
    layout->addWidget(moduleName, 0, 0);
    moduleName_ = new QLineEdit;
    layout->addWidget(moduleName_, 0 , 1);

    QLabel* experimentalLabel = new QLabel("Internal Module?");
    layout->addWidget(experimentalLabel, 1, 0);
    internal_ = new QCheckBox;
    internal_->setChecked(true);
    layout->addWidget(internal_, 1, 1);

    layout->setRowStretch(2, 1);

    layout->addWidget(controlWidget_, 3, 0, 1, 2);
}

void NewModuleWidget::createFiles() {
    QDir modulesDirectory;
    try {
        modulesDirectory = createModuleDirectories();
    }
    catch (std::exception& e) {
        QMessageBox::critical(this, "Creating module directory failed", QString::fromStdString(e.what()));
        return;
    }

    setDestinationDirectory(modulesDirectory.absolutePath());

    createCommonFile(modulesDirectory);
    createCoreFile(modulesDirectory);
    createModuleFiles(modulesDirectory);

    QMessageBox::information(this, "Success", "Module creation was successful");
}

QDir NewModuleWidget::createModuleDirectories() const {
    QDir modulesDirectory(getModulesPath());

    QString moduleNameLower = moduleName_->text().toLower();

    if (modulesDirectory.exists(moduleNameLower))
        throw std::runtime_error("The entered module already exists and therefore cannot be created");
    else {
        bool success = modulesDirectory.mkdir(moduleNameLower);
        if (success) {
            modulesDirectory.cd(moduleNameLower);
            modulesDirectory.mkdir("src");
            modulesDirectory.mkdir("include");
        }
        else
            throw std::runtime_error("The module directory could not be created");
    }
    return modulesDirectory;
}

void NewModuleWidget::createCommonFile(const QDir& moduleDir) const {
    QString moduleNameLower = moduleName_->text().toLower();
    QString fileName = moduleNameLower + "_common.pri";
    QFile commonFile(fileName);
    commonFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&commonFile);

    stream << "DEFINES += VRN_MODULE_" << moduleName_->text().toUpper() << endl;
    stream << endl;
    stream << "# module class" << endl;
    stream << "VRN_MODULE_CLASSES += " << moduleName_->text() << "Module" << endl;
    stream << "VRN_MODULE_CLASS_HEADERS += " << moduleNameLower << "/" << moduleNameLower << "module.h" << endl;
    stream << "VRN_MODULE_CLASS_SOURCES += " << moduleNameLower << "/" << moduleNameLower << "module.cpp" << endl;

    commonFile.close();
    QString modulePath = moduleDir.absolutePath();
    QFile::copy(fileName, modulePath + "/" + fileName);
    QFile::remove(commonFile.fileName());
}

void NewModuleWidget::createCoreFile(const QDir& moduleDir) const {
    QString moduleNameLower = moduleName_->text().toLower();
    QString fileName = moduleNameLower + "_core.pri";
    QFile commonFile(fileName);
    commonFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&commonFile);

    stream << "#SOURCES += \\" << endl;
    stream << "    #$${VRN_MODULE_DIR}/" << moduleNameLower << "/src/" << endl;
    stream << endl;
    stream << "#HEADERS += \\" << endl;
    stream << "    #$${VRN_MODULE_DIR}/" << moduleNameLower << "/include/" << endl;

    commonFile.close();
    QString modulePath = moduleDir.absolutePath();
    QFile::copy(fileName, modulePath + "/" + fileName);
    QFile::remove(commonFile.fileName());

}

void NewModuleWidget::createModuleFiles(const QDir& moduleDir) const {
    QString moduleName = moduleName_->text();
    QString fileName = moduleName + "Module";
    createHeaderFile(fileName, "VoreenModule", "voreen/core/voreenmodule.h");

    // create Source
    QString lowerFileName = fileName.toLower();
    QFile sourceFile(lowerFileName + ".cpp");
    sourceFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&sourceFile);

    addGPLHeader(stream);
    stream << "#include \"modules/" << moduleName.toLower() << "/" << lowerFileName << ".h\"" << endl << endl;
    stream << "namespace voreen {" << endl;
    stream << endl;
    stream << fileName << "::" << fileName << "()" << endl;
    stream << "    : VoreenModule()" << endl;
    stream << "{" << endl;
    stream << "    setName(\"" << moduleName << "\");" << endl;
    stream << "    setXMLFileName(\"" << moduleName.toLower() << "/" << lowerFileName << ".xml\");" << endl;
    stream << "}" << endl;
    stream << endl;
    stream << "} // namespace" << endl;
    sourceFile.close();

    QFile xmlFile(lowerFileName + ".xml");
    xmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
    xmlFile.close();

    // move files
    QFile::copy(lowerFileName + ".h", moduleDir.absolutePath() + "/" + lowerFileName + ".h");
    QFile::remove(lowerFileName + ".h");
    QFile::copy(lowerFileName + ".cpp", moduleDir.absolutePath() + "/" + lowerFileName + ".cpp");
    QFile::remove(lowerFileName + ".cpp");
    QFile::copy(lowerFileName + ".xml", moduleDir.absolutePath() + "/" + lowerFileName + ".xml");
    QFile::remove(lowerFileName + ".xml");

    // moduleregistration
    QString moduleRegistrationFile = getModuleRegistrationFile();
    QFile modRegFile(moduleRegistrationFile);
    modRegFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream modRegStream(&modRegFile);

    QVector<QString> lineList;
    while (!modRegStream.atEnd()) {
        QString l = modRegStream.readLine();
        lineList.append(l);
    }

    QString newLine  = "#ifdef VRN_MODULE_" + moduleName.toUpper();
    QString newLine2 = "#include \"modules/" + moduleName.toLower() + "/" + fileName.toLower() + ".h\"";
    QString newLine3 = "#endif";

    if (internal_->isChecked()) {
        newLine = "    " + newLine;
        newLine2 = "    " + newLine2;
        newLine3 = "    " + newLine3;
    }

    bool isInCorrectBlock = true;
    if (internal_->isChecked())
        isInCorrectBlock = false;

    QString correctPosition;
    for (int i = 0; i < lineList.size(); ++i) {
        const QString& line = lineList[i];
        QString lineSimple = line.simplified();

        if (isInCorrectBlock) {
            if (lineSimple.startsWith("#ifdef VRN_MODULE_")) {
                int comparison = newLine.compare(line);
                if (comparison < 0) {
                    correctPosition = lineList[i].simplified();
                    lineList.insert(i, newLine);
                    lineList.insert(i+1, newLine2);
                    lineList.insert(i+2, newLine3);
                    lineList.insert(i+3, "");
                    break;
                }
            }
        }
        else {
            if (line == "#ifndef VRN_SNAPSHOT")
                isInCorrectBlock = true;
        }
    }

    isInCorrectBlock = false;
    for (int i = 0; i < lineList.size(); ++i) {
        const QString& line = lineList[i];
        QString lineSimple = line.simplified();
        if (line.startsWith("void addAllModules(VoreenApplication*"))
            isInCorrectBlock = true;

        if (isInCorrectBlock) {
            int comp = lineSimple.compare(correctPosition);
            if (comp == 0) {
                lineList.insert(i,   "    #ifdef VRN_MODULE_" + moduleName.toUpper());
                lineList.insert(i+1, "        vapp->addModule(new " + fileName + "());");
                lineList.insert(i+2, "    #endif");
                lineList.insert(i+3, "");
                break;
            }
        }
    }
    
    modRegFile.close();
    modRegFile.open(QIODevice::WriteOnly | QIODevice::Text);
    modRegStream.reset();
    foreach (const QString& line, lineList)
        modRegStream << line << endl;
    modRegFile.close();
}

QString NewModuleWidget::getHelpText() const {
    return "<p><b>Internal Module:</b> If this is checked, the module will be added in the internal section \
        of the generatemodules.h file. This section is reserved for modules which are (currently) not ready \
        for a public release and will be omitted in the creation of a snapshot release.</p>";
}

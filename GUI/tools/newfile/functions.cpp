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
 
#include "functions.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QVector>

#include <stdexcept>

using std::runtime_error;

QString getFilePath(QString file) {
    bool found = false;
    while (!found) {
        QFileInfo tmp(file);

        // If we arrived at a drive, we are finished looking and we didn't find anything
        QString currentDirectory = tmp.dir().absolutePath();
        QFileInfoList drives = QDir::drives();
        foreach (QFileInfo drive, drives) {
            if (drive.absolutePath() == currentDirectory)
                throw runtime_error("File not found");
        }

        if (tmp.exists())
            found = true;
        else
            file = "../" + file;
    }
    return file;
}

QSettings* getSettings() {
    return new QSettings("Voreen.org", "NewFile");
}

QString getSettingsFile() {
    return getFilePath("settings.ini");
}

QString getProcessorsFile() {
    return getFilePath("processors.ini");
}

QString getHeaderFileName() {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString headerPath = settings.value("VoreenHeaderSource").toString();
    return getFilePath(headerPath);
}

QString getCoreProjectFileName() {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString corePath = settings.value("PathToCorePro").toString();
    return getFilePath(corePath);
}

QString getQtProjectFileName() {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString qtPath = settings.value("PathToQtPro").toString();
    return getFilePath(qtPath);
}

QString getModulesProjectFileName(const QString& moduleName) {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString modulePath = settings.value("ModulesDirectory").toString() + moduleName + "/" + moduleName + settings.value("ModulesProExtension").toString();
    return getFilePath(modulePath);
}

QString getModulesPath() {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString modulePath = settings.value("ModulesDirectory").toString();
    return getFilePath(modulePath);
}

QString getModuleRegistrationFile() {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString moduleFile = settings.value("ModuleRegistrationFile").toString();
    return getFilePath(moduleFile);
}

QString getSuperInclude(const QString& superClass) {
    if (superClass == "VoreenModule") {
        QSettings settings(getSettingsFile(), QSettings::IniFormat);
        QString superInclude = settings.value("VoreenModulePath").toString();
        return superInclude;
    }
    else {
        QSettings settings(getProcessorsFile(), QSettings::IniFormat);
        QString superInclude = settings.value(superClass).toString();
        return superInclude;
    }
}

QString getVoreenMainDirectory() {
    QSettings settings(getSettingsFile(), QSettings::IniFormat);
    QString newFilePrefix = settings.value("NewFileDirectory").toString();
    QString current = QDir::currentPath();
    int index = current.indexOf(newFilePrefix);
    current = current.left(index);
    return current;
}


QString convertDirectoryToInclude(QString headerPath, QString sourcePath) {
    // test if headerPath and sourcePath are both subdirectories in the voreen hierarchy
    QString mainDir = getVoreenMainDirectory();
    if (!headerPath.startsWith(mainDir) || !sourcePath.startsWith(mainDir)) {
        QString error = "Directories must be subdirs of Voreen\n\nHeaderPath: %1\nSourcePath: %2\nMainDir: %3";
        error = error.arg(headerPath, sourcePath, mainDir);
        throw runtime_error(error.toStdString());
    }

    // remove trailing voreen main directory
    headerPath = headerPath.remove(0, mainDir.length());
    sourcePath = sourcePath.remove(0, mainDir.length());

    if (sourcePath == headerPath)
        return USINGSAMEDIRECTORY;

    // check if we are in the 'core' area
    if (sourcePath.startsWith("src/core/") && headerPath.startsWith("include/voreen/core/")) {
        headerPath = headerPath.remove(0, strlen("include/"));
        return headerPath;
    }
    else if (sourcePath.startsWith("src/core/") || headerPath.startsWith("include/voreen/core/")) {
        QString error = "Either the header directory or the source directory is in the 'core' area, but not "
                        "both.\n\n HeaderPath: %1\nSourcePath: %2";
        error = error.arg(headerPath, sourcePath);
        throw runtime_error(error.toStdString());
    }

    // check if we are in the 'qt' area
    if (sourcePath.startsWith("src/qt/") && headerPath.startsWith("include/voreen/qt/")) {
        headerPath = headerPath.remove(0, strlen("include/"));
        return headerPath;
    }
    else if (sourcePath.startsWith("src/qt/") || headerPath.startsWith("include/voreen/qt/")) {
        QString error = "Either the header directory or the source directory is in the 'qt' area, but not "
                        "both.\n\n HeaderPath: %1\nSourcePath: %2";
        error = error.arg(headerPath, sourcePath);
        throw runtime_error(error.toStdString());
    }

    // check if we are in the 'modules' area
    if (sourcePath.startsWith("modules/") && headerPath.startsWith("modules/")) {
        return headerPath;
    }
    else if (sourcePath.startsWith("modules/") || headerPath.startsWith("modules/")) {
        QString error = "Either the header directory or the source directory is in the 'modules' area, but not "
                        "both.\n\n HeaderPath: %1\nSourcePath: %2";
        error = error.arg(headerPath, sourcePath);
        throw runtime_error(error.toStdString());
    }

    QString error = "Currently only the 'core', 'qt' and 'modules' directories are supported for automatic include "
                    "path generation.\n\nHeaderPath: %1\nSourcePath: %2";
    error = error.arg(headerPath, sourcePath);
    throw runtime_error(error.toStdString());
}

void addGPLHeader(QTextStream& fileStream) {
    const QString& gplFileName = getHeaderFileName();
    QFile gplFile(gplFileName);
    gplFile.open(QIODevice::ReadOnly);

    QTextStream stream(&gplFile);
    QString tmp;
    while (!stream.atEnd()) {
        tmp = stream.readLine();
        if (!tmp.isEmpty())
            fileStream << tmp << endl;
    }
    fileStream << "\n";
}

QFile* createSourceFile(const QString& fileName, const QString& location) {
    QString lowerFileName = fileName.toLower();
    QFile* file = new QFile(lowerFileName + ".cpp");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(file);

    addGPLHeader(stream);
    if (location != "")
        stream << "#include \"" << location << lowerFileName << ".h\"" << endl << endl;
    stream << "namespace voreen {" << endl;
    stream << endl << endl;
    stream << "} // namespace" << endl;
    file->close();
    return file;
}

QFile* createProcessorSourceFile(const QString& fileName, const QString& location, const QString& superClass) {
    QString lowerFileName = fileName.toLower();
    QFile* file = new QFile(lowerFileName + ".cpp");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(file);

    addGPLHeader(stream);
    stream << "#include \"" << location << lowerFileName << ".h\"" << endl << endl;
    stream << "namespace voreen {" << endl;
    stream << endl;
    stream << fileName << "::" << fileName << "()" << endl;
    stream << "    : " << superClass << "()" << endl;
    stream << "{" << endl;
    stream << "}" << endl;
    stream << endl;
    stream << "void " << fileName << "::process() {" << endl;
    stream << endl;
    stream << "}" << endl;
    stream << endl;
    stream << "} // namespace" << endl;

    file->close();
    return file;
}

QFile* createHeaderFile(const QString& fileName, const QString& superClass, const QString& includePath) {
    QString lowerFileName = fileName.toLower();
    QString upperFileName = fileName.toUpper();

    QFile* file = new QFile(lowerFileName + ".h");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(file);

    addGPLHeader(stream);
    stream << "#ifndef VRN_" << upperFileName << "_H" << endl;
    stream << "#define VRN_" << upperFileName << "_H" << endl;
    stream << endl;
    if (includePath != "")
        stream << "#include \"" << includePath << "\"" << endl << endl;
    stream << "namespace voreen {" << endl;
    stream << endl;
    if (superClass != "") {
        stream << "class " << fileName << " : public " << superClass << " {" << endl;
        stream << "public:" << endl;
        stream << "    " << fileName << "();";
        stream << endl;
        stream << "};" << endl;
    }
    stream << endl;
    stream << "} // namespace" << endl;
    stream << endl;
    stream << "#endif // VRN_" << upperFileName << "_H" << endl;

    file->close();
    return file;
}

QFile* createProcessorHeaderFile(const QString& fileName, const QString& superClass) {
    QString lowerFileName = fileName.toLower();
    QString upperFileName = fileName.toUpper();

    QFile* file = new QFile(lowerFileName + ".h");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(file);

    QString codeState = QString::fromStdString("CODE_STATE_EXPERIMENTAL");

    addGPLHeader(stream);
    stream << "#ifndef VRN_" << upperFileName << "_H" << endl;
    stream << "#define VRN_" << upperFileName << "_H" << endl;
    stream << endl;
    stream << "#include \"" << getSuperInclude(superClass) << "\"" << endl;
    stream << endl;
    stream << "namespace voreen {" << endl;
    stream << endl;
    stream << "class " << fileName << " : public " << superClass << " {" << endl;
    stream << "public:" << endl;
    stream << "    " << fileName << "();";
    stream << endl;
    int len = std::max(fileName.length() + 4, codeState.length());
    QString filler(len - fileName.length() - 2, ' ');
    stream << "    std::string getClassName() const   { return \"" << fileName << "\";" << filler << " }" << endl;
    filler = QString(len, ' ');
    stream << "    std::string getCategory() const    { return " << filler << "; }" << endl;
    filler = QString(len - codeState.length(), ' ');
    stream << "    CodeState getCodeState() const     { return " << codeState << ";"  << filler << " }" << endl;
    stream << endl;
    filler = QString(len - fileName.length()- 4, ' ');
    stream << "    Processor* create() const          { return new " << fileName << ";" << filler << " }" << endl;
    stream << endl;
    stream << "protected:" << endl;
    stream << "    void process();" << endl;
    stream << endl;
    stream << "};" << endl;
    stream << endl;
    stream << "} // namespace" << endl;
    stream << endl;
    stream << "#endif // VRN_" << upperFileName << "_H" << endl;

    file->close();
    return file;
}

void splitLocation(const QString& location, QString& projectFileName, QString& leadingPath, QString& restPath, bool& module) {
    // determine to which project file the new files might belong
    if (location.startsWith("voreen/")) {
        QString shortenedLocation = location.right(location.size() - 7); // 7 = strlen("voreen/")
        int slashIndex = shortenedLocation.indexOf("/");
        leadingPath = shortenedLocation.left(slashIndex);
        restPath = shortenedLocation.right(shortenedLocation.size() - slashIndex - 1); // -1 because we don't want the extra '/'

        if (leadingPath == "core")
            projectFileName = getCoreProjectFileName();
        else if (leadingPath == "qt")
            projectFileName = getQtProjectFileName();
        else
            throw runtime_error("'core' or 'qt' was expected after a location starting with 'voreen/'");
        module = false;
    }
    else {
        QString shortenedLocation = location.right(location.size() - 8); // 8 = strlen("modules/")
        int slashIndex = shortenedLocation.indexOf("/");
        leadingPath = shortenedLocation.left(slashIndex);
        restPath = shortenedLocation.right(shortenedLocation.size() - slashIndex - 1); // -1 because we don't want the extra '/'

        QString moduleProjectFileName = getModulesProjectFileName(leadingPath);

        if (!moduleProjectFileName.isEmpty())
            projectFileName = moduleProjectFileName;
        else {
            std::string message = "The project file for the module '" + leadingPath.toStdString() + "' could not be found.";
            throw runtime_error(message.c_str());
        }
        module = true;
    }
}

void addFileToProject(const QString& fileName, const QString& location, FileType type) {
    // if the location is empty nothing can be done here
    if (location.isEmpty())
        throw runtime_error("Location field is empty and therefore no project file could be determined.");

    // the location has to start with either "voreen/" (indicating core or qt classes)
    // or with "modules/" (indicating module classes). If neither -> bail out
    if (!(location.startsWith("voreen/") || (location.startsWith("modules/"))))
        throw runtime_error("Location field has to start with either 'voreen' or 'modules'.");

    QString projectFileName, leadingPath, restPath;
    bool inModule;
    splitLocation(location, projectFileName, leadingPath, restPath, inModule);

    QString fullName = restPath + fileName;

    if ((type == FileTypeHeader) && (!fullName.endsWith(".h")))
        fullName += ".h";
    else if ((type == FileTypeSource) && (!fullName.endsWith(".cpp")))
        fullName += ".cpp";

    if (type == FileTypeHeader) {
        if (inModule) {
            restPath = "$${VRN_MODULE_DIR}/" + leadingPath + "/" + restPath.toLower();
            fullName = "$${VRN_MODULE_DIR}/" + leadingPath + "/" + fullName.toLower();
        }
        else {
            restPath = "../../include/voreen/" + leadingPath + "/" + restPath.toLower();
            fullName = "../../include/voreen/" + leadingPath + "/" + fullName.toLower();
        }
    }
    else {
        if (restPath.startsWith("include/")) {
            int includeIndex = strlen("include/");
            restPath.remove(0, includeIndex);
            fullName.remove(0, includeIndex);
        }

        if (inModule) {
            restPath = "$${VRN_MODULE_DIR}/" + leadingPath + "/src/" + restPath.toLower();
            fullName = "$${VRN_MODULE_DIR}/" + leadingPath + "/src/" + fullName.toLower();
        }
        // no else necessary
    }

    QFile projectFile(projectFileName);
    projectFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&projectFile);

    QVector<QString> lineList;
    while (!stream.atEnd()) {
        QString l = stream.readLine();
        lineList.append(l);
    }

    int sizeBefore = lineList.size();
    bool isInCorrectBlock = false;
    for (int i = 0; i < lineList.size(); ++i) {
        const QString& line = lineList[i];
        QString simpleLine = line.simplified();
        QString simpleRest = restPath.simplified();
        QString simpleFull = fullName.simplified();

        if (simpleLine.startsWith(simpleRest)) {
            isInCorrectBlock = true;
            int comparison = simpleFull.compare(simpleLine);
            if (comparison < 0) {
                QString newLine = "    " + simpleFull;
                const QString& lastLine = lineList[i-1];
                if (lastLine.endsWith("\\"))
                    newLine += " \\";
                else {
                    QString lastLine = lineList.at(i-1);
                    lastLine += " \\";
                    lineList.replace(i-1, lastLine);
                }
                lineList.insert(i, newLine);
                break;
            }
        }
        else if (isInCorrectBlock) {
            // We passed the correct block but all entries were smaller
            // -> add the new entry and add "\" behind the last one if needed

            QString newLine = "    " + simpleFull;
            const QString& lastLine = lineList[i-1];
            if (lastLine.endsWith("\\"))
                newLine += " \\";
            else {
                QString lastLine = lineList.at(i-1);
                lastLine += " \\";
                lineList.replace(i-1, lastLine);
            }

            lineList.insert(i, newLine);
            break;
        }
    }
    int sizeAfter = lineList.size();
    if (sizeAfter != sizeBefore + 1)
        throw runtime_error("Could not enter the new files in the project file.\nFor now, new directories have to be entered manually.");

    projectFile.close();
    projectFile.open(QIODevice::WriteOnly | QIODevice::Text);
    stream.reset();
    foreach (const QString& line, lineList)
        stream << line << endl;
    projectFile.close();
}

QString copyFileToLocationInclude(const QString& fileName, const QString& location, FileType type) {
    // if the location is empty nothing can be done here
    if (location.isEmpty())
        throw runtime_error("Location field is empty and therefore no destination could be determined.");

    // the location has to start with either "voreen/" (indicating core or qt classes)
    // or with "modules/" (indicating module classes). If neither -> bail out
    if (!(location.startsWith("voreen/") || (location.startsWith("modules/"))))
        throw runtime_error("Location field has to start with either 'voreen' or 'modules'.");

    QString projectFileName, leadingPath, restPath;
    bool inModule;
    splitLocation(location, projectFileName, leadingPath, restPath, inModule);

    QString path;
    if (type == FileTypeHeader) {
        if (inModule)
            path = projectFileName.left(projectFileName.lastIndexOf("/")) + "/" + restPath;
        else
            path = "../../include/voreen/" + leadingPath + "/" + restPath;

    } else {
        if (restPath.startsWith("include/")) {
            int includeIndex = strlen("include/");
            restPath.remove(0, includeIndex);
        }

        if (inModule)
            path = projectFileName.left(projectFileName.lastIndexOf("/")) + "/src/" + restPath;
        else
            path = projectFileName.left(projectFileName.lastIndexOf("/")) + "/" + restPath;
    }

    return copyFileToLocationPath(fileName, path);
}

QString copyFileToLocationPath(const QString& fileName, const QString& path) {
    QDir dir(path);
    if (!dir.exists())
        throw runtime_error("The directory doesn't exist.");

    QFile::copy(fileName.toLower(), path + fileName.toLower());
    QFile::remove(fileName.toLower());
    return path;
}

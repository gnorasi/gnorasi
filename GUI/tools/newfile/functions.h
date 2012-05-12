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
 
#ifndef VRN_FUNCTIONS_H
#define VRN_FUNCTIONS_H

#include <QString>
#include <QTextStream>

class QFile;
class QSettings;

#define USINGSAMEDIRECTORY "<Using same directory>"

enum FileType {
    FileTypeHeader,
    FileTypeSource
};

QSettings* getSettings();

QString getSettingsFile();
QString getProcessorsFile();

QString getHeaderFileName();
QString getCoreProjectFileName();
QString getQtProjectFileName();
QString getModulesProjectFileName(const QString& moduleName);
QString getModulesPath();
QString getModuleRegistrationFile();
QString getSuperInclude();
QString getVoreenMainDirectory();

void addGPLHeader(QTextStream& fileStream);

QString convertDirectoryToInclude(QString headerPath, QString sourcePath);

QFile* createSourceFile(const QString& fileName, const QString& location = "");
QFile* createProcessorSourceFile(const QString& fileName, const QString& location, const QString& superClass);
QFile* createHeaderFile(const QString& fileName, const QString& superClass = "", const QString& includePath = "");
QFile* createProcessorHeaderFile(const QString& fileName, const QString& superClass);

void splitLocation(const QString& location, QString& projectFileName, QString& leadingPath, QString& restPath, bool& module);
void addFileToProject(const QString& fileName, const QString& location, FileType type);
QString copyFileToLocationInclude(const QString& fileName, const QString& location, FileType type);
QString copyFileToLocationPath(const QString& fileName, const QString& path);

#endif // VRN_FUNCTIONS_H

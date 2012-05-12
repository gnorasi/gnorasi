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
 
#ifndef VRN_NEWWIDGET_H
#define VRN_NEWWIDGET_H

#include <QWidget>

class MainWindow;
class QPushButton;

class NewWidget : public QWidget {
Q_OBJECT
public:
    NewWidget(MainWindow* parent);

public slots:
    void showHelp() const;

private slots:
    virtual void createFiles() = 0;
    void gotoDirectory() const;

protected:
    virtual QString getHelpText() const = 0;
    void setDestinationDirectory(const QString& directory);

    MainWindow* parent_;

    QWidget* controlWidget_;
    QPushButton* helpButton_;
    QPushButton* gotoButton_;
    QPushButton* createButton_;
    QString destinationDirectory_;
};

#endif // VRN_NEWWIDGET_H

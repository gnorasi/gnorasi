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

#include "newwidget.h"

#include "mainwindow.h"
#include <QDesktopServices>
#include <QDir>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QUrl>

NewWidget::NewWidget(MainWindow* parent)
    : parent_(parent)
{
    controlWidget_ = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(controlWidget_);

    helpButton_ = new QPushButton(tr("Help"));
    connect(helpButton_, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
    layout->addWidget(helpButton_);

    layout->addStretch();

    gotoButton_ = new QPushButton(tr("Goto Destination"));
    connect(gotoButton_, SIGNAL(clicked(bool)), this, SLOT(gotoDirectory()));
    gotoButton_->setEnabled(false);
    gotoButton_->setToolTip("Opens up the file browser in the directory where the last files were created.");
    layout->addWidget(gotoButton_);

    layout->addStretch();

    createButton_ = new QPushButton(tr("Create"));
    createButton_->setDefault(true);
    connect(createButton_, SIGNAL(clicked(bool)), this, SLOT(createFiles()));
    layout->addWidget(createButton_);
}

void NewWidget::showHelp() const {
    QTextEdit* helpWidget = parent_->getHelpWidget();
    QString helpText = getHelpText();
    helpWidget->setText(helpText);
    helpWidget->show();
}

void NewWidget::setDestinationDirectory(const QString& directory) {
    destinationDirectory_ = directory;
    gotoButton_->setEnabled(directory != "");
}

void NewWidget::gotoDirectory() const {
    QDir dir(destinationDirectory_);
    QUrl url = QUrl::fromLocalFile(dir.absolutePath());
    QDesktopServices::openUrl(url);
}

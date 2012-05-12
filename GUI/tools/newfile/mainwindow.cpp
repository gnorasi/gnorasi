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

#include "mainwindow.h"

#include "newfilepairwidget.h"
#include "newfilesinglewidget.h"
#include "newmodulewidget.h"
#include "newprocessorwidget.h"

#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent)
    : QTabWidget(parent)
    , helpWidget_(0)
{
    helpWidget_ = new QTextEdit(this);
    helpWidget_->setWindowFlags(Qt::Tool);
    helpWidget_->setReadOnly(true);
    
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(setTitle(int)));

    newProcessorWidget_ = new NewProcessorWidget(this);
    addTab(newProcessorWidget_, "Processor");

    newModuleWidget_ = new NewModuleWidget(this);
    addTab(newModuleWidget_, "Module");

    newFilePairWidget_ = new NewFilePairWidget(this);
    addTab(newFilePairWidget_, "Other File (Pair)");

    newFileSingleWidget_ = new NewFileSingleWidget(this);
    addTab(newFileSingleWidget_, "Other File (Single)");

    helpWidget_->hide();
}

QTextEdit* MainWindow::getHelpWidget() const {
    // Move help window to the right border
    QRect r = frameGeometry();
    helpWidget_->move(r.right(), r.top());
    helpWidget_->setFixedHeight(height());
    return helpWidget_;
}

void MainWindow::moveEvent(QMoveEvent*) {
    QRect r = frameGeometry();
    helpWidget_->move(r.right(), r.top());
}

void MainWindow::setTitle(int index) {
    switch (index) {
    case 0:
        {
            setWindowTitle("New Processor");
            helpWidget_->setWindowTitle("New Processor --- Help");
            break;
        }
    case 1:
        {
            setWindowTitle("New Module");
            helpWidget_->setWindowTitle("New Module --- Help");
            break;
        }
    case 2:
        {
            setWindowTitle("New File (Pair)");
            helpWidget_->setWindowTitle("New File (Pair) --- Help");
            break;
        }
    case 3:
        {
            setWindowTitle("New File (Single)");
            helpWidget_->setWindowTitle("New File (Single) --- Help");
            break;
        }
    }

    if (helpWidget_->isVisible()) {
        NewWidget* widget = dynamic_cast<NewWidget*>(currentWidget());
        widget->showHelp();
    }
}

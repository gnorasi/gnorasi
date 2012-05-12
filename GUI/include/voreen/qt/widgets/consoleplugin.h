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

#ifndef CONSOLEPLUGIN_H
#define CONSOLEPLUGIN_H

#include <QWidget>
#include <QKeyEvent>

#include "voreen/qt/voreenqtdefine.h"
#include "tgt/logmanager.h"

class QTextEdit;

namespace voreen {

class ConsoleLogQt;

class VRN_QT_API ConsolePlugin : public QWidget {
Q_OBJECT
public:
    ConsolePlugin(QWidget* parent = 0, tgt::LogLevel logLevel = tgt::Info, bool autoScroll = true);
    ~ConsolePlugin();

    void log(const std::string& msg);

public slots:
    void showContextMenu(const QPoint &pt);

private:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);

    ConsoleLogQt* log_;
    QTextEdit* consoleText_;
    QAction* clearText_;
    QAction* disableAction_;
    bool autoScroll_;
    bool ctrlButtonDown_;

private slots:
    void disableToggled();

};

} // namespace voreen

#endif

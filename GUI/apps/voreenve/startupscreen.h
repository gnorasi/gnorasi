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

#ifndef VRN_STARTUPSCREEN_H
#define VRN_STARTUPSCREEN_H

#include <QFrame>

#include "tutorialvideofetcher.h"

class QCheckBox;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QToolButton;
class QLabel;

namespace voreen {

// ------------------------------------------------------------------------------------------------

class StartupScreen : public QFrame {
Q_OBJECT
public:
    StartupScreen(QWidget* parent = 0);

    QPoint getPixmapPosition();

signals:
    void selectedRecentWorkspace(const QString& recent);
    void selectedTemplate(const QString& t);
    void selectedNewWorkspace();
    void closeSignal();

    void gettingStartedSignal();
    void helpTutorialSlides();
    void helpAnimation();

private slots:
    void recentsButtonClicked();
    void templateButtonClicked();
    void tutorialButtonClicked();
    void helpButtonClicked();
    void closeButtonClicked();
    void itemDoubleClicked(QListWidgetItem* item);

private:
    QLabel* pixmap_;
    QToolButton* recentsButton_;
    QToolButton* templatesButton_;
    QToolButton* tutorialButton_;
    QToolButton* helpButton_;
    QListWidget* list_;
    QCheckBox* dontShowAgain_;

    TutorialVideoFetcher videoFetcher_;
};


} // namespace

#endif // VRN_STARTUPSCREEN_H

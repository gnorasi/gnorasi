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

#include "startupscreen.h"

#include "templateworkspacefetcher.h"
#include "voreen/core/voreenapplication.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QDesktopServices>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>


namespace voreen {

namespace {
    const int FULLFILENAMEROLE = Qt::UserRole + 1;
    const int DESCRIPTIONROLE = Qt::UserRole + 2;
    const int URLROLE = Qt::UserRole + 3;
    const int UPDATEDROLE = Qt::UserRole + 5;

    const qreal edgeSpacing = 40.f;

    enum {
        GETTING_STARTED = 0,
        TUTORIAL_SLIDES = 1,
        ANIMATION_MANUAL = 2
    };
}

StartupScreen::StartupScreen(QWidget* parent)
    : QFrame(parent, Qt::FramelessWindowHint | Qt::Dialog)
    , list_(0)
    , dontShowAgain_(0)
{
    setWindowModality(Qt::ApplicationModal);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setWindowTitle("VoreenVE");

    QFile file(":/voreenve/widgetstyle/startupscreen.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);
   
    setMinimumHeight(400);
    setMinimumWidth(480);

    QPixmap startupBanner = QPixmap(":/voreenve/image/startupwidget.png");
    pixmap_ = new QLabel;
    pixmap_->setPixmap(startupBanner);

    recentsButton_ = new QToolButton;
    recentsButton_->setText("Recents");
    recentsButton_->setMinimumSize(QSize(75,75));
    recentsButton_->setMaximumSize(QSize(75,75));
    recentsButton_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    recentsButton_->setCheckable(true);
    recentsButton_->setAutoRaise(true);
    recentsButton_->setIcon(QIcon(":/voreenve/icons/recents.png"));
    recentsButton_->setIconSize(QSize(40, 40));
    connect(recentsButton_, SIGNAL(clicked(bool)), this, SLOT(recentsButtonClicked()));
    templatesButton_ = new QToolButton;
    templatesButton_->setText("Templates");
    templatesButton_->setMinimumSize(QSize(75,75));
    templatesButton_->setMaximumSize(QSize(75,75));
    templatesButton_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    templatesButton_->setCheckable(true);
    templatesButton_->setAutoRaise(true);
    templatesButton_->setIcon(QIcon(":/voreenve/icons/template.png"));
    templatesButton_->setIconSize(QSize(40, 40));
    connect(templatesButton_, SIGNAL(clicked(bool)), this, SLOT(templateButtonClicked()));
    tutorialButton_ = new QToolButton;
    tutorialButton_->setText("Tutorials");
    tutorialButton_->setMinimumSize(QSize(75,75));
    tutorialButton_->setMaximumSize(QSize(75,75));
    tutorialButton_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tutorialButton_->setIconSize(QSize(40, 40));
    tutorialButton_->setIcon(QIcon(":/voreenve/icons/videoreen.png"));
    tutorialButton_->setCheckable(true);
    tutorialButton_->setAutoRaise(true);
    connect(tutorialButton_, SIGNAL(clicked(bool)), this, SLOT(tutorialButtonClicked()));
    helpButton_ = new QToolButton;
    helpButton_->setText("Documentation");
    helpButton_->setMinimumSize(QSize(75,75));
    helpButton_->setMaximumSize(QSize(75,75));
    helpButton_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    helpButton_->setIconSize(QSize(40, 40));
    helpButton_->setIcon(QIcon(":/voreenve/icons/question1.png"));
    helpButton_->setCheckable(true);
    helpButton_->setAutoRaise(true);
    connect(helpButton_, SIGNAL(clicked(bool)), this, SLOT(helpButtonClicked()));

    QButtonGroup* group = new QButtonGroup;
    group->addButton(recentsButton_);
    group->addButton(templatesButton_);
    group->addButton(tutorialButton_);
    group->addButton(helpButton_);

    dontShowAgain_ = new QCheckBox("Don't show again");
    QSettings settings;
    bool show = settings.value("showStartupScreen", QVariant(true)).toBool();
    dontShowAgain_->setChecked(!show);

    QPushButton* closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(closeButtonClicked()));

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setRowMinimumHeight(2, 15);
    mainLayout->setRowMinimumHeight(4, 5);

    mainLayout->addWidget(pixmap_, 0, 0, 1, 2);

    QBoxLayout* topLeftLayout = new QVBoxLayout;
    topLeftLayout->addWidget(recentsButton_);
    topLeftLayout->addWidget(templatesButton_);
    topLeftLayout->addWidget(tutorialButton_);
    topLeftLayout->addWidget(helpButton_);
    topLeftLayout->addStretch(-1);
    mainLayout->addLayout(topLeftLayout, 1, 0);

    list_ = new QListWidget;
    list_->setSpacing(2);
    connect(list_, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
    mainLayout->addWidget(list_, 1, 1);

    QBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->addSpacing(10);
    bottomLayout->addWidget(dontShowAgain_);
    bottomLayout->addSpacing(250);
    bottomLayout->addWidget(closeButton);
    bottomLayout->addSpacing(5);
    
    mainLayout->addLayout(bottomLayout, 3, 0, 1, 2);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    QStringList recents = settings.value("recentFileList").toStringList();
    if (recents.size() > 0) {
        if (recents.size() == 1 && recents.at(0).contains("standard.vws"))
            templatesButton_->click();
        else
            recentsButton_->click();
    }
    else
        templatesButton_->click();
}

void StartupScreen::recentsButtonClicked() {
    QSettings settings;
    list_->clear();

    QStringList recents = settings.value("recentFileList").toStringList();
    QStringList displayName = recents;

    QString basePath = QString::fromStdString(VoreenApplication::app()->getBasePath());
    basePath = basePath.replace("\\", "/");
    for (int i = 0; i < displayName.size(); ++i) {
        QString& s = displayName[i].replace("\\", "/");;
        if (s.startsWith(basePath, Qt::CaseInsensitive))
            s = s.right(s.length() - (basePath.length() + 1));
    }

    for (int i = 0; i < recents.size(); ++i) {
        const QString& recent = recents.at(i);

        QString filename = displayName.at(i);
        QString display = filename;
        if (filename.indexOf("/") != -1) {
            QString path = filename.left(filename.lastIndexOf("/") + 1);
            filename = filename.right(filename.length() - filename.lastIndexOf("/") - 1);
            display = filename + "  (" + path + ")";
        }

        QListWidgetItem* item = new QListWidgetItem(display);
        item->setData(FULLFILENAMEROLE, recent);
        list_->addItem(item);
    }
}

void StartupScreen::templateButtonClicked() {
    list_->clear();

    QList<QPair<QString, QString> > templates = TemplateWorkspaceFetcher::getTemplateWorkspaces();

    QListWidgetItem* newWorkspaceItem = new QListWidgetItem(tr("Empty Workspace"));
    newWorkspaceItem->setData(Qt::UserRole, 1);
    list_->addItem(newWorkspaceItem);

    for (int i = 0; i < templates.size(); ++i) {
        QString first = templates.at(i).first;
        QString second = templates.at(i).second;

        QListWidgetItem* item;
        if (second.isEmpty())
            item = new QListWidgetItem(first);
        else
            item = new QListWidgetItem(second);
        item->setData(FULLFILENAMEROLE, QString::fromStdString(VoreenApplication::app()->getWorkspacePath()) + "/templates/" + first);
        list_->addItem(item);

    }
}

void StartupScreen::tutorialButtonClicked() {
    list_->clear();

    QList<TutorialVideoFetcher::TutorialVideo> videos = videoFetcher_.getListOfVideos();

    if (videos.count() > 0) {
        foreach (TutorialVideoFetcher::TutorialVideo video, videos) {
            QListWidgetItem* item = new QListWidgetItem;
            item->setData(Qt::DisplayRole, video.title_);
            item->setData(DESCRIPTIONROLE, video.description_);
            item->setData(URLROLE, video.url_);
            list_->addItem(item);
        }
    }
    else {
        QListWidgetItem* firstItem = new QListWidgetItem("Could not establish YouTube connection.");
        firstItem->setFlags(Qt::NoItemFlags);
        list_->addItem(firstItem);
    }
}

void StartupScreen::helpButtonClicked() {
    list_->clear();

    QListWidgetItem* gettingStarted = new QListWidgetItem(tr("Getting Started Guide"));
    gettingStarted->setData(Qt::UserRole, GETTING_STARTED);
    list_->addItem(gettingStarted);

    QListWidgetItem* tutorialSlides = new QListWidgetItem(tr("Tutorial Slides"));
    tutorialSlides->setData(Qt::UserRole, TUTORIAL_SLIDES);
    list_->addItem(tutorialSlides);

    QListWidgetItem* animationManual = new QListWidgetItem(tr("Animation Manual"));
    animationManual->setData(Qt::UserRole, ANIMATION_MANUAL);
    list_->addItem(animationManual);
}

void StartupScreen::closeButtonClicked() {
    QSettings settings;
    settings.setValue("showStartupScreen", !dontShowAgain_->isChecked());
    close();
    emit closeSignal();
}

void StartupScreen::itemDoubleClicked(QListWidgetItem* item) {
    if (recentsButton_->isChecked()) {
        QString fileName = item->data(FULLFILENAMEROLE).toString();
        emit selectedRecentWorkspace(fileName);
        emit closeSignal();
        close();
    }
    else if (templatesButton_->isChecked()) {
        if (item->data(Qt::UserRole).toInt() == 1) {
            emit selectedNewWorkspace();
            emit closeSignal();
            close();
        }
        else {
            QString fileName = item->data(FULLFILENAMEROLE).toString();
            emit selectedTemplate(fileName);
            emit closeSignal();
            close();
        }
    }
    else if (tutorialButton_->isChecked()) {
        QString fileName = item->data(URLROLE).toString();
        QDesktopServices::openUrl(QUrl(fileName));
    }
    else { // help button
        int id = item->data(Qt::UserRole).toInt();
        switch (id) {
        case GETTING_STARTED:
            emit gettingStartedSignal();
            emit close();
            break;
        case TUTORIAL_SLIDES:
            emit helpTutorialSlides();
            break;
        case ANIMATION_MANUAL:
            emit helpAnimation();
            emit close();
            break;
        }
    }
}

QPoint StartupScreen::getPixmapPosition() {
    return mapToGlobal(pixmap_->pos());
}

} // namespace

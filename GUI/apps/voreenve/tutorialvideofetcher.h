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

#ifndef VRN_TUTORIALVIDEOFETCHER_H
#define VRN_TUTORIALVIDEOFETCHER_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>

class QNetworkReply;

namespace voreen {

class TutorialVideoFetcher : public QObject {
Q_OBJECT
public:
    struct TutorialVideo {
        QString title_;
        QString description_;
        QString url_;
        QString thumbnailUrl_;
        QDateTime updated_;

    };

    TutorialVideoFetcher();

    QList<TutorialVideo> getListOfVideos();

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    QList<TutorialVideo> videos_;
};


} // namespace

#endif // VRN_TUTORIALVIDEOFETCHER_H

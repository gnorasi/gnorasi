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

#include "tutorialvideofetcher.h"

#include <QDomDocument>
#include <QDomElement>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace voreen {

namespace {
    const QString& youtubeChannelUrl = "http://gdata.youtube.com/feeds/api/users/voreentv/uploads";

    bool videoLessThan(const TutorialVideoFetcher::TutorialVideo& v1, const TutorialVideoFetcher::TutorialVideo& v2) {
        return v1.title_ < v2.title_;
    }
};

TutorialVideoFetcher::TutorialVideoFetcher() {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(youtubeChannelUrl)));
}

QList<TutorialVideoFetcher::TutorialVideo> TutorialVideoFetcher::getListOfVideos() {
    return videos_;
}

void TutorialVideoFetcher::replyFinished(QNetworkReply* reply) {
    QString replyText = QString(reply->readAll());
    QDomDocument doc("document");
    QString errorMsg;
    int errorLine = 0;
    int errorColumn = 0;
    doc.setContent(replyText, false, &errorMsg, &errorLine, &errorColumn);   

    videos_.clear();

    QDomElement docElem = doc.documentElement();

    QDomNodeList children = docElem.childNodes();
    for (int i = 0; i < children.size(); ++i) {
        const QDomNode& child = children.at(i);
        if (child.nodeName() == "entry") {
            QDomElement childElem = child.toElement();

            TutorialVideo video;
            QDomElement updatedElem = childElem.firstChildElement("updated");
            QString updatedText = updatedElem.text();
            video.updated_ = QDateTime::fromString(updatedText, "yyyy-MM-ddTHH:mm:ss.zzzZ");

            QDomElement titleElem = childElem.firstChildElement("title");
            QString titleText = titleElem.text();
            video.title_ = titleText;

            QDomElement contentElem = childElem.firstChildElement("content");
            QString contentText = contentElem.text();
            video.description_ = contentText;

            QDomElement mediaGroup = childElem.firstChildElement("media:group");
            QDomElement urlElem = mediaGroup.firstChildElement("media:player");
            QString urlText = urlElem.attribute("url");
            video.url_ = urlText;

            QDomElement thumbnailElem = mediaGroup.firstChildElement("media:thumbnail");
            QString thumbnailText = thumbnailElem.attribute("url");
            video.thumbnailUrl_ = thumbnailText;

            videos_.append(video);
        }
    }
    qSort(videos_.begin(), videos_.end(), videoLessThan);
}

} // namespace

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

#include "templateworkspacefetcher.h"

#include "voreen/core/voreenapplication.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

namespace voreen {

namespace {
    QString tutorialPath = "/templates";
    QString orderingFile = "order.cfg";
    QString separatorBetweenFileNameAndName = ";";
}

QList<QPair<QString, QString> > TemplateWorkspaceFetcher::getTemplateWorkspaces() {
    VoreenApplication* app = VoreenApplication::app();
    QString workspacePath = QString::fromStdString(app->getWorkspacePath());
    workspacePath.append(tutorialPath);

    QDir workspaceDirectory(workspacePath);
    QStringList fullList = workspaceDirectory.entryList(QDir::Files);
    fullList.removeOne(orderingFile);

    QFile orderFile(workspaceDirectory.absolutePath() + "/" + orderingFile);
    orderFile.open(QIODevice::ReadOnly);
    QTextStream stream(&orderFile);

    QList<QPair<QString, QString> > orderList;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList splittedLine = line.split(separatorBetweenFileNameAndName);

        QString first = splittedLine.at(0);
        fullList.removeOne(first);

        QString second = "";
        if (splittedLine.size() > 1)
            second = splittedLine.at(1);

        orderList << QPair<QString, QString>(first, second);
    }

    orderFile.close();

    foreach (QString s, fullList)
        orderList << QPair<QString, QString>(s, "");

    return orderList;
}

} // namespace

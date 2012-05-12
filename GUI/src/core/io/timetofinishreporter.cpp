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

#include "voreen/core/io/timetofinishreporter.h"

#include "tgt/logmanager.h"

namespace voreen {

TimeToFinishReporter::TimeToFinishReporter(const std::string& className, bool printFinishedMessage)
    : className_(className)
    , printFinishedMessage_(printFinishedMessage)
    , messageInterval_(5)
    , tier_(0)
{
    setProgress(0.f);
}

void TimeToFinishReporter::setProgress(float progress) {
    if (progress == 0.f) {
        lastMessageTime_ = time_t(0);
        startTime_ = time_t(0);
    } else if (progress == 1.f) {
        if (printFinishedMessage_ && (startTime_ != time_t(0)))
            LINFOC(className_, "Time to finish:\tFinished!");
    }
    else {
        time_t current = time(NULL);
        if (current - lastMessageTime_ > messageInterval_) {
            if (startTime_ != time_t(0)) {
                float delta = static_cast<float>(current - startTime_);
                int secondsLeft = static_cast<int>((delta / progress) - delta);
                int minutesLeft = secondsLeft / 60;
                int hoursLeft = minutesLeft / 60;
                secondsLeft %= 60;
                minutesLeft %= 60;
                if (tier_ == 0)
                    LINFOC(className_, "Time to finish:\t\t" << hoursLeft << ":" << minutesLeft << ":" << secondsLeft);
                else
                    LINFOC(className_, "Time to finish (Tier " << tier_ << "):\t" << hoursLeft << ":" << minutesLeft << ":" << secondsLeft);

                if (hoursLeft > 5)
                    messageInterval_ = 30 * 60;
                else if (hoursLeft > 0)
                    messageInterval_ = 2 * 60;
                else if (minutesLeft > 0)
                    messageInterval_ = 25;
                else if (secondsLeft > 30)
                    messageInterval_ = 5;
                else
                    messageInterval_ = 2;
                lastMessageTime_ = current;
            }
            else {
                // first setProgress after 0.f
                startTime_ = current;
                return;
            }
        }
    }
}

void TimeToFinishReporter::setProgressTier(int tier, const std::string& tierName) {
    tgtAssert(tier >= 0, "tier must be positive");
    LINFOC(className_, "Time to finish:\t\t Starting tier " << tier << " (" << tierName << ")");
    tier_ = tier;
}

} // namespace

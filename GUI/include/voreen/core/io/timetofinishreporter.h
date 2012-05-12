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

#ifndef VRN_TIMETOFINISHREPORTER_H
#define VRN_TIMETOFINISHREPORTER_H

#include <string>
#include <time.h>

namespace voreen {

/**
 * This class calculates the time a given task needs to complete
 * and prints it via LINFOC where C is the passed className in
 * the constructor. See Processor for usage.
 * Note: The time between two messages will be computed dynamically
 * depending on the remaining time:
 * > 5 hours:    every 30 minutes
 * > 60 minutes: every 2 minutes
 * > 60 seconds: every 25 seconds
 * > 30 seconds: every 5 seconds
 * else:         every 2 seconds
 */
class TimeToFinishReporter {
public:
    TimeToFinishReporter(const std::string& className, bool printFinishedMessage = true);

     /**
     * Assigns the current progress state. Setting the progress
     * to 0 will reset the time. Setting the progress to 1 will
     * print a "Finished!" message.
     *
     * @param progress The current progress. Must lie in [0, 1]
     */
    void setProgress(float progress);

    /**
     * Sets the 'level' of progress. This is used if a processor
     * has multiple expensive tasks to do in a single process call
     * and wants to inform the user about the status of each.
     * Usage: Use \sa setProgress as normal for the first tier
     * after completion call setProgressTier(1) and start using
     * \sa setProgress again, ranging from 0.f to 1.f as normal.
     */
    void setProgressTier(int tier, const std::string& tierName = "");

private:
    std::string className_; ///< the category used in the output
    bool printFinishedMessage_; ///< print a "Finished!" message at the end?
    time_t messageInterval_; ///< the interval between two messages
    time_t startTime_; ///< the time when the computation started
    time_t lastMessageTime_; ///< the last time a message was outputted
    int tier_; ///< the progress tier we are currently in
};

} // namespace

#endif // VRN_TIMETOFINISHREPORTER_H

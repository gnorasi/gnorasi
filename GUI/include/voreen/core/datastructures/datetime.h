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

#ifndef VRN_DATETIME_H
#define VRN_DATETIME_H

#include "voreen/core/io/serialization/serializable.h"
#include "voreen/core/voreencoredefine.h"

#include <string>
#include <ostream>
#include <ctime>

namespace voreen {

/**
 * Stores a date value, a time value, or a combined date and time value.
 */
class VRN_CORE_API DateTime : public Serializable {

public:
    /// Specifies the data type stored in the DateTime object.
    enum Type {
        DATETIME, 
        DATE,
        TIME
    };
    
    /// Creates a DateTime object of the passed type and initializes it to all zero values.
    DateTime(Type = DATETIME);

    /// Constructs a DateTime object of type DATETIME and initializes it with the passed parameters.
    DateTime(int year, int month, int day, int hour, int min, int sec);

    /// Constructs a DateTime object of type DATETIME and initializes it from the passed timestamp.
    DateTime(time_t timestamp);

    /// Creates a DateTime object of type DATE and initializes it with the passed parameters.
    static DateTime createDate(int year, int month, int day);

    /// Creates a DateTime object of type TIME and initializes it with the passed parameters.
    static DateTime createTime(int hour, int minute, int second);

    friend std::ostream& operator<< (std::ostream& out, const DateTime &dateTime);
    std::string toString() const;

    /// Returns the type of the DateTime object.
    Type getType() const;
    
    void setDateTime(int year, int month, int day, int hour, int min, int sec);
    void setDate(int year, int month, int day);
    void setTime(int hour, int min, int sec);

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;
    int getSecond() const;

    void setTimestamp(time_t timestamp);
    time_t getTimestamp() const;

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

private:
    Type type_;

    int year_;
    int month_;
    int day_;
    int hour_;
    int minute_;
    int second_;

    static const std::string loggerCat_;
};

} // namespace

#endif

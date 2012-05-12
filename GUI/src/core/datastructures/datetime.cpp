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

#include "voreen/core/datastructures/datetime.h"

#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"

#include <sstream>
#include <cstdio>

namespace voreen {

const std::string DateTime::loggerCat_ = "voreen.DateTime";

DateTime::DateTime(Type type /*= DATETIME*/) 
    : type_(type)
{
    setDateTime(0, 0, 0, 0, 0, 0);
}

DateTime::DateTime(int year, int month, int day, int hour, int min, int sec)
    : type_(DATETIME)
{
    setDateTime(year, month, day, hour, min, sec);
}

DateTime::DateTime(time_t timestamp) 
    : type_(DATETIME)
{
    setTimestamp(timestamp);
}

DateTime DateTime::createDate(int year, int month, int day) {
    DateTime dateTime(DATE);
    dateTime.setDate(year, month, day);
    return dateTime;
}

DateTime DateTime::createTime(int hour, int minute, int second) {
    DateTime dateTime(TIME);
    dateTime.setTime(hour, minute, second);
    return dateTime;
}

std::ostream& operator<<(std::ostream& os, const DateTime& dateTime) {
    return os << dateTime.toString();
}

std::string DateTime::toString() const {
    char* charString = new char[30];
    if (type_ == DATETIME)
        sprintf(charString, "%04d-%02d-%02d %02d:%02d:%02d", 
            getYear(), getMonth(), getDay(), getHour(), getMinute(), getSecond());
    else if (type_ == DATE) 
        sprintf(charString, "%04d-%02d-%02d", getYear(), getMonth(), getDay());
    else if (type_ == TIME)
        sprintf(charString, "%02d:%02d:%02d", getHour(), getMinute(), getSecond());
    else {
        LERRORC("voreen.DateTime", "invalid type");
    }

    std::string result(charString);
    delete[] charString;
    return result;
}

DateTime::Type DateTime::getType() const {
    return type_;
}

void DateTime::setDateTime(int year, int month, int day, int hour, int min, int sec) {
    year_ = year;
    month_ = month;
    day_ = day;
    hour_ = hour;
    minute_ = min;
    second_ = sec;
}

void DateTime::setDate(int year, int month, int day) {
    if (type_ == TIME) {
        LWARNINGC("voreen.DateTime", "setDate() used on instance of type TIME");
    }
    year_ = year;
    month_ = month;
    day_ = day;
}

void DateTime::setTime(int hour, int min, int sec) {
    if (type_ == DATE) {
        LWARNINGC("voreen.DateTime", "setDate() used on instance of type DATE");
    }
    hour_ = hour;
    minute_ = min;
    second_ = sec;
}

int DateTime::getYear() const {
    return year_;
}

int DateTime::getMonth() const  {
    return month_;
}

int DateTime::getDay() const {
    return day_;
}

int DateTime::getHour() const {
    return hour_;
}

int DateTime::getMinute() const {
    return minute_;
}

int DateTime::getSecond() const {
    return second_;
}

void DateTime::setTimestamp(time_t timestamp) {
    tm* t = gmtime(&timestamp);
    setDateTime(
        t->tm_year + 1900, 
        t->tm_mon + 1, 
        t->tm_mday, 
        t->tm_hour, 
        t->tm_min, 
        t->tm_sec);
}

time_t DateTime::getTimestamp() const {
    tm dateTime;
    dateTime.tm_year = year_ - 1900;
    dateTime.tm_mon = month_ - 1;
    dateTime.tm_mday = day_;
    dateTime.tm_hour = hour_;
    dateTime.tm_min = minute_;
    dateTime.tm_sec = second_;

    return mktime(&dateTime);
}

void DateTime::serialize(XmlSerializer& s) const {
    s.serialize("type", type_);

    s.serialize("year", year_);
    s.serialize("month", month_);
    s.serialize("day", day_);
    s.serialize("hour", hour_);
    s.serialize("minute", minute_);
    s.serialize("second", second_);
}

void DateTime::deserialize(XmlDeserializer& s) {
    int typeInt;
    s.optionalDeserialize<int>("type", typeInt, DATETIME);
    type_ = Type(typeInt);

    s.deserialize("year", year_);
    s.deserialize("month", month_);
    s.deserialize("day", day_);
    s.deserialize("hour", hour_);
    s.deserialize("minute", minute_);
    s.deserialize("second", second_);
}

} // namespace

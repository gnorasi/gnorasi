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

#ifndef VRN_STRINGUTILS_H
#define VRN_STRINGUTILS_H

#include "voreen/core/voreencoredefine.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace voreen {

/// Converts an int to a string using stringstreams
VRN_CORE_API std::string itos(int i);

/// Converts an size_t to a string using stringstreams
VRN_CORE_API std::string itos(size_t i);

/// Converts a float to a string using stringstreams
VRN_CORE_API std::string ftos(float f);

/// Converts a double to a string using stringstreams
VRN_CORE_API std::string dtos(double d);

/// MSVC++ 2010 and later already provides the sto* functions in the <string> header
#if !defined(_MSC_VER) || (_MSC_VER < 1600) 
/// Converts a string to an int using stringstreams
VRN_CORE_API int stoi(const std::string& s);

/// Converts a string to a float using stringstreams
VRN_CORE_API float stof(const std::string& s);

/// Converts a string to a double using stringstreams
VRN_CORE_API double stod(const std::string& s);
#endif

/**
 * Converts the string to a null-terminated char array with length s.size()+1.
 * Deleting the allocated memory is up to the caller.
 */
VRN_CORE_API char* strtochr(const std::string& s);

/**
 * Returns a copy of \p str where all occurrences of \p from have been replaced by \p to.
 */
VRN_CORE_API std::string strReplaceAll(const std::string& str, const std::string& from, const std::string& to);

/** 
 * @brief Remove whitespaces from beginning and end of a string.
 * 
 * @param str Input string.
 * @param charlist Characters to remove, defaults to space, tab, newline, carriage return, 0, vertical tab.
 */
VRN_CORE_API std::string trim(std::string str, const std::string& charlist = " \t\n\r\0\x0B");

/**
 * Splits a string by the specified delimiter and returns the items in a vector.
 */
VRN_CORE_API std::vector<std::string> strSplit(const std::string& str, char delim);

/**
 * Joins a sequence of tokens to a string. The converted tokens
 * are separated by the specified delimiter.
 */
template<typename T>
std::string strJoin(const std::vector<T>& tokens, const std::string& delim) {
    if (tokens.empty())
        return "";
    std::stringstream s;
    s << tokens.at(0);
    for (size_t i=1; i<tokens.size(); i++)
        s << delim << tokens.at(i);
    return s.str();
}

} // namespace

#endif // VRN_STRINGUTILS_H

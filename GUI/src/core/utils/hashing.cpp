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

#include <stdio.h>

#include "voreen/core/utils/hashing.h"
#include "md5/md5.c"

namespace voreen {

std::string VoreenHash::getHash(const void* data, size_t size) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, const_cast<void*>(data), static_cast<unsigned long>(size));
    unsigned char result[16];
    MD5_Final(result, &ctx);

    unsigned char output[2 * 16 + 1];
    for (int i=0; i<16; i++) {
        sprintf((char *) output + (2 * i), "%02x", result[i]);
    }
    output[2 * 16 ] = '\0';

    return std::string((const char*)output);
}

std::string VoreenHash::getHash(const std::string& s) {
    return getHash(s.c_str(), s.length());
}

} // namespace

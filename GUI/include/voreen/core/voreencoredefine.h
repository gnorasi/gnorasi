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

#ifndef VRN_VOREENCOREDEFINE_H
#define VRN_VOREENCOREDEFINE_H

#ifdef VRN_DYNAMIC_LIBS
    #ifdef VRN_CORE_BUILD_DLL
        #ifdef WIN32
            #define VRN_CORE_API __declspec(dllexport)
        #else //WIN32 - so it is UNIX -> I assume gcc
            #define VRN_CORE_API __attribute__ ((visibility ("default")))
        #endif //WIN32
    #else //VRN_CORE_BUILD_DLL
        #ifdef WIN32
            #define VRN_CORE_API __declspec(dllimport)
        #else //WIN32 - so it is UNIX -> I assume gcc
            #define VRN_CORE_API
        #endif //WIN32
    #endif //VRN_CORE_API
#else
    #define VRN_CORE_API // building static library -> do nothing
#endif

#endif // VRN_VOREENCOREDEFINE_H

/**********************************************************************
 *                                                                    *
 * tgt - Tiny Graphics Toolbox                                        *
 *                                                                    *
 * Copyright (C) 2006-2011 Visualization and Computer Graphics Group, *
 * Department of Computer Science, University of Muenster, Germany.   *
 * <http://viscg.uni-muenster.de>                                     *
 *                                                                    *
 * This file is part of the tgt library. This library is free         *
 * software; you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License version 2.1 as published  *
 * by the Free Software Foundation.                                   *
 *                                                                    *
 * This library is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU Lesser General Public License for more details.                *
 *                                                                    *
 * You should have received a copy of the GNU Lesser General Public   *
 * License in the file "LICENSE.txt" along with this library.         *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 **********************************************************************/

#ifndef TGT_SINGLETON_H
#define TGT_SINGLETON_H

#include <iostream>

#include "tgt/assert.h"
#include "tgt/types.h"

#define SINGLETON_CLASS_HEADER(T) \
public:\
static void init() {\
tgtAssert(!singletonClass_, "singletonClass_ has already been initialized." );\
singletonClass_ = new T;\
tgtAssert(singletonClass_, "singletonClass_ has not been created.");\
}\
\
static void deinit() {\
tgtAssert(singletonClass_, "singletonClass_ has already been deinitialized." );\
delete singletonClass_;\
singletonClass_ = 0;\
}\
\
static T* getPtr() {\
tgtAssert(singletonClass_, "singletonClass_ has not been intitialized." );\
return singletonClass_;\
}\
\
static T& getRef() {\
tgtAssert(singletonClass_ , "singletonClass_ has not been intitialized." );\
return *singletonClass_;\
}\
\
static bool isInited() {\
return (singletonClass_ != 0);\
}\
protected:\
static T* singletonClass_;\

#define SINGLETON_CLASS_SOURCE(T) \
T* T::singletonClass_ = 0;



#endif // TGT_SINGLETON_H

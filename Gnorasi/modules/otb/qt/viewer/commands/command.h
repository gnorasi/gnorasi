/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

namespace itiviewer{

/*!
 * \brief The Command class
 *
 * \ingroup Visualization
 *
 *  This class is an interface class
 *  Whenever a new functionality is needed to be  implemented
 *  a new subclass of this class should be created.
 *  the new subclass should have a pointer to the receiver object
 *  the reveiver object should have the actual action needs to be implemented by the subclass command.
 */
class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = 0);

    //! every subclass must implement the execute function
    virtual void execute() = 0;

    
};

}

#endif // COMMAND_H

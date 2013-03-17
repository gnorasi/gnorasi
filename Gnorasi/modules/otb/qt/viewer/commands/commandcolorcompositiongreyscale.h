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

#ifndef COMMANDCOLORCOMPOSITIONGREYSCALE_H
#define COMMANDCOLORCOMPOSITIONGREYSCALE_H

#include <QtCore>
#include <QtGui>

#include "command.h"

#include "../vector_globaldefs.h"

using namespace otb;

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandColorCompositionGreyscale class
 *
 *  This class is responsible for setting a greyscale fix to the viewer.
 *  All it does is to set the preferred channel to the viewer and reset the model data,
 *  in order to reflect changes to the viewer.
 *
 */
class CommandColorCompositionGreyscale : public Command
{
    Q_OBJECT
    Q_PROPERTY(int channel  READ channel    WRITE setChannel    NOTIFY channelChanged)

public:

    explicit                CommandColorCompositionGreyscale(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);

    int                     channel() const {return m_channel; }
    void                    setChannel(int c) { m_channel = c; }

    /*!
     * \brief execute
     */
    void                    execute();

signals:
    void                    channelChanged();

public slots:
    void                    updateChannel(int c) { setChannel(c); }

private:
    int                     m_channel;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCOLORCOMPOSITIONGREYSCALE_H

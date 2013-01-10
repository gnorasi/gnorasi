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

#ifndef COMMANDCOLORCOMPOSITIONRGB_H
#define COMMANDCOLORCOMPOSITIONRGB_H

#include <QtCore>
#include <QtGui>

#include "command.h"

#include "../vector_globaldefs.h"

using namespace otb;

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandColorCompositionRGB class
 *
 */
class CommandColorCompositionRGB : public Command
{
    Q_OBJECT
    Q_PROPERTY(int red      READ red    WRITE setRed    NOTIFY redChanged)
    Q_PROPERTY(int green    READ green  WRITE setGreen  NOTIFY greenChanged)
    Q_PROPERTY(int blue     READ blue   WRITE setBlue   NOTIFY blueChanged)

public:

    explicit                CommandColorCompositionRGB(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);

    int                     red() const {return m_red; }
    void                    setRed(int c) { m_red = c; }

    int                     green() const {return m_green; }
    void                    setGreen(int c) { m_green = c; }

    int                     blue() const {return m_blue; }
    void                    setBlue(int c) { m_blue = c; }

    /*!
     * \brief execute
     */
    void                    execute();

signals:
    void                    redChanged();
    void                    greenChanged();
    void                    blueChanged();

public slots:
    void                    updateChannels(int r, int g, int b) { setRed(r);  setGreen(g); setBlue(b); }

private:
    int                     m_red;
    int                     m_green;
    int                     m_blue;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCOLORCOMPOSITIONRGB_H

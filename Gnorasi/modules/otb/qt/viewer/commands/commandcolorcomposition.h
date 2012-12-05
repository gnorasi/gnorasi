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

#ifndef COMMANDCOLORCOMPOSITION_H
#define COMMANDCOLORCOMPOSITION_H

#include "command.h"

namespace itiviewer{

class ItiOtbImageViewer;

/*!
 * \brief The CommandColorComposition class
 *  This command handles color composition functionality
 */
class CommandColorComposition : public Command
{
    Q_OBJECT
public:

    //! ctor
    CommandColorComposition(ItiOtbImageViewer *, QObject *parent);

    //! implementation
    void execute();

public slots:
    void setGreyScaleMethod(int channel);

    void setRGBMethod(int red, int green, int blue);

private:
    ItiOtbImageViewer *m_pItiOtbImageViewer;

    //! a color composition could be either greyscale or RGB
    bool m_isGreyScale;

    //! channel numbers
    int m_greyChannel;
    int m_redChannel;
    int m_greenChannel;
    int m_blueChannel;
};

} // namespace itiviewer

#endif // COMMANDCOLORCOMPOSITION_H

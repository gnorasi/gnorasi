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

    /*!
     * \brief execute
     *  implementation
     */
    void execute();

public slots:
    /*!
     * \brief setGreyScaleMethod
     *  set the method to greyscale
     * \param channel , the channel number to be used for the color composition
     */
    void setGreyScaleMethod(int channel);

    /*!
     * \brief setRGBMethod , sets the method to RGB
     * \param red , the red channel to be used for the color composition
     * \param green, the green channel to be used for the color composition
     * \param blue, the blue channel to be used for the color composition
     */
    void setRGBMethod(int red, int green, int blue);

private:
    /*!
     * \brief m_pItiOtbImageViewer , the viewer instance
     */
    ItiOtbImageViewer *m_pItiOtbImageViewer;

    /*!
     * \brief m_isGreyScale , a color composition could be either greyscale or RGB
     */
    bool m_isGreyScale;

    /*!
     * \brief m_greyChannel , the grey channel to be used
     */
    int m_greyChannel;

    /*!
     * \brief m_redChannel, the red channel to be used
     */
    int m_redChannel;

    /*!
     * \brief m_greenChannel, the green channel to be used
     */
    int m_greenChannel;

    /*!
     * \brief m_blueChannel, the blue channel to be used
     */
    int m_blueChannel;
};

} // namespace itiviewer

#endif // COMMANDCOLORCOMPOSITION_H

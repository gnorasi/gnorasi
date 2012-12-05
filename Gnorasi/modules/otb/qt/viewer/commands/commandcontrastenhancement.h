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

#ifndef COMMANDCONTRASTENHANCEMENT_H
#define COMMANDCONTRASTENHANCEMENT_H

#include "command.h"

namespace itiviewer{

class ItiOtbImageViewer;

/*!
 * \brief The CommandContrastEnhancement class
 *  This command handles contrast enhancement functionality
 */
class CommandContrastEnhancement : public Command
{
    Q_OBJECT

public:

    enum METHOD{
        METHOD_LINEAR_0_255 = 0,
        METHOD_LINERA_PERC  = 1,
        METHOD_GAUSSIAN     = 2,
        METHOD_SQUARE_ROOT  = 3
    };

    //! ctor
    CommandContrastEnhancement(ItiOtbImageViewer *, QObject *parent);

    //! implementation
    void execute();

public slots:
    //!
    void setContrastEnhancementMethod(int method, double aval, double bval);

private:
    //!
    ItiOtbImageViewer *m_pItiOtbImageViewer;

    //! contrast enhancement variable
    METHOD m_method;

    //! parameters
    double m_standardDeviation;
    double m_upperQuantile;
    double m_lowerQuantile;
};

} //end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENT_H

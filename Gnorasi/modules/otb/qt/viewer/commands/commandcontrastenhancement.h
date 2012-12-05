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

    /*!
     * \brief CommandContrastEnhancement
     * \param parent
     */
    CommandContrastEnhancement(ItiOtbImageViewer *, QObject *parent);

    /*!
     * \brief execute , implementation
     */
    void execute();

public slots:
    /*!
     * \brief setContrastEnhancementMethod , sets the method and the arguments of the contrast enhancement
     * \param method , the method of contrast enhancement
     * \param aval , equals to the standard deviation if the method equals to the GAUSSIAN method or the lower quantile value
     * \param bval, equal to the upper quantile value or could be -1.0 if the GAUSSIAN method is set
     */
    void setContrastEnhancementMethod(int method, double aval, double bval = -1.0);

private:
    /*!
     * \brief m_pItiOtbImageViewer
     */
    ItiOtbImageViewer *m_pItiOtbImageViewer;

    /*!
     * \brief m_method , variable for holding the contrast enhancement value
     */
    METHOD m_method;

    /*!
     * \brief m_standardDeviation , a variable for holding the standard deviation value
     */
    double m_standardDeviation;

    /*!
     * \brief m_upperQuantile, a variable for holding the upper quantile value
     */
    double m_upperQuantile;

    /*!
     * \brief m_upperQuantile, a variable for holding the lower quantile value
     */
    double m_lowerQuantile;
};

} //end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENT_H

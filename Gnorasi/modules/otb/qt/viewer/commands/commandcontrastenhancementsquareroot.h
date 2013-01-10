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

#ifndef COMMANDCONTRASTENHANCEMENTSQUAREROOT_H
#define COMMANDCONTRASTENHANCEMENTSQUAREROOT_H

#include <QtCore>
#include <QtGui>

#include "command.h"

#include "../vector_globaldefs.h"

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandContrastEnhancementGaussian class
 *  this class encapsulates the itkGaussianOperator functionality ,
 *  plz see http://www.orfeo-toolbox.org/SoftwareGuide/SoftwareGuidech26.html#x54-36200026.4.1
 */
class CommandContrastEnhancementSquareRoot : public Command
{
    Q_OBJECT
    Q_PROPERTY(double upperQuantile READ upperQuantile WRITE setUpperQuantile   NOTIFY upperQuantileChanged)
    Q_PROPERTY(double lowerQuantile READ lowerQuantile WRITE setLowerQuantile   NOTIFY lowerQuantileChanged)

public:

    explicit                CommandContrastEnhancementSquareRoot(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);
    
    double                  upperQuantile() const           { return m_upperQuantile; }
    void                    setUpperQuantile(double v)      { m_upperQuantile = v; }

    double                  lowerQuantile() const           { return m_lowerQuantile; }
    void                    setLowerQuantile(double v)      { m_lowerQuantile = v; }

    /*!
     * \brief execute
     */
    void                    execute();

signals:
    void                    upperQuantileChanged();

    void                    lowerQuantileChanged();

public slots:
    void                    updateUpperQuantile(double v) { setUpperQuantile(v); }

    void                    updateLowerQuantile(double v) { setLowerQuantile(v); }


private:
    /*!
     * \brief m_upperQuantile, a variable for holding the upper quantile value
     */
    double m_upperQuantile;

    /*!
     * \brief m_upperQuantile, a variable for holding the lower quantile value
     */
    double m_lowerQuantile;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENTSQUAREROOT_H

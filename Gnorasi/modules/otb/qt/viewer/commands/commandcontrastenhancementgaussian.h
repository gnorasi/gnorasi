#ifndef COMMANDCONTRASTENHANCEMENTGAUSSIAN_H
#define COMMANDCONTRASTENHANCEMENTGAUSSIAN_H

#include <QtCore>
#include <QtGui>

#include "command.h"

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

#include "otbGaussianRenderingFunction.h"

#include "../vector_globaldefs.h"

using namespace otb;

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandContrastEnhancementGaussian class
 *  this class encapsulates the itkGaussianOperator functionality ,
 *  plz see http://www.orfeo-toolbox.org/SoftwareGuide/SoftwareGuidech26.html#x54-36200026.4.1
 */
class CommandContrastEnhancementGaussian : public Command
{
    Q_OBJECT
    Q_PROPERTY(double deviation READ deviation WRITE setDeviation   NOTIFY deviationChanged)

public:

    typedef Function::GaussianRenderingFunction<VPixelType,
                                                RGBAPixelType> GaussianRenderingFunctionType;

    explicit                CommandContrastEnhancementGaussian(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);
    
    double                  deviation() const           { return m_deviation; }
    void                    setDeviation(double dev)    { m_deviation = dev; }

    /*!
     * \brief execute
     */
    void                    execute();

signals:
    void                    deviationChanged();

public slots:
    void                    updateDeviation(double v) { setDeviation(v); }

private:
    double                  m_deviation;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENTGAUSSIAN_H

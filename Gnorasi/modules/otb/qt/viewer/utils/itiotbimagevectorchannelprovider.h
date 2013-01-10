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

#ifndef ITIOTBIMAGEVECTORCHANNELPROVIDER_H
#define ITIOTBIMAGEVECTORCHANNELPROVIDER_H

#include "itiotbimagechannelprovider.h"

#include "../vector_globaldefs.h"

#include "../models/itiotbVectorImageModel.h"

namespace itiviewer{

class ItiOtbImageVectorChannelProvider : public ItiOtbImageChannelProvider
{
    Q_OBJECT
public:
    explicit ItiOtbImageVectorChannelProvider(VectorImageModel *m, QObject *parent = 0);
    
    /*!
     * \brief parseImage, implementation
     */
    void parse();

signals:
    
public slots:
    /*!
     * \brief onModelJChanged
     */
    void onModelJChanged();

private:
    VectorImageModel *m_pVectorImageModel;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVECTORCHANNELPROVIDER_H

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

#ifndef ITIOTBIMAGEMANAGER_H
#define ITIOTBIMAGEMANAGER_H

#include <QObject>

#include "voreen/core/ports/port.h"


namespace itiviewer{

#define ITIOTBIMAGEMANAGER ItiOtbImageManager::instance()

class Command;

/*!
 *
 *  This class is created in order to take over the functionality
 *  previously handled by the ImageView class declared in the
 *  otbImageView header file contained in the OTB library.
 *
 *  This is a singleton class in order the unique instance of this class to be
 *  used as an interface - console item , accessible from everywhere in
 *  the project.
 *
 *  This class has a voreen Port as member variable, this port holds data related to the image
 *  The concrete ItiOtbImageViewer class knows which kind of port is used a raster or vector port
 *
 */
class ItiOtbImageManager
{
public:
    //!
    static ItiOtbImageManager* instance();

    //! This function should be called at the destructor of the top level class
    static void deleteInstance();

    /** Set/Get the image to render */
    voreen::Port* port() const { return m_pPort; }
    void setPort(voreen::Port* p) { m_pPort = p; }

    /** Set/Get the DEM directory */
    void setDemDirectory(const QString &d) { m_DEMDirectory = d; }
    QString demDirectory() const { return m_DEMDirectory; }

    /** Set/Get the DEM directory */
    void setGeoidFile(const QString &fp) { m_GeoidFile = fp; }
    QString geoidFile() const { return m_GeoidFile; }

private:
    //! ctor
    ItiOtbImageManager();

    //!dtor
    ~ItiOtbImageManager();

    //! this is the unique instance
    static ItiOtbImageManager* m_pInstance;

    /*! Pointer to the image , the port holds image related data.*/
    voreen::Port *m_pPort;

    /*! Path to the DEMDirectory (used if a VectorData is rendered */
    QString m_DEMDirectory;

    /*! GeoidFile filename (used if a VectorData is rendered */
    QString m_GeoidFile;
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEMANAGER_H

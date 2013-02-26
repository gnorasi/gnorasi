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
#include "voreen/core/processors/processor.h"

#include "../vector_globaldefs.h"


namespace itiviewer{

/*!
 *  A helper MACRO
 */
#define ITIOTBIMAGEMANAGER ItiOtbImageManager::instance()

class Command;
class Level;
class VectorImageModel;

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
 *  This class has a vetore image as member variable,
 *  The concrete ItiOtbImageViewer class handles vector image data
 *
 */
class ItiOtbImageManager : public QObject
{
    Q_OBJECT
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


    ///! getter setters
    VectorImageType *image() const { return m_pImgType; }
    void setImage(VectorImageType *i) { m_pImgType = i; }

    /*!
     * \brief setupImage, setup the image , cast the image to vector
     *  This function should be called after setting the port to the manager , namely call the setPort function
     */
    void setupImage();

    /*!
     * \brief imageFile
     * \return
     */
    QString imageFile(voreen::Port *port = 0) ;

    /*!
     * \brief getNextPort
     * \return
     */
    voreen::Port* nextPort(voreen::Port* ) const;


    /*!
     * \brief clearLevels
     */
    void clearLevels();

    /*!
     * \brief appendLevel
     * \param pLevel
     */
    void appendLevel(Level *pLevel);


    /*!
     * \brief deleteLevel
     * \param pLevel
     */
    void deleteLevel(Level *pLevel);


    /*!
     * \brief levelById
     * \param id
     * \return
     */
    Level* levelById(int id);

    /*!
     * \brief levels
     * \return
     */
    QList<Level*> levels() const { return m_levelList; }

    /*!
     * \brief classficationNamesIds
     * \return
     */
    QHash<int ,QString> classficationNamesIds() const {return m_classficationNamesIds; }

    /*!
     * \brief classificationColorsIds
     * \return
     */
    QHash<int,QColor> classificationColorsIds() const { return m_colorHash ; }

    /*!
     * \brief constructInfoByIndex
     * \param idx, the index for which a text is being constructed
     * \param pixelInfo , a string holding pixel information
     * \return
     */
    QString constructInfoByIndex(ImageRegionType::IndexType idx, const QString &pixelInfo);

    /*!
     * \brief constructInfoByIndexAlt
     * \param idx
     * \return
     */
    QString constructInfoByIndexAlt(ImageRegionType::IndexType idx);

    /*!
     * \brief isInsideTheImage
     * \param region checked whether the given point is inside
     * \param point to check
     * \param z, the zoom level
     * \return
     */
    static inline bool isInsideTheImage(const ImageRegionType region, const QPoint &point, double z = 1.);

private:
    /*!
     * \brief ItiOtbImageManager
     *
     *  The constructor function
     */
    ItiOtbImageManager();

    /*!
     *  \brief ~ItiOtbImageManager
     *
     *  The destructor function
     */
    ~ItiOtbImageManager();

    /*!
     * \brief createRegions
     */
    void createRegions();

    //! this is the unique instance
    static ItiOtbImageManager* m_pInstance;

    //! this is the image
    VectorImageType *m_pImgType;

    /*! Pointer to the image , the port holds image related data.*/
    voreen::Port *m_pPort;

    /*! Path to the DEMDirectory (used if a VectorData is rendered */
    QString m_DEMDirectory;

    /*! GeoidFile filename (used if a VectorData is rendered */
    QString m_GeoidFile;

    /*!
     * \brief filter , an raster to vector image cast filter
     */
    ImageToVectorImageCastFilterType::Pointer filter;

    //! helper class
    std::vector<voreen::Processor*> nextConnectedProcessor(voreen::Port* ) const;

    //! helper class
    bool isReader(voreen::Processor* ) const;

    //! helper class
    QString getPathFromReaderProcessor(voreen::Processor*) const ;

    //! helper class
    void checkNext(voreen::Processor*, QString& );

    /*!
     * \brief m_levelList
     */
    QList<Level*> m_levelList;

    //!
    void setupColors();

    //! a container where the key value is the class if from the segmentation
    QHash<int, QColor> m_colorHash;

    //! a container holding the clasficaiton ids and a string values for names
    QHash<int ,QString> m_classficationNamesIds;
};


bool ItiOtbImageManager::isInsideTheImage(const ImageRegionType extent, const QPoint &point, double z){

    if(point.x() < 0 || point.y() < 0)
        return false;

    if(point.x() > ( extent.GetSize()[0]-1 ) / z || point.y() > (extent.GetSize()[1]-1) / z)
        return false;

    return true;
}

} // end of namespace itiviewer

#endif // ITIOTBIMAGEMANAGER_H

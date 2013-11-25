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
//#include "../../histogram/histogramgenerator.h"


class HistogramGenerator;
class MyHistogramList;

namespace itiviewer{


class Command;
class Level;
class VectorImageModel;

/*!
 *
 *  This class is created in order to take over the functionality
 *  previously handled by the ImageView class declared in the
 *  otbImageView header file contained in the OTB library.
 *
 *
 *  This class has a vetor image member variable,
 *  The concrete ItiOtbImageViewer class handles vector image data.
 *
 *  Each concrete viewer instance has an instance of this class as a member variable.
 *  The Image Model also has a member variable of this classes instance.
 *
 */
class ItiOtbImageManager : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief ItiOtbImageManager
     *
     *  The constructor function
     */
    ItiOtbImageManager(QObject *parent = 0);

    /*!
     *  \brief ~ItiOtbImageManager
     *
     *  The destructor function
     */
    ~ItiOtbImageManager();

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
     *  Parse the port chain in order to fetch the image file path of the original image which is being loaded by a reader processor.
     * \param port, the port given to which parsing of all ports is done
     * \return the path of the original image.
     */
    QString imageFile(voreen::Port *port = 0) ;

    /*!
     * \brief getNextPort
     *  Helper function, it gets the next incoming port given a port member variable
     * \return
     */
    voreen::Port* nextPort(voreen::Port* ) const;


    /*!
     * \brief clearLevels
     *  Clear all level objects. This function is used on the classification process.
     */
    void clearLevels();

    /*!
     * \brief appendLevel
     *  This function is used on the classification process.
     * \param pLevel
     */
    void appendLevel(Level *pLevel);


    /*!
     * \brief deleteLevel
     *  This function is used on the classification process.
     * \param pLevel
     */
    void deleteLevel(Level *pLevel);


    /*!
     * \brief levelById
     *  This function is used on the classification process.
     * \param id
     * \return
     */
    Level* levelById(int id);

    /*!
     * \brief levels, getter
     *  This function is used on the classification process.
     * \return
     */
    QList<Level*> levels() const { return m_levelList; }

    /*!
     * \brief classficationNamesIds, getter.
     *  This function is used on the classification process.
     * \return
     */
    QHash<int ,QString> classficationNamesIds() const {return m_classficationNamesIds; }

    /*!
     * \brief classificationColorsIds, getter.
     *  This function is used on the classification process.
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

    /*!
     * \brief isPortEmpty,
     *  Checks whether this port has not an empty image.
     * \param port
     * \return
     */
    bool isPortEmpty(voreen::Port *port);

    /*!
     * \brief histogramGenerator
     * \return
     */
    MyHistogramList* histogramList() { return m_pMyHistogramList; }

    /*!
     * \brief setHistogramGenerator
     * \param hg
     */
//    void setHistogramGenerator(HistogramGenerator *hg) { m_pHistogramGenerator = hg; }


    bool isHistogramReady() const { return m_isHistogramReady; }
    void setHistogramReady(bool b) { m_isHistogramReady = b; ; emit histogramChanged(m_isHistogramReady); }



signals:
    void histogramFinished(MyHistogramList* );

    void histogramChanged(bool );

public slots:
    void onHistogramFinished(MyHistogramList*);

private:

    VectorDataPointer getVectorDataType();


    /*!
     * \brief createRegions
     */
    void createRegions();

    /*!
     * \brief m_pImgType, this is the image
     */
    VectorImageType *m_pImgType;

    /*!
     * \brief m_pPort, Pointer to the image , the port holds image related data.
     */
    voreen::Port *m_pPort;

    /*!
     * \brief m_DEMDirectory, Path to the DEMDirectory (used if a VectorData is rendered
     */
    QString m_DEMDirectory;

    /*!
     * \brief m_GeoidFile, GeoidFile filename (used if a VectorData is rendered
     */
    QString m_GeoidFile;

    /*!
     * \brief filter , an raster to vector image cast filter
     */
    ImageToVectorImageCastFilterType::Pointer filter;

    /*!
     * \brief nextConnectedProcessor
     * \return the next connected processor given a specific port.
     */
    std::vector<voreen::Processor*> nextConnectedProcessor(voreen::Port* ) const;

    /*!
     * \brief isReader, checks whether a specific processor is of type reader
     * \return
     */
    bool isReader(voreen::Processor* ) const;

    /*!
     * \brief getPathFromReaderProcessor
     *  helper class, gets the path from a given reader processor
     * \return
     */
    QString getPathFromReaderProcessor(voreen::Processor*) const ;

    /*!
     * \brief checkNext
     *  helper class, check the next processor given a specific processor
     * \param proc, the processor which is used as the origin processor during the iterating process
     * \param path, the path of the file, this is a reference to a QString , once the reader procesor is found its port data is passed to it.
     */
    void checkNext(voreen::Processor *proc, QString &path);

    /*!
     * \brief m_levelList
     */
    QList<Level*> m_levelList;

    /*!
     * \brief setupColors
     *  Setup the colors of all levels, randomly.
     *  This function is used during the classification process.
     */
    void setupColors();

    /*!
     * \brief m_colorHash
     *  a container where the key value is the class if from the segmentation
     */
    QHash<int, QColor> m_colorHash;

    /*!
     * \brief m_classficationNamesIds
     *
     *  a container holding the clasficaiton ids and a string values for names
     */
    QHash<int ,QString> m_classficationNamesIds;

    /*!
     * \brief m_pHistogramGenerator
     */
    MyHistogramList*                   m_pMyHistogramList;


    bool m_isHistogramReady;
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

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

#ifndef VRN_QGLOTBIMAGEVIEWERWIDGET_H
#define VRN_QGLOTBIMAGEVIEWERWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "modules/otb/processors/Visualization/otbimageviewerprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"


namespace itiviewer{
    class ItiOtbImageViewer;
    class ItiOtbImageViewerFactory;
    class ItiOtbImageViewerPanel;
    class ItiOtbImageChannelProvider;
}

using namespace otb;

namespace voreen {

/*!
 * \brief The QGLOtbImageViewerWidget class
 *
 *  This is the main widget of the GeospatialClassificationProcessor
 *  This widget accepts data from the processors which are connected with the
 *  OtbImageViewerProcessor. It has two key member variables : ItiOtbImageViewer and ItiOtbImageViewerFactory
 *  The ItiOtbImageViewer is the GUI class which hadles all the visualization stuff.
 *  The ItiOtbImageViewerFactory is the core class which is responsible for creating the
 *  ItiOtbImageViewer instances. Depending on the port a new ItiOtbImageViewerFactory instance
 *  is created.
 *
 */
class VRN_QT_API QGLOtbImageViewerWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    QGLOtbImageViewerWidget(QWidget*, OTBImageViewerProcessor* );

    virtual ~QGLOtbImageViewerWidget();

    /*!
     * \brief initialize
     */
    void initialize();

    /*!
     * \brief updateFromProcessor , implementation
     */
    virtual void updateFromProcessor();
    
protected:
    /*!
     * \brief keyPressEvent , implementation
     */
    void keyPressEvent(QKeyEvent *);

signals:
    
public slots:
    //!


private slots:
    //!


private:

    /*!
     * \brief createViewer
     * \param port
     */
    void createViewer();

    /*!
     * \brief setupByPort
     * \param port , port in general
     */
    void setupByPort(voreen::Port* port) ;

    /*!
     * \brief assembleWidgets
     *  this function creates and assembles all the widgets into one single widget
     */
    void assembleWidgets();

    /*!
     * \brief disassembleWidgets
     *  this function splits the layout
     */
    void disassembleWidgets();

    /*!
     * \brief loggerCat_
     */
    static const std::string loggerCat_;

    /*!
     * \brief m_pItiOtbImageViewer , this is the main widget of this viewer
     */
    itiviewer::ItiOtbImageViewer *m_pItiOtbImageViewer;

    /*!
     * \brief m_pItiOtbImageFactory, a factory responsible for creating ItiOtbImageViewer instances..
     */
    itiviewer::ItiOtbImageViewerFactory *m_pItiOtbImageFactory;

    /*!
     * \brief m_pItiOtbImageViewerPanel, a panel widget for handling viewer parameters
     */
    itiviewer::ItiOtbImageViewerPanel *m_pItiOtbImageViewerPanel;

    /*!
     * \brief m_pvSplitter
     */
    QSplitter *m_pvSplitter;
};

}

#endif // VRN_QGLOTBIMAGEVIEWERWIDGET_H

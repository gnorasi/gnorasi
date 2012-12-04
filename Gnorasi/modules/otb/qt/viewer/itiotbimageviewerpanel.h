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

#ifndef ITIOTBIMAGEVIEWERPANEL_H
#define ITIOTBIMAGEVIEWERPANEL_H

#include <QWidget>
#include <QTabWidget>

namespace itiviewer{

//! class declarations
class ItiOtbImageViewerPanelDataTab;
class ItiOtbImageViewerPanelSetupTab;
class ItiOtbImageViewerPanelHistogramTab;
class ItiOtbImageViewerPanelPixelDescriptionTab;
class ItiOtbImageViewer;

/*!
 * \brief The ItiOtbImageViewerPanel class
 *  This is the widget handling the various parameters of the
 *  ItiOtbImageViewer.
 *  The tab widget will have 4 tabs , as the Monteverdi application has..
 */
class ItiOtbImageViewerPanel : public QWidget
{
    Q_OBJECT
public:
    //! ctor
    explicit ItiOtbImageViewerPanel(ItiOtbImageViewer* v,  QWidget *parent = 0);
    
    //! dtor
    virtual ~ItiOtbImageViewerPanel() {}

signals:
    
public slots:

private:
    //! inititializing stuff
    void initialize();

    //! the main tool box
    QTabWidget *m_pTabWidget;

    //! tab widget for setting up data properties
//    ItiOtbImageViewerPanelDataTab *m_pDataTab;

    //! tab widget for setting up setup properties
    ItiOtbImageViewerPanelSetupTab *m_pSetupTab;

    //! tab widget for setting up histogram properties
//    ItiOtbImageViewerPanelHistogramTab *m_pHistogramTab;

    //! tab widget for setting up pixel description properties
//    ItiOtbImageViewerPanelPixelDescriptionTab *m_pPixelDescriptionTab;

    //!
    ItiOtbImageViewer *m_pItiOtbImageViewer;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVIEWERPANEL_H

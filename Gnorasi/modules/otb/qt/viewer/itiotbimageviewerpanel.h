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
#include <QtCore/QHash>

namespace itiviewer{

//! class declarations
class ItiOtbImageViewerPanelDataTab;
class ItiOtbImageViewerPanelSetupTab;
class ItiOtbImageViewerPanelHistogramTab;
class ItiOtbImageViewerPanelPixelDescriptionTab;
class ItiOtbImageViewer;

class Command;

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
    enum COMMAND_KEY{
        COMMAND_CC  = 0,    //! color composition
        COMMAND_CE  = 1     //! contrast enhancement
    };

    //! ctor
    explicit ItiOtbImageViewerPanel(QWidget *parent = 0);
    
    //! dtor
    virtual ~ItiOtbImageViewerPanel() {}

    //! set the command to the hash
    void setCommand(COMMAND_KEY cmdk, Command *pC);

signals:
    //! emitted when the grey scale channel selection has been altered
    void greyScaleColorCompositionChannelChanged(int);

    //! emitted when the rgb channel selection has been altered
    void rgbColorCompositionChannelsChanged(int red, int green, int blue);

    //! emitted when the contrast enhancement method has been changed
    void contrastEnhancementChanged(int method, double aval, double bval);
    
private slots:
    //!
    void applyColorComposition();

    //!
    void applyConstrastEnhancement();

private:
    //! inititializing stuff
    void initialize();

    //! the main tool box
    QTabWidget *m_pTabWidget;

    //! tab widget for setting up setup properties
    ItiOtbImageViewerPanelSetupTab *m_pSetupTab;

    //! a has holding the command objects
    QHash<QString,Command*> m_commandHash;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVIEWERPANEL_H

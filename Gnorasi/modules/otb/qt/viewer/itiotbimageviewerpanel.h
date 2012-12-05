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
 *  \class The ItiOtbImageViewerPanel class
 *  This is the widget handling the various parameters of the
 *  ItiOtbImageViewer.
 *  The tab widget will have 4 tabs , as the Monteverdi application has..
 *
 *  The processing of various slots provided by the GUI of this class is handled
 *  by a commands mechanism. Firstly a client object creates a new command instance.
 *  Then the client connects the newly created command instance slots with signals provided by this class.
 *  Finally the client must use the setCommand operation which sets the created command instance
 *  to the apporpriate slot given a slot_key argument.
 *  A SLOT enumeration of the available slots is declared.
 *  For an example usage of the above mechanism please see the QOtbImageViewerWidget class source file.
 */
class ItiOtbImageViewerPanel : public QWidget
{
    Q_OBJECT
public:
    enum SLOT_KEY{
        SLOT_CC  = 0,    //! color composition
        SLOT_CE  = 1     //! contrast enhancement
    };

    /*!
     * \brief ItiOtbImageViewerPanel
     * \param parent
     */
    explicit ItiOtbImageViewerPanel(QWidget *parent = 0);
    
    /*!
     * \brief ~ItiOtbImageViewerPanel
     */
    virtual ~ItiOtbImageViewerPanel() {}

    /*!
     * \brief setCommand, adds the command to the hash
     * \param cmdk the slot key argument
     * \param pC the Command instance
     */
    void setCommand(SLOT_KEY cmdk, Command *pC);

signals:
    /*!
     * \brief greyScaleColorCompositionChannelChanged
     *  emitted when the grey scale channel selection has been altered
     */
    void greyScaleColorCompositionChannelChanged(int);

    /*!
     * \brief rgbColorCompositionChannelsChanged emitted when the rgb channel selection has been altered
     * \param red, the red channel number
     * \param green, the green channel number
     * \param blue, the blue channel number
     */
    void rgbColorCompositionChannelsChanged(int red, int green, int blue);

    /*!
     * \brief contrastEnhancementChanged emitted when the contrast enhancement method has been changed
     * \param method , the method to be used for contrast enhancement
     * \param aval, the a val could be a standard deviation value of a lower quantile value
     * \param bval, the bval could be -1.0 if the method is standard deviation, or the upper quantile value
     */
    void contrastEnhancementChanged(int method, double aval, double bval = -1.0);
    
private slots:
    /*!
     * \brief applyColorComposition
     */
    void applyColorComposition();

    /*!
     * \brief applyConstrastEnhancement
     */
    void applyConstrastEnhancement();

private:
    /*!
     * \brief initialize, inititializing stuff
     */
    void initialize();

    /*!
     * \brief m_pTabWidget , the main tool box
     */
    QTabWidget *m_pTabWidget;

    /*!
     * \brief m_pSetupTab , tab widget for setting up setup properties
     */
    ItiOtbImageViewerPanelSetupTab *m_pSetupTab;

    /*!
     * \brief m_commandHash , a has holding the command objects
     */
    QHash<QString,Command*> m_commandHash;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVIEWERPANEL_H

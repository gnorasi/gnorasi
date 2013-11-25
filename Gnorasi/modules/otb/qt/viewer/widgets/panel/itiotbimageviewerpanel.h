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
class ItiOtbImageManager;
class Command;
class ItiOtbImageChannelProvider;

/*!
 *  \class ItiOtbImageViewerPanel
 *
 *  \brief This is the widget handling the various parameters of the ItiOtbImageViewer.
 *
 *  The tab widget will have 4 tabs , as the Monteverdi application has..
 *
 *  The processing of various slots provided by the GUI of this class is handled
 *  by a commands mechanism. Firstly a client object creates a new command instance.
 *  Then the client connects the newly created command object's slots with signals provided by child objects of this class's instance.
 *  Finally the client must use the setCommand operation which sets the created command instance
 *  to the apporpriate slot given a SLOT_KEY argument.
 *  A SLOT enumeration of the available slots is declared below.
 *  For an example usage of the above mechanism please see the QOtbImageViewerWidget class source file
 *  and especially the setupCommands() function.
 *  Setting up this object also passes from setting a valid ItiOtbImageChannelProvider object .
 *  The ItiOtbImageChannelProvider object sets up the channels used by children objects ex. the setup tab .
 */
class ItiOtbImageViewerPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief The SLOT_KEY enum
     *  each command in the panel has a discrete slot enumaration value.
     *  The first two values are reserved for general purposes.
     *  each new command should just increase the value of the enumeration.
     */
    enum SLOT_KEY{
        SLOT_CC                 = 0,    //! color composition
        SLOT_CE                 = 1,    //! contrast enhancement
        SLOT_CE_GAUSSIAN        = 2,    //! gaussian filter
        SLOT_CE_LINEAR_0_255    = 3,    //! linear, the default
        SLOT_CE_LINEARXPERC     = 4,    //! linear X %
        SLOT_CE_SQUAREROOT      = 5,     //! square root
        SLOT_CC_GREYSCALE       = 6,    //! grey scale
        SLOT_CC_RGB             = 7,
        SLOT_CL_TOGGLEVISIBLE   = 11,    //! toggle the visibility of class label
        SLOT_CL_CHANGECOLOR     = 12
    };

    /*!
     * \brief ItiOtbImageViewerPanel, ctor!
     * \param parent
     */
    explicit ItiOtbImageViewerPanel(QWidget *parent = 0);
    
    /*!
     * \brief ~ItiOtbImageViewerPanel
     */
    virtual ~ItiOtbImageViewerPanel()   ;

    /*!
     * \brief setCommand, adds the command to the hash object
     *  The client object uses this function in order to set the command objects, which encapsulate the specialized core functionality.
     * \param cmdk the slot key argument
     * \param pC the Command instance
     */
    void setCommand(SLOT_KEY cmdk, Command *pC);

    /*!
     * \brief setProvider, setter
     * \param p
     */
    void setProvider(ItiOtbImageChannelProvider *p) { m_pItiOtbImageChannelProvider = p; }


    /*!
     * \brief provider, getter
     * \return
     */
    ItiOtbImageChannelProvider* provider() { return m_pItiOtbImageChannelProvider; }


    /*!
     * \brief setupTab
     * \return
     */
    ItiOtbImageViewerPanelSetupTab *setupTab() { return m_pSetupTab; }

    /*!
     * \brief saveDisplaySettings
     *  This function is called when the viewer is on a splitted mode,
     *  all it does is to save the GUI state of the widget on an INI settings file.
     */
    void saveDisplaySettings();

    /*!
     * \brief readDisplaySettings
     *
     *  This function is called when the viewer is on the splitted mode,
     *  all it does is to load and read the GUI settings size and pos and restore the widgets state
     *  according to the value red from the INI settings file.
     */
    void readDisplaySettings();

    /*!
     * \brief setManager , setter , need error checking
     * \param manager
     */
    void setManager(ItiOtbImageManager *manager) { m_pManager = manager; }

    /*!
     * \brief manager, getter function
     * \return
     */
    ItiOtbImageManager* manager() { return m_pManager; }


    /*!
     * \brief initialize, inititializing stuff
     */
    void initialize();

    /*!
     * \brief isGreyscale
     *  Returns the current mode is being used and setup by the number of channels and setup tab panel widget
     * \return
     */
    bool isGreyscale();

    /*!
     * \brief currentGreyscaleChannel
     * \return
     */
    int currentGreyscaleChannel() const ;


    /*!
     * \brief currentRedChannel
     * \return
     */
    unsigned currentRedChannel() const;

    /*!
     * \brief currentGreenChannel
     * \return
     */
    unsigned currentGreenChannel() const;

    /*!
     * \brief currentBlueChannel
     * \return
     */
    unsigned currentBlueChannel() const;

public slots:
    /*!
     * \brief setupChannels,
     *  this function is called during the setup process when a new image is being loaded.
     */
    void setupChannels();

    /*!
     * \brief setupHistogram
     * this funciton , is called during the setup process when a new image has been set or the channel selection has been changed
     */
    void setupHistogram();

    
private slots:

    /*!
     * \brief applyContrastEnhancementGaussian , triggers the respective command's execution call
     */
    void applyContrastEnhancementGaussian();

    /*!
     * \brief applyContrastEnhancementLinear0_255 , triggers the respective command's execution call
     */
    void applyContrastEnhancementLinear0_255();

    /*!
     * \brief applyContrastEnhancementLinearXPerc , triggers the respective command's execution call
     */
    void applyContrastEnhancementLinearXPerc();

    /*!
     * \brief applyContrastEnhancementSquareRoot , triggers the respective command's execution call
     */
    void applyContrastEnhancementSquareRoot();

    /*!
     * \brief applyColorCompositionGreyscale , triggers the respective command's execution call
     */
    void applyColorCompositionGreyscale();

    /*!
     * \brief applyColorCompositionRGB , triggers the respective command's execution call
     */
    void applyColorCompositionRGB();

    /*!
     * \brief applyToggleClassLabelVisible , triggers the respective command's execution call
     */
    void applyToggleClassLabelVisible();


    /*!
     * \brief applyChangeColorClass , triggers the respective command's execution call
     */
    void applyChangeColorClass();

private:

    /*!
     * \brief m_pTabWidget , the main tool box
     */
    QTabWidget *m_pTabWidget;

    /*!
     * \brief m_pSetupTab , tab widget for setting up setup properties
     */
    ItiOtbImageViewerPanelSetupTab *m_pSetupTab;

    /*!
     * \brief m_pHistogramTab
     */
    ItiOtbImageViewerPanelHistogramTab *m_pHistogramTab;

    /*!
     * \brief m_commandHash , a has holding the command objects
     *  keys are SLOT_KEY enum values
     */
    QHash<int,Command*> m_commandHash;

    /*!
     * \brief m_pItiOtbImageChannelProvider
     */
    ItiOtbImageChannelProvider *m_pItiOtbImageChannelProvider;

    /*!
     * \brief m_pManager
     */
    ItiOtbImageManager *m_pManager;
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVIEWERPANEL_H

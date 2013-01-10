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

#ifndef ITIOTBVECTORIMAGEVIEWERFACTORY_H
#define ITIOTBVECTORIMAGEVIEWERFACTORY_H

#include "itiotbimageviewerfactory.h"

#include <QObject>

namespace itiviewer{

class Command;
class ItiOtbImageViewer;
class ItiOtbImageViewerPanelSetupTab;

/*!
 * \brief The ItiOtbRgbaImageViewerFactory class
 *
 *  \class A factory class for creating viewers visualizing RGBA type of images
 *
 */
class ItiOtbVectorImageViewerFactory : public ItiOtbImageViewerFactory
{
public:
    //! ctor
    ItiOtbVectorImageViewerFactory(QObject *parent = 0);

    //! dtor
    ~ItiOtbVectorImageViewerFactory();

    /*!
     * \brief createViewer
     */
    void createViewer(ItiOtbImageViewerPanel *panel);

    /*!
     * \brief createCommandContrastEnhancementGaussian
     * \param sTab
     */
    Command* createCommandContrastEnhancementGaussian(ItiOtbImageViewerPanelSetupTab *sTab);

    /*!
     * \brief createCommandContrastEnhancementLinear0_255
     * \param sTab
     */
    Command* createCommandContrastEnhancementLinear0_255(ItiOtbImageViewerPanelSetupTab *sTab);

    /*!
     * \brief createCommandContrastEnhancementLinearXPerc
     * \param sTab
     */
    Command* createCommandContrastEnhancementLinearXPerc(ItiOtbImageViewerPanelSetupTab *sTab);

    /*!
     * \brief createCommandContrastEnhancementSquareRoot
     * \param sTab
     */
    Command* createCommandContrastEnhancementSquareRoot(ItiOtbImageViewerPanelSetupTab *sTab);

    /*!
     * \brief createCommandColorCompositionGreyscale
     * \return
     */
    Command* createCommandColorCompositionGreyscale(ItiOtbImageViewerPanelSetupTab* );

    /*!
     * \brief createCommandColorCompositionRGB
     * \return
     */
    Command* createCommandColorCompositionRGB(ItiOtbImageViewerPanelSetupTab* );

};

} // end of namespace itiviewer

#endif // ITIOTBVECTORIMAGEVIEWERFACTORY_H

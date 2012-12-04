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

#ifndef ITIOTBIMAGEVIEWER_H
#define ITIOTBIMAGEVIEWER_H

#include <QWidget>
#include <QLabel>

#include "itkImage.h"


namespace itiviewer{

/*!
 * \brief The ITIImageViewer class
 *
 * \class The ITIImageViewer class is an abstract product that is used by an application
 * where a visualization of images is needed.
 */
class ItiOtbImageViewer : public QWidget
{
    Q_OBJECT
public:
    enum COLORCOMPOSITION{
        COLORCOMPOSITION_GREYSCALE              = 0,
        COLORCOMPOSITION_RGBA                   = 1
    };

    enum CONTRASTENHACEMENT{
        CONTRASTENHACEMENT_LINEAR_0_255         = 0,
        CONTRASTENHACEMENT_LINEAR_X_PERC        = 1,
        CONTRASTENHACEMENT_GAUSSIAN             = 2,
        CONTRASTENHACEMENT_SQUARE_ROOT          = 3
    };

    //! ctor
    explicit ItiOtbImageViewer(QWidget *parent = 0);
    
    //! This is an abstract function needs to be implemented by every concrete subclass
    //! The implementation should contain the functionality to disassemble all viewer's widgets
    //! each sub widget should be a separate window
    virtual void disassembleWidgets()           = 0;

    //! This is an abstract function needs to be implemented by every concrete subclass
    //! The implementation should contain the functionality to assemble all viewer's widgets
    //! into one single window
    virtual void assembleWidgets()              = 0;

    //! This is a virtual method required for painting stuff
    virtual void draw()                         = 0;

    //! This is a virtual method for abstracting the zoomin functionality
    virtual void zoomIn()                       = 0;

    //! This is a virtual method for abstracting the zoom out functionality
    virtual void zoomOut()                      = 0;

    //! This is a virtual method for abstracting the setting the color mode functionality
    //! \param The band parameter sets the band channel will be used to draw the image
    virtual void setGreyScaleColorMode(int band) = 0;

    //! This is a virtual method for abstracting the setting the color mode functionality
    //! \param The red parameter sets the band channel to be used to visualize the red part
    //! \param The green parameter sets the band channel to be used to visualize the green part
    //! \param the blue parameter sets the band channel to be used to visualize the blue part
    virtual void setRGBColorMode(int red, int green, int blue) = 0;

    //! This is a virtual function for abstracting the contrast enhancement method
    //! \param The ce parameter sets the method to be set
    //! \param The aval parameter sets the lower quantile value or the standard deviation value if the ce param equals to CONTRASTENHACEMENT_GAUSSIAN
    //! \param The bval parameter equals to the upper quantile value
    void setContrastEnhancementMethod(CONTRASTENHACEMENT ce, double aval, double bval = 0.0);

protected:

};

} // namespace itiviewer

#endif // ITIOTBIMAGEVIEWER_H

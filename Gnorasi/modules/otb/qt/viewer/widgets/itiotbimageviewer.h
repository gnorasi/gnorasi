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

//
// Class declaration.
class AbstractModel;
class ItiOtbImageManager;

/*!
 * \class ItiOtbImageViewer
 *
 *  \brief The ITIImageViewer class is an abstract product that is used by an application
 *  where a visualization of images is needed.
 *
 *  The viewer has two modes as regards the layout perspective. The First mode is a packed
 *  mode where all widgets are merged and assembled into one single widget. The second mode
 *  is a splitted kind of mode , where all widgets have their own window.
 */
class ItiOtbImageViewer : public QWidget
{
    Q_OBJECT
public:

    /*!
     * \brief The VMODE enum  stands for Visualization mode and has two values, packed and splitted.
     */
    enum VMODE{
        VMODE_PACKED                                                                    = 0,
        VMODE_SPLITTED                                                                  = 1
    };

    //!
    //! ctor
    //!
    explicit ItiOtbImageViewer(QWidget *parent = 0);

    /*!
     * \brief disassembleWidgets
     *  This is an abstract function needs to be implemented by every concrete subclass
     *  The implementation should contain the functionality to disassemble all viewer's widgets
     *  each sub widget should be a separate window
     */
    virtual void disassembleWidgets()                                                   = 0;

    /*!
     * \brief assembleWidgets
     *  This is an abstract function needs to be implemented by every concrete subclass
     *  The implementation should contain the functionality to assemble all viewer's widgets
     *  into one single window
     */
    virtual void assembleWidgets()                                                      = 0;

    /*!
     * \brief draw
     *  This is a virtual method required for painting stuff
     */
    virtual void draw()                                                                 = 0;

    /**
     */
    // Method could be inline but it's better not new/delete in inline
    // methods (heap and memory-alignment contexts).
    void setModel( AbstractModel* model );

    /**
     */
    inline AbstractModel* model()
    {
        return m_pAbstractImageModel;
    }

    /*!
     * \brief vmode
     * \return
     */
    VMODE vmode() const                                                                 { return m_vmode; }


    /*!
     * \brief manager
     * \return
     */
    ItiOtbImageManager* manager()                                                       { return m_pManager; }


protected:
    /*!
     * \brief m_vmode , a variable holding the VMODE value
     */
    VMODE                                                                               m_vmode;

    /*!
     * \brief m_pAbstractImageModel
     */
    AbstractModel*                                                                      m_pAbstractImageModel;

    /*!
     * \brief m_pManager
     */
    ItiOtbImageManager                                                                  *m_pManager;

};

} // namespace itiviewer

#endif // ITIOTBIMAGEVIEWER_H

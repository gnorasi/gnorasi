/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef itiotbImageViewManipulatorZoomable_h
#define itiotbImageViewManipulatorZoomable_h


//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

#include "../vector_globaldefs.h"


//
// External classes pre-declaration.
namespace
{
}

namespace itiviewer
{
//
// Internal classes pre-declaration.

/** \class ImageViewManipulatorZoomable
 *  Brief this class handles the event related to a QGLWidget. It also
 *  handles :
 *    - NavigationContext : to store the region of the image to be
 *      rendered.
 *    - MouseContext : to remember the user action as a mouse press,
 *                     mouse drag
 *  
 *   WIP
 */
class ImageViewManipulatorZoomable :
    public QObject
{
  Q_OBJECT

//
// Public methods.
public:

  /** Constructor */
  ImageViewManipulatorZoomable( QObject* parent =NULL );

  /** Destructor */
  virtual ~ImageViewManipulatorZoomable();

  /** */
  void mouseMoveEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  void resizeEvent ( QResizeEvent * event );

  /** Methods to access to the context */
  /**
  const NavigationContextType& GetNavigationContext() const
  {
    return m_NavigationContext;
  }
  */
  inline
    const ImageRegionType&
    GetViewportImageRegion() const
  {
    return m_NavigationContext.m_ViewportImageRegion;
  }

  /*
  const MouseContextType&  GetMouseContextType() const
  {
    return m_MouseContext;
  }
  */

  void SetImageLargestRegion(const ImageRegionType & largestRegion)
  {
    m_NavigationContext.m_ModelImageRegion = largestRegion;

    // Need to call ConstrainRegion here cause not called when the input image
    // is opened
    this->ConstrainRegion(
      m_NavigationContext.m_ViewportImageRegion,
      m_NavigationContext.m_ModelImageRegion
    );
  }

  /*!
   * \brief setExtent
   * \param extent
   */
  void setExtent(const ImageRegionType &extent){
      m_NavigationContext.m_ExtentRegion = extent;
  }


  /*!
   * \brief extent
   * \return
   */
  ImageRegionType extent() const {
      return m_NavigationContext.m_ExtentRegion;
  }

  /*!
   * \brief bufferRegion
   * \return
   */
  ImageRegionType bufferRegion() const {
      return m_NavigationContext.m_ModelImageRegion;
  }

//
// SIGNALS.
signals:

//
// Public SLOTS.
public slots:
  void InitializeContext(int width, int height);

//
// Protected methods.
protected:
  void ConstrainRegion( ImageRegionType& region, const ImageRegionType& largest);
  
//
// Protected attributes.
protected:

//
// Private types.
private:
  /** Navigation context  */
  struct NavigationContextType
  {
    ImageRegionType m_ViewportImageRegion;
    ImageRegionType m_ModelImageRegion;
    ImageRegionType m_ExtentRegion;
  };

  /** Mouse context */
  struct MouseContextType
  {
    /** Default constructor */
    MouseContextType() :
      x( 0 ),
      y( 0 ),
      xMove( 0 ),
      yMove( 0 ),
      dx( 0 ),
      dy( 0 )
    {
    }

    int x;  // mousePress x
    int y;  // mousePress y
    int xMove;  // for mouseMove x
    int yMove;  // for mouseMove y
    int dx; // mouseMove in x (Drag)
    int dy; // mouseMove in y (Drag)
  };

//
// Private methods.
private:

//
// Private attributes.
private:
  NavigationContextType  m_NavigationContext;
  MouseContextType       m_MouseContext;

//
// SLOTS.
private slots:
};

} // end namespace itiviewer

#endif // itiotbImageViewManipulatorZoomable_h

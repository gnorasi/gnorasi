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

#ifndef itiotbImageModelRendererFullView_h
#define itiotbImageModelRendererFullView_h


//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
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
class AbstractImageModel;

/** \class ImageModelRenderer
 *
 */
class ImageModelRendererFullView :
    public QObject
{
  Q_OBJECT

//
// Public types.
public:
  struct RenderingContext
  {
     inline
     RenderingContext( const AbstractImageModel* model =NULL,
                       const ImageRegionType& region =ImageRegionType(),
                       unsigned int width = 0,
                       unsigned int height = 0,
                       double iz = 1.0) :
       m_AbstractImageModel( model ),
       m_ImageRegion( region ),
       m_WidgetWidth(width),
       m_WidgetHeight(height),
       m_isotropicZoom(iz)
    {
    }

    const AbstractImageModel* m_AbstractImageModel;
    ImageRegionType m_ImageRegion;
    unsigned int m_WidgetWidth;
    unsigned int m_WidgetHeight;
    double m_isotropicZoom;
  };

//
// Public methods.
public:
  /** Constructor */
  ImageModelRendererFullView( QObject* parent = NULL );

  /** Destructor */
  virtual ~ImageModelRendererFullView();

  /** */
  virtual void paintGL( const RenderingContext& context );

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private methods.
private:

//
// Private attributes.
private:

//
// SLOTS.
private slots:
};

} // end namespace itiviewer

#endif // itiotbImageModelRendererFullView_h

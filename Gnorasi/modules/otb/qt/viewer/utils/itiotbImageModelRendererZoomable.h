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

#ifndef itiotbImageModelRendererZoomable_h
#define itiotbImageModelRendererZoomable_h


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


class AbstractImageModel;

/*!
 * \brief The ImageModelRendererZoomable class
 */
class ImageModelRendererZoomable :
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
                       const ImageRegionType& ext = ImageRegionType(),
                       unsigned int width = 0,
                       unsigned int height = 0,
                       double iz = 1.0) :
       m_AbstractImageModel( model ),
       m_ImageRegion( region ),
       m_extent(ext),
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
    ImageRegionType m_extent;
  };

//
// Public methods.
public:
  /** Constructor */
  ImageModelRendererZoomable( QObject* parent = NULL );

  /** Destructor */
  virtual ~ImageModelRendererZoomable();

  /** */
  virtual void paintGL( const RenderingContext& context );

  //! getters
  unsigned int nbDisplayColumns()   const { return m_nb_displayed_cols; }
  unsigned int nbDisplayRows()      const { return m_nb_displayed_rows; }
  unsigned int firstDisplayColumn() const { return m_first_displayed_col; }
  unsigned int firstDisplayRow()    const { return m_first_displayed_row; }


  void setPaintingParameters(
          unsigned int nb_d_cs,
          unsigned int nb_d_rs,
          unsigned int f_d_c,
          unsigned int f_d_r){
        m_nb_displayed_cols     = nb_d_cs;
        m_nb_displayed_rows     = nb_d_rs;
        m_first_displayed_col   = f_d_c;
        m_first_displayed_row   = f_d_r;
  }

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

  /*!
   * \brief m_nb_displayed_rows , a variable holding the number of rows that are visualized on an image of a fixed size [columns,rows]
   */
  unsigned int m_nb_displayed_rows;

  /*!
   * \brief m_nb_displayed_cols , a variable holding the number of columns that are visualized on an image of a fixed size [columns,rows]
   */
  unsigned int m_nb_displayed_cols;

  /*!
   * \brief m_nb_displayed_rows , a variable holding the first visualized row on an image of a fixed size [columns,rows]
   */
  unsigned int m_first_displayed_row;

  /*!
   * \brief m_nb_displayed_cols , a variable holding the first visualized column on an image of a fixed size [columns,rows]
   */
  unsigned int m_first_displayed_col;

//
// SLOTS.
private slots:
};

} // end namespace itiviewer

#endif // itiotbImageModelRendererZoomable_h

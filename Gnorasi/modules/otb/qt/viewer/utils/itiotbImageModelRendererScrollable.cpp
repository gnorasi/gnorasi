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

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)
// necessary for the opengl variables and methods

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "itiotbImageModelRendererScrollable.h"
#include "../models/itiotbVectorImageModel.h"

using namespace itiviewer;
/*
  TRANSLATOR itiviewer::ImageModelRenderer

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
ImageModelRendererScrollable
::ImageModelRendererScrollable( QObject* parent ) :
  QObject( parent )
{
    m_first_displayed_col   = 0;
    m_first_displayed_row   = 0;
    m_nb_displayed_cols     = 0;
    m_nb_displayed_rows     = 0;
}

/*****************************************************************************/
ImageModelRendererScrollable
::~ImageModelRendererScrollable()
{
}

/*****************************************************************************/
void ImageModelRendererScrollable::paintGL( const RenderingContext& context )
{
    //! the VectorImageModel used for the rendering
    VectorImageModel * viModel = dynamic_cast<  VectorImageModel *>(
    const_cast<AbstractImageModel*>(context.m_AbstractImageModel)
    );

    ImageRegionType extent = context.m_extent;
    ImageRegionType bufferedRegion = context.m_ImageRegion;
    unsigned char* buffer = viModel->RasterizeRegion(bufferedRegion);


    VectorIndexType startPosition = extent.GetIndex();
    startPosition[0] = startPosition[0] < 0 ? 0 : startPosition[0];
    startPosition[1] = startPosition[1] < 0 ? 0 : startPosition[1];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, bufferedRegion.GetSize()[0]);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, m_first_displayed_col);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,m_first_displayed_row);

    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom(context.m_isotropicZoom,context.m_isotropicZoom);

    glRasterPos2f(startPosition[0], startPosition[1]);
    glDrawPixels(m_nb_displayed_cols,
       m_nb_displayed_rows,
       GL_RGB,
       GL_UNSIGNED_BYTE,
       buffer);

    glFlush();

    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/


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
#include "itiotbImageModelRendererAlt.h"
#include "../models/itiotbVectorImageModel.h"

using namespace itiviewer;
/*
  TRANSLATOR itiviewer::ImageModelRendererAlt

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
ImageModelRendererAlt
::ImageModelRendererAlt( QObject* parent ) :
  QObject( parent )
{
}

/*****************************************************************************/
ImageModelRendererAlt
::~ImageModelRendererAlt()
{
}

/*****************************************************************************/
void ImageModelRendererAlt::paintGL( const RenderingContext& context )
{
    // the VectorImageModel used for the rendering
    VectorImageModel * viModel = dynamic_cast<  VectorImageModel *>(
    const_cast<AbstractImageModel*>(context.m_AbstractImageModel)
    );

    // the region of the image to render
    const ImageRegionType&    region = context.m_ImageRegion;
    unsigned char* buffer = viModel->RasterizeRegion(region);

    VectorSizeType size;
    size [0] = static_cast<unsigned int>(context.m_isotropicZoom * static_cast<double>(region.GetSize()[0]));
    size [1] = static_cast<unsigned int>(context.m_isotropicZoom * static_cast<double>(region.GetSize()[1]));

    ImageRegionType::IndexType index;
    index[0] = (context.m_WidgetWidth - static_cast<int>(size[0])) / 2;
    index[1] = (context.m_WidgetHeight - static_cast<int>(size[1])) / 2;

    ImageRegionType extent;
    extent.SetIndex(index);
    extent.SetSize(size);

    unsigned int nb_displayed_rows;
    unsigned int nb_displayed_cols;
    unsigned int first_displayed_row;
    unsigned int first_displayed_col;

    if( extent.GetIndex()[0] >= 0 )
    {
        nb_displayed_cols = region.GetSize()[0];
        first_displayed_col = 0;
    }
    else
    {
        nb_displayed_cols = context.m_WidgetWidth / context.m_isotropicZoom;
        first_displayed_col = (region.GetSize()[0] - nb_displayed_cols) / 2;
    }

    if( extent.GetIndex()[1] >= 0 )
    {
        nb_displayed_rows = region.GetSize()[1];
        first_displayed_row = 0;
    }
    else
    {
        nb_displayed_rows = context.m_WidgetHeight / context.m_isotropicZoom;
        first_displayed_row = (region.GetSize()[1] - nb_displayed_rows) / 2;
    }


    VectorIndexType startPosition = extent.GetIndex();
    startPosition[0] = startPosition[0] < 0 ? 0 : startPosition[0];
    startPosition[1] = startPosition[1] < 0 ? 0 : startPosition[1];

//    setupViewport(width(), height());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, region.GetSize()[0]);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, first_displayed_col);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,first_displayed_row);

    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom(context.m_isotropicZoom,context.m_isotropicZoom);

    glRasterPos2f(startPosition[0], startPosition[1]);
    glDrawPixels(nb_displayed_cols,
       nb_displayed_rows,
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

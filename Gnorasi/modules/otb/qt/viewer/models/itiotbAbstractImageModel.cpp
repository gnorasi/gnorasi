
#include "itiotbAbstractImageModel.h"
#include "../utils/itiotbimagemanager.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)


using namespace itiviewer;
/*
  TRANSLATOR mvd::AbstractImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
AbstractImageModel
::AbstractImageModel( ItiOtbImageManager *manager, QObject* parent ) :
    m_pManager(manager), AbstractModel( parent )
{
}

/*******************************************************************************/
AbstractImageModel
::~AbstractImageModel()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/


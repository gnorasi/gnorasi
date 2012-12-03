#ifndef ITIOTBRGBAIMAGEVIEWERFACTORY_H
#define ITIOTBRGBAIMAGEVIEWERFACTORY_H

#include "itiotbimageviewerfactory.h"

#include <QObject>

namespace itiviewer{

class ItiOtbImageViewer;

/*!
 * \brief The ItiOtbRgbaImageViewerFactory class
 *
 *  \class A factory class for creating viewers visualizing RGBA type of images
 *
 */
class ItiOtbRgbaImageViewerFactory : public ItiOtbImageViewerFactory
{
public:
    //! ctor
    ItiOtbRgbaImageViewerFactory(QObject *parent = 0);

    //! dtor
    ~ItiOtbRgbaImageViewerFactory();

    //!
    ItiOtbImageViewer* createViewer();
};

} // end of namespace itiviewer

#endif // ITIOTBRGBAIMAGEVIEWERFACTORY_H

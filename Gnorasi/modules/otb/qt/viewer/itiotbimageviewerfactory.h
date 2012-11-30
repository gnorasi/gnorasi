#ifndef ITIIMAGEVIEWERFACTORY_H
#define ITIIMAGEVIEWERFACTORY_H

#include <QObject>

namespace itiviewer{

class ItiOtbImageViewer;

/*!
 * \brief The ITIImageViewerFactory class
 *
 * \class The ITIImageViewerFactory class is an abstract factory which is responsible for creating ITIImageViewer instances
 * Each subclass of this class must reimplement the create method.
 *
 */
class ItiOtbImageViewerFactory : public QObject
{
    Q_OBJECT
public:
    //! ctor
    explicit ItiOtbImageViewerFactory(QObject *parent = 0) : QObject(parent) { }

    //! abstract method, every subclass must implement this class
    virtual ItiOtbImageViewer* create() = 0;
    
};

} // end of namespace iti

#endif // ITIIMAGEVIEWERFACTORY_H

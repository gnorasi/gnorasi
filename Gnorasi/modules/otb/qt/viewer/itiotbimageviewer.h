#ifndef ITIOTBIMAGEVIEWER_H
#define ITIOTBIMAGEVIEWER_H

#include <QWidget>

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
    //! ctor
    explicit ItiOtbImageViewer(QWidget *parent = 0) : QWidget(parent) { }
    
    //! This is an abstract function needs to be implemented by every concrete subclass
    virtual void clearLayout() = 0;

    //! This is a virtual method required for painting stuff
    virtual void draw() = 0;

protected:

    
};

} // namespace itiviewer

#endif // ITIOTBIMAGEVIEWER_H

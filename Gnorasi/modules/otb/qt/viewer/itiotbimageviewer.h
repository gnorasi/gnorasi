#ifndef ITIOTBIMAGEVIEWER_H
#define ITIOTBIMAGEVIEWER_H

#include <QWidget>
#include <QLabel>

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
    explicit ItiOtbImageViewer(QWidget *parent = 0);
    
    //! This is an abstract function needs to be implemented by every concrete subclass
    //! The implementation should contain the functionality to disassemble all viewer's widgets
    //! each sub widget should be a separate window
    virtual void disassembleWidgets() = 0;

    //! This is an abstract function needs to be implemented by every concrete subclass
    //! The implementation should contain the functionality to assemble all viewer's widgets
    //! into one single window
    virtual void assembleWidgets() = 0;

    //! This is a virtual method required for painting stuff
    virtual void draw() = 0;

protected:
    //! the following labels were taken from the monteverdi viewer
    //! a label showing a title for the full resolution view
    QLabel *m_pLabelFullResolution;

    //! a label showing a title for the navigation view
    QLabel *m_pLabelNavigationView;

    //! a label showing a title for the zoom view
    QLabel *m_pLabelZoomView;

    
};

} // namespace itiviewer

#endif // ITIOTBIMAGEVIEWER_H

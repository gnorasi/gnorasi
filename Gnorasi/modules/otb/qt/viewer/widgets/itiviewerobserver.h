#ifndef ITIOTBFOCUSREGIONOBSERVER_H
#define ITIOTBFOCUSREGIONOBSERVER_H

#include <QWidget>

namespace itiviewer{

class ItiViewerObservable;

/*!
 * \brief The ItiOtbFocusRegionObserver class
 *  This is the base class for all subclasses .
 *  It observes an observable item. In this case of the viewer
 *  an observable item could be a focus region are or the visible area of a widget.
 *  Once the observable item's data has been modified, the function update is called.
 */
class ItiViewerObserver : public QWidget
{
    Q_OBJECT
public:
    explicit ItiViewerObserver(QWidget *parent = 0) : QWidget(parent) { }

    /*!
     * \brief notify and update appropriate data.
     */
    virtual void update(ItiViewerObservable* ) = 0;
    
signals:
    
public slots:

private:

    
};

} // namespace itiviewer

#endif // ITIOTBFOCUSREGIONOBSERVER_H

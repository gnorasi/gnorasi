#ifndef ITIOTBFOCUSREGIONOBSERVER_H
#define ITIOTBFOCUSREGIONOBSERVER_H

#include <QWidget>

namespace itiviewer{

class ItiViewerObservable;

/*!
 * \brief The ItiOtbFocusRegionObserver class
 *  This is the base class for all subclasses .
 *  It observes a focus region
 */
class ItiViewerObserver : public QWidget
{
    Q_OBJECT
public:
    explicit ItiViewerObserver(QWidget *parent = 0);

    /*!
     * \brief notify
     */
    virtual void update(ItiViewerObservable* ) = 0;
    
signals:
    
public slots:

private:

    
};

} // namespace itiviewer

#endif // ITIOTBFOCUSREGIONOBSERVER_H

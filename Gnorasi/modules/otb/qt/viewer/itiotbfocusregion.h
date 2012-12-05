#ifndef ITIOTBFOCUSREGION_H
#define ITIOTBFOCUSREGION_H

#include <QObject>

namespace itiviewer{

class ItiOtbFocusRegionObserver;

/*!
 * \brief The ItiOtbFocusRegion class
 *  This class is a subject of observing through various observer classes
 *  It contains variables related to the Image visualized and the region in
 *  which the scene is focused on.
 *  This class provides an iterface , a set of pure virtual functions all
 *  concrete subclasses must implement.
 */
class ItiOtbFocusRegion : public QObject
{
    Q_OBJECT
public:
    //!
    explicit ItiOtbFocusRegion(QObject *parent = 0);
    
    /*!
     * \brief registerObserver , adds an observer
     * \param obr,  the observer to register
     */
    void registerObserver(ItiOtbFocusRegionObserver *obr) { m_observerList.append(obr); }

    /*!
     * \brief unRegisterObserver , unregisters the observer object
     * \param obr , the observer instance to be unregistered by the list
     */
    void unRegisterObserver(ItiOtbFocusRegionObserver *obr) { m_observerList.removeOne(obr); }

    /*!
     * \brief notifyObservers , notifies all registered observer objects
     */
    void notifyObservers();

signals:
    
public slots:

private:
    /*!
     * \brief m_observerList , a list of observers
     */
    QList<ItiOtbFocusRegionObserver*> m_observerList;
    
};

}

#endif // ITIOTBFOCUSREGION_H

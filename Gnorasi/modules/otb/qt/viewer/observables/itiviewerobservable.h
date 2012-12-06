/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef ITIVIEWEROBSERVABLE_H
#define ITIVIEWEROBSERVABLE_H

#include <QObject>

namespace itiviewer{

class ItiViewerObserver;

/*!
 * \brief The ItiViewerObservable class
 *  This class is a subject of observing through various observer classes
 *  It contains variables related to the Image visualized and the region in
 *  which the scene is focused on.
 *  This class provides an iterface , a set of pure virtual functions all
 *  concrete subclasses must implement.
 */
class ItiViewerObservable : public QObject
{
    Q_OBJECT
public:
    //!
    explicit ItiViewerObservable(QObject *parent = 0);
    
    /*!
     * \brief registerObserver , adds an observer
     * \param obr,  the observer to register
     */
    void registerObserver(ItiViewerObserver *obr) { m_observerList.append(obr); }

    /*!
     * \brief unRegisterObserver , unregisters the observer object
     * \param obr , the observer instance to be unregistered by the list
     */
    void unRegisterObserver(ItiViewerObserver *obr) { m_observerList.removeOne(obr); }

    /*!
     * \brief notifyObservers , notifies all registered observer objects
     */
    void notifyObservers();


private:
    /*!
     * \brief m_observerList , a list of observers
     */
    QList<ItiViewerObserver*> m_observerList;
    
};

}

#endif // ITIVIEWEROBSERVABLE_H

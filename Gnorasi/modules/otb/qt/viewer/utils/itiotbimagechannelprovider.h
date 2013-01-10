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

#ifndef ITIOTBIMAGECHANNELPROVIDER_H
#define ITIOTBIMAGECHANNELPROVIDER_H

#include <QObject>

namespace itiviewer{

/*!
 * \brief The ItiOtbImageChannelProvider class , The provider provides channel information related to a set image
 */
class ItiOtbImageChannelProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> channels  READ channels   WRITE setChannels   NOTIFY channelsChanged)

public:
    explicit ItiOtbImageChannelProvider(QObject *parent = 0) : QObject(parent) { }

    /*!
     * \brief channels, getter
     * \return
     */
    QList<int> channels() const { return m_channelList; }

    /*!
     * \brief setChannels , setter
     * \param list
     */
    void setChannels(const QList<int> &list ) { m_channelList = list; }

    /*!
     * \brief parseImage , this is a pure virtual function, every sub class must implement this function
     */
    virtual void parse() = 0;
    
signals:
    /*!
     * \brief channelsChanged
     */
    void channelsChanged();
    
public slots:

protected:
    /*!
     * \brief m_channelList
     */
    QList<int> m_channelList;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGECHANNELPROVIDER_H

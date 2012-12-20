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
    virtual void parseImage() = 0;
    
signals:
    /*!
     * \brief channelsChanged
     */
    void channelsChanged();
    
public slots:

private:
    /*!
     * \brief m_channelList
     */
    QList<int> m_channelList;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGECHANNELPROVIDER_H

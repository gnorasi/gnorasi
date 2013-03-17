#ifndef ITIOTBLEVEL_H
#define ITIOTBLEVEL_H

#include <QObject>

namespace itiviewer{

class Region;

/*!
 * \brief The Level class
 *  Conecptually , a level is a container object which accumulates regions .
 *  The Level class is used on the classification process.
 */
class Level : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id   READ id     WRITE setId     NOTIFY idChanged)
public:
    /*!
     * \brief Level
     * \param parent
     */
    explicit Level(QObject *parent = 0);

    int id() const { return m_id; }
    void setId(int i) { m_id = i; }

    QList<Region*> regions() const { return m_regionList; }
    void setRegions(QList<Region*> rl) { m_regionList = rl; }


signals:
    void idChanged();

private:
    int m_id;

    QList<Region*> m_regionList;
    
};

} // end of namespace itiviewer

#endif // ITIOTBLEVEL_H

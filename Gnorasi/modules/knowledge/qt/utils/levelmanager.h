#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QObject>


// a helper macro
#define LEVELMANAGER LevelManager::instance()

class Level;

class LevelManager : public QObject
{
    Q_OBJECT
public:
    static LevelManager* instance();
    static void deleteInstance();
    
    QList<Levels*> levelList() const { return m_levelList; }
    void setLevelList(const QList<Level*> &l) { m_levelList = l; }

    void removeLevel(Level *l) {m_levelList.remoOne(l); }
    void addLevel(Level *l) {m_levelList.append(l); }

    void clearLevels() { qDeleteAll(m_levelList); m_levelList.clear(); }

signals:
    
public slots:
    

private:
    explicit LevelManager(QObject *parent = 0);
    ~LevelManager();

    static LevelManager *m_pInstance;

    QList<Level*> m_levelList;
};

#endif // LEVELMANAGER_H

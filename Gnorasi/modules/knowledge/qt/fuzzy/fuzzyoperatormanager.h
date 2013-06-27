#ifndef FUZZYOPERATORMANAGER_H
#define FUZZYOPERATORMANAGER_H

#include <QObject>

class FuzzyOperator;

// a helper macro
#define FUZZYOPERATORMANAGER FuzzyOperatorManager::instance()

class FuzzyOperatorManager : public QObject
{
    Q_OBJECT
public:
    
    static FuzzyOperatorManager* instance();
    static void deleteInstance();

    QList<FuzzyOperator*> fuzzyOperatorList() const { return m_fuzzyOperatorList; }
    void setFuzzyOperatorList(const QList<FuzzyOperator*> &l){ m_fuzzyOperatorList = l; }

    void addOperator(FuzzyOperator *f) { m_fuzzyOperatorList.append(f); }
    void removeOperator(FuzzyOperator *f) { m_fuzzyOperatorList.removeOne(f); }

    void clear() { qDeleteAll(m_fuzzyOperatorList); m_fuzzyOperatorList.clear(); }

signals:
    
public slots:

private:
    explicit FuzzyOperatorManager(QObject *parent = 0);

    ~FuzzyOperatorManager();

    static FuzzyOperatorManager *m_pInstance;

    QList<FuzzyOperator*> m_fuzzyOperatorList;
    
};

#endif // FUZZYOPERATORMANAGER_H

#ifndef FUZZYFUNCTIONMANAGER_H
#define FUZZYFUNCTIONMANAGER_H

#include <QObject>

#define FUZZYFUNCTIONMANAGER FuzzyFunctionManager::instance()

class FuzzyFunction;

class FuzzyFunctionManager : public QObject
{
    Q_OBJECT
public:
    static FuzzyFunctionManager* instance();

    static void deleteInstance();
    
    QList<FuzzyFunction*> fuzzyFunctionList() const { return m_fuzzyFunctionList; }

    void addFuzzyFunction(FuzzyFunction *l) { m_fuzzyFunctionList.append(l); }

    void clear() { qDeleteAll(m_fuzzyFunctionList); m_fuzzyFunctionList.clear(); }

    FuzzyFunction* fuzzyFunctionById(int );

    int count() const { return m_fuzzyFunctionList.count(); }

signals:
    
public slots:

private:
    explicit FuzzyFunctionManager(QObject *parent = 0);

    ~FuzzyFunctionManager();
    
    QList<FuzzyFunction*> m_fuzzyFunctionList;

    static FuzzyFunctionManager *m_pInstance;

};

#endif // FUZZYFUNCTIONMANAGER_H

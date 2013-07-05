#ifndef FUZZYRULEMANAGER_H
#define FUZZYRULEMANAGER_H

#include <QObject>

class FuzzyRule;

#define FUZZYRULEMANAGER FuzzyRuleManager::instance()

class FuzzyRuleManager : public QObject
{
    Q_OBJECT
public:
    static FuzzyRuleManager* instance();

    static void deleteInstance();

    FuzzyRule* fuzzyRuleById(int );

    QList<FuzzyRule*> fuzzyRuleList() const {return m_fuzzyRuleList; }

    void clear() { qDeleteAll(m_fuzzyRuleList); m_fuzzyRuleList.clear(); }

    void addFuzzyRule(FuzzyRule *pRule) { m_fuzzyRuleList.append(pRule); }
    void removeFuzzyRule(FuzzyRule *pRule ) { m_fuzzyRuleList.removeOne(pRule); }

    QStringList fuzzyRulesNames();
    
signals:
    
public slots:


private:
    explicit FuzzyRuleManager(QObject *parent = 0);

    ~FuzzyRuleManager();

    QList<FuzzyRule*>  m_fuzzyRuleList;

    static FuzzyRuleManager* m_pInstance;
};

#endif // FUZZYRULEMANAGER_H

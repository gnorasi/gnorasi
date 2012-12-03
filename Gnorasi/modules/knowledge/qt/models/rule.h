#ifndef RULE_H
#define RULE_H

#include <QObject>

namespace voreen{

class                   RuleItem;
class                   OntologyClassItem;
class                   RuleUtility;
class                   PersistentRuleItem;

//!
//! The Rule class contains information about Rules the user can set through
//! the related processors example GeoClassification processor
//! A Rule is always attributed to a reference Ontology Class
//! A Rule has a confidence number ( ask Babis for more )
//! A Rule has a unique if possible id
//! A Rule has a list of Rule items
//!
class                   Rule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  id          READ id         WRITE   setid           NOTIFY idChanged)
    Q_PROPERTY(QString  headClass   READ headClass  WRITE   setHeadClass    NOTIFY headClassChanged)
    Q_PROPERTY(double   confidence  READ confidence WRITE   setConfidence   NOTIFY confidenceChanged)
public:
    virtual             ~Rule();
    
    //
    QString             id() const { return m_id; }
    void                setid(const QString &cl) { m_id = cl; }

    //
    QString             headClass() const { return m_headClass; }
    void                setHeadClass(const QString &cl) { m_headClass = cl; }

    //
    double              confidence() const { return m_confidence; }
    void                setConfidence(double l) { m_confidence = l; }

    //
    void                setRuleItem(QList<RuleItem*> l) { m_ruleItemList = l ; }
    QList<RuleItem*>    ruleItemList() { return m_ruleItemList; }

    //
    void                appendRuleItem(RuleItem *r) { m_ruleItemList.append(r) ; }
    bool                removeRuleItem(RuleItem *r) { return m_ruleItemList.removeOne(r); }
    void                removeRuleItems(int position,int rows);

    //
    int                 childCount() { return m_ruleItemList.count(); }

    //
    RuleItem*           childAt(int);

    //
    QList<RuleItem*>    ruleItemList() const {return m_ruleItemList; }

    //
    RuleItem*           createRuleItem() ;

signals:
    void                idChanged();
    void                headClassChanged();
    void                confidenceChanged();
    
public slots:

private:
    friend class        RuleUtility;

    explicit            Rule(const QString& headc, QObject *parent = 0);

    QList<RuleItem*>    m_ruleItemList;

    QString             m_id;
    QString             m_headClass;
    double              m_confidence;
};

} // namespace voreen

#endif // RULE_H

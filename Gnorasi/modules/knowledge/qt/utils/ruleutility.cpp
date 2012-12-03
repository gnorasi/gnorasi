#include "ruleutility.h"
#include "../models/rule.h"
#include "../models/ruleitem.h"
#include "../models/ontologyclassitem.h"

#include <QStringList>

using namespace voreen;

RuleUtility* RuleUtility::m_pInstance = NULL;

RuleUtility::RuleUtility()
{
}

RuleUtility* RuleUtility::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new RuleUtility();

    return m_pInstance;
}

void RuleUtility::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//!
void RuleUtility::clear(){
    qDeleteAll(m_ruleList);
    m_ruleList.clear();
}

Rule * RuleUtility::createRule(const QString &ocid){
    Rule *pRule = new Rule(ocid,this);
    // create a new unique id key
    QString uid = uniqueId();
    pRule->setid(uid);

    // append the new item to the list
    m_ruleList << pRule;

    return pRule;
}

QList<Rule*> RuleUtility::ruleListByHeadClass(const QString &name){
    QList<Rule*> list;
    QList<Rule*>::const_iterator i;
    for(i = m_ruleList.constBegin(); i != m_ruleList.constEnd(); i++){
        Rule *pRule = *i;

        if(!pRule->headClass().compare(name))
            list << pRule;
    }

    return list;
}

Rule* RuleUtility::ruleById(const QString &id){
    QList<Rule*>::const_iterator i;
    for(i = m_ruleList.constBegin(); i != m_ruleList.constEnd(); i++){
        Rule *pRule = *i;

        if(!pRule->id().compare(id))
            return pRule;
    }

    return 0;
}


QString RuleUtility::uniqueId() const{
    // initialize max number to 0
    int max = 0;

    QList<Rule*>::const_iterator i;
    for(i = m_ruleList.constBegin(); i != m_ruleList.constEnd(); i++){
        Rule *pRule = *i;

        QString idt = pRule->id();
        int idn = idt.mid(idt.indexOf("_")+1).toInt();

        max = qMax(max,idn);
    }

    return QString("Rule_%1").arg(QString::number(max+1));
}

//
QStringList RuleUtility::classNamesList() const{
    QStringList names;

    QList<Rule*>::const_iterator i;
    for(i = m_ruleList.constBegin(); i != m_ruleList.constEnd(); i++){
        Rule *pRule = *i;
        QString headclass = pRule->headClass();
        if(!names.contains(headclass))
            names.append(headclass);

        QList<RuleItem*>::const_iterator k;
        QList<RuleItem*> ritemlist = pRule->ruleItemList();
        for(k = ritemlist.constBegin(); k != ritemlist.constEnd(); k++){
            RuleItem *pRuleItem = *k;

            headclass = pRuleItem->headClass();
            if(!names.contains(headclass))
                names.append(headclass);
        }
    }

    return names;
}

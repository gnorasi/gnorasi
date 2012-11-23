#include "rule.h"
#include "ruleitem.h"
#include "ontologyclassitem.h"

using namespace voreen;

Rule::Rule(const QString &headec, QObject *parent) :
    m_headClass(headec), m_confidence(0.0), QObject(parent)
{

}

//
RuleItem* Rule::childAt(int id) {
    if(m_ruleItemList.count()>id && id >=0 )
        return m_ruleItemList.at(id);
    return 0;
}

void Rule::removeRuleItems(int position, int rows){
    QMutableListIterator<RuleItem*> i(m_ruleItemList);
    int init = 0;
    while(i.hasNext()){
        RuleItem *item = i.next();
        if(!init){
            if(m_ruleItemList.indexOf(item) == position)
                init++;
        }

        if(init && init <= rows){
            i.remove();
            init++;
        }
    }
}

//
RuleItem* Rule::createRuleItem(){
    RuleItem *pRuleItem = new RuleItem(QVector<QVariant>(4));
    m_ruleItemList.append(pRuleItem);

    return pRuleItem;
}

//
Rule::~Rule()
{
    qDeleteAll(m_ruleItemList);
    m_ruleItemList.clear();
}

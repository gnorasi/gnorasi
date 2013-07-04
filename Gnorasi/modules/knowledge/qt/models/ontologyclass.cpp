#include <QStringList>

#include "ontologyclass.h"

//using namespace voreen;

#include "../fuzzy/fuzzyrule.h"

OntologyClass::OntologyClass(OntologyClass *parent)
    : QObject(parent)
{
    parentItem = parent;
}


OntologyClass::~OntologyClass()
{
    qDeleteAll(childItems);
}


OntologyClass *OntologyClass::child(int number)
{
    return childItems.value(number);
}


int OntologyClass::childCount() const
{
    return childItems.count();
}


int OntologyClass::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OntologyClass*>(this));

    return 0;
}


OntologyClass *OntologyClass::parent()
{
    return parentItem;
}

void OntologyClass::removeFuzzyRule(int l, FuzzyRule *r){
    QList<FuzzyRule*> rList = m_fuzzyRuleHash.values(l);
    rList.removeOne(r);
    m_fuzzyRuleHash.remove(l);

    QList<FuzzyRule*>::const_iterator i;
    for(i = rList.begin(); i != rList.end(); i++){
        FuzzyRule *pr = *i;

        m_fuzzyRuleHash.insertMulti(l,pr);
    }
}



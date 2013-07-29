#include "fuzzyrule.h"
//#include "ruleitem.h"
//#include "../models/ontologyclass.h"
#include "fuzzyfunction.h"
//#include "../models/fuzzyrulemodel.h"

#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"


FuzzyRule::FuzzyRule(FuzzyFunction *p, QObject *parent) :
    m_pFuzzyFunction(p),  QObject(parent)
{

}


QString FuzzyRule::name() const {
    ObjectAttribute *attr = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(m_levelId,m_attributeId);

    if(!attr)
        return QString();

    return attr->name();
}



//
FuzzyRule::~FuzzyRule()
{

}

#include "fuzzyrule.h"
//#include "ruleitem.h"
#include "../models/ontologyclass.h"
#include "fuzzyfunction.h"

using namespace voreen;

FuzzyRule::FuzzyRule(FuzzyFunction *p, QObject *parent) :
    m_pFuzzyFunction(p),  QObject(parent)
{

}




//
FuzzyRule::~FuzzyRule()
{

}

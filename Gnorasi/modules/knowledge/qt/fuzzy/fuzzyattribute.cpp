#include "fuzzyattribute.h"
#include "fuzzyfunctionfactory.h"
#include "fuzzyfunction.h"

#include <QVariant>

QStringList FuzzyAttribute::fuzzyOperatorNames = QStringList()  << ">=" << ">" << "<=" << "<";


FuzzyAttribute::FuzzyAttribute(const QString &n , QObject *parent) :
    m_name(n), m_fuzzyOperator(FOT_NOTSET), QObject(parent)
{
}

double FuzzyAttribute::parameterValueForIndex(int index) const{
    return m_pFuzzyFunction->parameterFunctionForIndex(index);
}


QString FuzzyAttribute::fuzzyOperatorName(){
    QString name ;
    if((int)m_fuzzyOperator > 0 && (int)m_fuzzyOperator < fuzzyOperatorNames.count())
        name = fuzzyOperatorNames.at((int)m_fuzzyOperator);

    return name;
}

void FuzzyAttribute::setParameterValueForIndex(int index, double val){
    m_pFuzzyFunction->setParameterValueForIndex(index,val);
}

FuzzyAttribute::FUZZYOPERATORTYPE FuzzyAttribute::fuzzyOperatorByName(const QString &name){
    return (FUZZYOPERATORTYPE)fuzzyOperatorNames.indexOf(name);
}


bool FuzzyAttribute::updateAttribute(int index, const QVariant &val, FuzzyFunctionFactory *pFactory){
    if(index == 1){ // then this is the function
        QString ftype = val.toString();
        FuzzyFunction *pFuzzyFunction = pFactory->createFuzzyFunction(ftype);
        setFuzzuFunction(pFuzzyFunction);

    }else if(index == m_pFuzzyFunction->MAXPARAMETERSCOUNT + 2){ // then it is the operator
        QString oname = val.toString();
        setFuzzyOperator(fuzzyOperatorByName(oname));
    }else if(index == m_pFuzzyFunction->MAXPARAMETERSCOUNT + 3){ // then it is the member value
        setMemberShipValues(val.toDouble());
    }else{ // the it is the parameters value
        m_pFuzzyFunction->setParameterValueForIndex(index,val.toDouble());
    }

    return true;
}

#include "fuzzyattribute.h"

#include "fuzzyfunction.h"

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

#include "fuzzyfunctionrightshoulder.h"

FuzzyFunctionRightShoulder::FuzzyFunctionRightShoulder(QObject *parent) :
    m_parameterA(0.0),
    m_parameterB(0.0),
    FuzzyFunction(parent)
{
}


double FuzzyFunctionRightShoulder::parameterFunctionForIndex(int index){
    return index ? m_parameterA : m_parameterB;
}


void FuzzyFunctionRightShoulder::setParameterValueForIndex(int index, double val)
{
    if(index == 0)
        m_parameterA = val;
    else if(index == 1)
        m_parameterB = val;
    else
        m_parameterA = val;
}


double FuzzyFunctionRightShoulder::calculate(double val){
    double calcVal = 0.0;

    return calcVal;
}

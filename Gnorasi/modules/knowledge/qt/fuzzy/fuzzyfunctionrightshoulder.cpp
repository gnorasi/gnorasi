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

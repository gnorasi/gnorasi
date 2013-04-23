#include "fuzzyfunctionleftshoulder.h"

FuzzyFunctionLeftShoulder::FuzzyFunctionLeftShoulder(QObject *parent) :
    m_parameterA(0.0),
    m_parameterB(0.0),
    FuzzyFunction(parent)
{

}


double FuzzyFunctionLeftShoulder::parameterFunctionForIndex(int index){
    return index ? m_parameterA: m_parameterB;
}


void FuzzyFunctionLeftShoulder::setParameterValueForIndex(int index, double val){
    if(index == 0)
        m_parameterA = val;
    else if(index = 1)
        m_parameterB = val;
    else
        m_parameterA = val;
}


double FuzzyFunctionLeftShoulder::calculate(double val){
    double calcVal = 0.0;

    return calcVal;
}

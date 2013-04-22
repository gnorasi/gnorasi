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

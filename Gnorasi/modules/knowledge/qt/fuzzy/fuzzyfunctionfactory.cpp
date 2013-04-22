#include "fuzzyfunctionfactory.h"

#include "fuzzyfunctionleftshoulder.h"
#include "fuzzyfunctionrightshoulder.h"

#define FUZZYFUNCTIONLEFTSHOULDER "Left Shoulder"
#define FUZZYFUNCTIONRIGHTSHOULDER "Right Shoulder"

FuzzyFunctionFactory::FuzzyFunctionFactory(QObject *parent) :
    QObject(parent)
{
}


FuzzyFunction* FuzzyFunctionFactory::createFuzzyFunction(const QString &name){
    if(!name.compare(QString::fromAscii(FUZZYFUNCTIONLEFTSHOULDER)))
        return new FuzzyFunctionLeftShoulder(this);
    else if(!name.compare(QString::fromAscii(FUZZYFUNCTIONRIGHTSHOULDER)))
        return new FuzzyFunctionRightShoulder(this);
    else
        return new FuzzyFunctionLeftShoulder(this); // this is the default fuzzy function
}

#include "fuzzyontologyclass.h"

#include "fuzzyattribute.h"

FuzzyOntologyClass::FuzzyOntologyClass(QObject *parent) :
    QObject(parent)
{
}


bool FuzzyOntologyClass::removeFuzzyAttribute(const QString &name){
    FuzzyAttribute *pAttribute = getAttributeByName(name);

    if(pAttribute != NULL){
        m_fuzzyAttributeList.removeOne(pAttribute);
        return true;
    }

    return false;
}

FuzzyAttribute* FuzzyOntologyClass::getAttributeByName(const QString &name){
    QList<FuzzyAttribute*>::const_iterator i;
    for(i = m_fuzzyAttributeList.constBegin(); i != m_fuzzyAttributeList.constEnd(); i++){
        FuzzyAttribute *pAttribute = *i;

        if(!pAttribute->name().compare(name))
            return pAttribute;
    }

    return 0;
}

FuzzyAttribute* FuzzyOntologyClass::getAttributeByIndex(int index){
    if(index > 0 && index < m_fuzzyAttributeList.count())
        return m_fuzzyAttributeList.at(index);
    else
        return 0;
}

bool FuzzyOntologyClass::updateAttribute(int row, int column, const QVariant &val, FuzzyFunctionFactory *pFactory){
    FuzzyAttribute *pFuzzyAttribute = getAttributeByIndex(row);
    if(pFuzzyAttribute){
        return pFuzzyAttribute->updateAttribute(column,val,pFactory);
    }


    return false;
}

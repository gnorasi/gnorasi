#include "fuzzyontologyclass.h"

#include "fuzzyattribute.h"

FuzzyOntologyClass::FuzzyOntologyClass(QObject *parent) :
    QObject(parent)
{
}


FuzzyAttribute* FuzzyOntologyClass::addAttribute(const QString &name){

}


QString FuzzyOntologyClass::createANewName(const QString &name){
    int counter = 1;
    QList<FuzzyAttribute*>::const_iterator i;
    for(i = m_fuzzyAttributeList.constBegin(); i != m_fuzzyAttributeList.constEnd(); i++){
        FuzzyAttribute *pAttribute = *i;

        if(!pAttribute->name().compare(name))
            counter++;
    }

    return QString("Fuzzy%1%2").arg(name).arg(QString::number(counter));
}

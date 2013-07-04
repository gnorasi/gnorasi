#include <QStringList>

#include "ontologyclass.h"

//using namespace voreen;

#include "../fuzzy/fuzzyrule.h"

OntologyClass::OntologyClass(OntologyClass *parent)
    : QObject(parent)
{
    parentItem = parent;
}


OntologyClass::~OntologyClass()
{
    qDeleteAll(childItems);
}


OntologyClass *OntologyClass::child(int number)
{
    return childItems.value(number);
}


int OntologyClass::childCount() const
{
    return childItems.count();
}


int OntologyClass::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OntologyClass*>(this));

    return 0;
}


OntologyClass *OntologyClass::parent()
{
    return parentItem;
}



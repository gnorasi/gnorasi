#include "itiotblevel.h"

using namespace itiviewer;

Level::Level(QObject *parent) :
    m_id(1), QObject(parent)
{
}

Level::~Level(){
    qDeleteAll(m_regionList);
    m_regionList.clear();
}

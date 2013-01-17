#ifndef ITIOTBLABELMAPPARSER_H
#define ITIOTBLABELMAPPARSER_H

#include <QObject>

#include "../../../ports/otblabelmapport.h"

#include "itkShapeLabelObject.h"

using namespace voreen;

namespace itiviewer {

class Region;

class LabelMapParser : public QObject
{
    Q_OBJECT
public:
    explicit LabelMapParser(QObject *parent = 0);

    typedef unsigned long                         LabelType;
    typedef otb::Polygon<double> PolygonType;
    typedef itk::ShapeLabelObject<LabelType, 2> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType>        LabelMapType;
    
    /*!
     * \brief parse
     * \param lblmap
     * \return
     */
    QList<Region*> parse(LabelMapType* lblmap);

signals:
    
public slots:
    
};

} // end of namespace itiviewer

#endif // ITIOTBLABELMAPPARSER_H

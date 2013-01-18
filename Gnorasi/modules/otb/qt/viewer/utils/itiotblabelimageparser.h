#ifndef ITIOTBLABELIMAGEPARSER_H
#define ITIOTBLABELIMAGEPARSER_H

#include <QObject>

#include "../../../ports/otblabelimageport.h"

#include "itkShapeLabelObject.h"

using namespace voreen;

namespace itiviewer {

class Region;

class LabelImageParser : public QObject
{
    Q_OBJECT
public:
    explicit LabelImageParser(QObject *parent = 0);

    typedef otb::Image<unsigned long, 2> LabelImageType;
    
    /*!
     * \brief parse
     * \param lblmap
     * \return
     */
    QList<Region*> parse(LabelImageType* lblimage);

signals:
    
public slots:
    
};

} // end of namespace itiviewer

#endif // ITIOTBLABELIMAGEPARSER_H

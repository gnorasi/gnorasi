#ifndef ITIOTBIMAGERGBACHANNELPROVIDER_H
#define ITIOTBIMAGERGBACHANNELPROVIDER_H

#include "itiotbimagechannelprovider.h"

#include "../rgba_globaldefs.h"

namespace itiviewer{

class ItiOtbImageRgbaChannelProvider : public ItiOtbImageChannelProvider
{
    Q_OBJECT
public:
    explicit ItiOtbImageRgbaChannelProvider(RasterImageType *imgT, QObject *parent = 0);
    
    /*!
     * \brief parseImage, implementation
     */
    void parseImage();

signals:
    
public slots:

private:
    RasterImageType *m_pRasterImageType;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGERGBACHANNELPROVIDER_H

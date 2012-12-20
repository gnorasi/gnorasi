#ifndef ITIOTBIMAGEVECTORCHANNELPROVIDER_H
#define ITIOTBIMAGEVECTORCHANNELPROVIDER_H

#include "itiotbimagechannelprovider.h"

#include "../vector_globaldefs.h"

namespace itiviewer{

class ItiOtbImageVectorChannelProvider : public ItiOtbImageChannelProvider
{
    Q_OBJECT
public:
    explicit ItiOtbImageVectorChannelProvider(VectorImageType *imgT, QObject *parent = 0);
    
    /*!
     * \brief parseImage, implementation
     */
    void parseImage();

signals:
    
public slots:

private:
    VectorImageType *m_pVectorImageType;
    
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEVECTORCHANNELPROVIDER_H

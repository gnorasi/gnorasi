/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_OTBIMAGEPORT_H
#define VRN_OTBIMAGEPORT_H

#include "voreen/core/voreencoredefine.h"
#include "voreen/core/ports/port.h"
//#include "Utilities/ITK/Common/itkSmartPointer.h"
#include "IO/otbImage.h"

namespace voreen {

class VRN_CORE_API OTBImagePort : public Port {
public:
    explicit OTBImagePort(PortDirection direction, const std::string& name,
                        bool allowMultipleConnections = false,
                        Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    ~OTBImagePort();
    
    typedef otb::Image<double, 2> ImageType;
    typedef ImageType::Pointer ImagePointer;

    virtual void setData(const ImagePointer& pointer);

    virtual ImagePointer getData() const;

    /// Returns true.
    virtual bool hasData() const;

    //virtual std::vector<ImagePointer> getAllData() const;

    std::vector<const OTBImagePort* > getConnected() const;

    /**
     * Returns true, if the port is connected
     */
    virtual bool isReady() const;

protected:
    ImagePointer portData_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBIMAGEPORT_H

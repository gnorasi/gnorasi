/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_OTBSMARTPOINTERPORT_H
#define VRN_OTBSMARTPOINTERPORT_H

#include "voreen/core/voreencoredefine.h"
#include "voreen/core/ports/port.h"
#include "itkSmartPointer.h"

namespace voreen {

template <class TObjectType> class VRN_CORE_API OTBSmartPointerPort : public Port {
public:
    typedef TObjectType ObjectType;
    explicit OTBSmartPointerPort(PortDirection direction, const std::string& name,
                        bool allowMultipleConnections = false,
                        Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    ~OTBSmartPointerPort();
    
    typedef itk::SmartPointer<ObjectType> SmartPointer;
    
    virtual void setData(const SmartPointer& pointer);

    virtual SmartPointer getData() const;

    /// Returns true.
    virtual bool hasData() const;

    //virtual std::vector<ImagePointer> getAllData() const;

    std::vector<const OTBSmartPointerPort* > getConnected() const;

    /**
     * Returns true, if the port is connected
     */
    virtual bool isReady() const;

protected:
    SmartPointer portData_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBSMARTPOINTERPORT_H

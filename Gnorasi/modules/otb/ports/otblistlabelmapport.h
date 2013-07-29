#ifndef OTBLISTLABELMAPPORT_H
#define OTBLISTLABELMAPPORT_H

#include "voreen/core/voreencoreapi.h"
#include "voreen/core/ports/port.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"


namespace voreen {

class VRN_CORE_API OTBListLabelMapPort : public Port
{
public:
    explicit OTBListLabelMapPort(PortDirection direction, const std::string& name,
                 bool allowMultipleConnections = false,
                 Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);

    ~OTBListLabelMapPort();


    typedef unsigned long LabelType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;

    typedef LabelMapType::Pointer LabelMapSmartPointer;
    typedef LabelMapType* LabelMapPointer;

    virtual void setData(const std::vector<LabelMapPointer>& pointer);

    virtual std::vector<LabelMapPointer> getData() const;

    /// Returns true.
    virtual bool hasData() const;


    std::vector<const OTBListLabelMapPort* > getConnected() const;

    /**
     * Returns true, if the port is connected
     */
    virtual bool isReady() const;

private:
    std::vector<LabelMapPointer> portData_;

    static const std::string loggerCat_; ///< category used in logging
};

}

#endif // OTBLISTLABELMAPPORT_H

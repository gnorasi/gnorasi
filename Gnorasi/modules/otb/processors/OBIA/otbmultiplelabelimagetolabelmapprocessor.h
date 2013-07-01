#ifndef OTBMULTIPLELABELIMAGETOLABELMAPPROCESSOR_H
#define OTBMULTIPLELABELIMAGETOLABELMAPPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "../../ports/otblabelimageport.h"
#include "../../ports/otblistlabelmapport.h"
#include "otbImage.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"

namespace voreen {


class OtbMultipleLabelImageToLabelMapProcessor : public Processor
{
public:
    OtbMultipleLabelImageToLabelMapProcessor();
    virtual ~OtbMultipleLabelImageToLabelMapProcessor();
    virtual Processor* create() const;

    virtual std::string getCategory() const { return "OBIA"; }
    virtual std::string getClassName() const { return "Multiple Label Image to Object Map List "; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;
    virtual bool isReady() const;
    virtual bool isEndProcessor() const;

    typedef unsigned long           LabelType;
    typedef otb::Image<LabelType, 2> LabeledImageType;
    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>
                                LabelMapFilterType;
    LabelMapFilterType::Pointer labelMapFilter;


protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    OTBLabelImagePort inPort_;
    OTBListLabelMapPort outPort_;

    static const std::string loggerCat_; ///< category used in logging

};

}

#endif // OTBMULTIPLELABELIMAGETOLABELMAPPROCESSOR_H

/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef OTBSOMMAPREADERPROCESSOR_H
#define OTBSOMMAPREADERPROCESSOR_H


#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/floatproperty.h"


#include "../../ports/otbvectorimageport.h"
#include "../../ports/otbimageport.h"
#include "../../ports/otbsommapport.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"

#include "itkVariableLengthVector.h"
#include "itkVectorExpandImageFilter.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"
#include "itkExpandImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbPrintableImageFilter.h"
#include "itkEuclideanDistance.h"
#include "itkListSample.h"


namespace voreen {

/**
 * Loads an SOM Map image file from disk and supplies the SmartPointer to it.
 */

class OTBSOMMapReaderProcessor : public Processor {
public:
    OTBSOMMapReaderProcessor();
    virtual ~OTBSOMMapReaderProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "SOM Map Image Reader"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    typedef double                          InputPixelType;
//    typedef otb::VectorImage<InputPixelType, 2>  VectorImageType;
//    typedef VectorImageType::PixelType      VectorType;

    typedef itk::VariableLengthVector<InputPixelType> PixelType;
    typedef itk::Statistics::EuclideanDistance<PixelType>  DistanceType;


    typedef otb::SOMMap<PixelType, DistanceType, 2> MapType;
    typedef MapType*    MapTypePointer;

    typedef otb::ImageFileReader<MapType> ReaderType;
    ReaderType::Pointer reader;
        /**
     * Loads the image specified by filename.
     *
     * @note The processor must have been initialized
     *       before calling this function.
     *
     * @param filename the image to load
     */
    void loadImage();

     /**
     * Returns the currently assigned image pointer.
     */
    MapTypePointer getMap();

    //virtual bool usesExpensiveComputation() const { return true; }
    virtual bool isEndProcessor() const;
    virtual bool isReady() const;
    virtual std::string getProcessorInfo() const;



protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:
    void setSOMMapPortData();
    void readData();
    void updateView();
    bool hasImage;
//    void setSingleBandData();

    /**
     * Clears the loaded image. Processor needs to be initialized when calling this function.
     */
    void clearImage();

    OTBSOMMapPort SOMMapOutport_;

    FileDialogProperty imageFile_;  ///< Path of the loaded image file.
    ButtonProperty clearImage_;      ///< Executes clearImage().

    MapTypePointer pDataOut_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // VRN_OTBSOMMAPREADERPROCESSOR_H

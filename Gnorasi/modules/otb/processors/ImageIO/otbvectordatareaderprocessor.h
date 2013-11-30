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

#ifndef OTBVECTORDATAREADERPROCESSOR_H
#define OTBVECTORDATAREADERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../../ports/otbvectordataport.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorData.h"

namespace voreen {

class OTBVectorDataReaderProcessor : public Processor {
public:
    OTBVectorDataReaderProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Vector Data File Reader"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;

    typedef otb::VectorData<double, 2> VectorDataType;
    VectorDataType::Pointer outVectorData;

    typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
    VectorDataFileReaderType::Pointer reader;

    virtual bool isReady() const;

    void loadVectorData();
    void clearVectorData();

protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:

    bool hasFileName;

    OTBVectorDataPort outPort_;

    FileDialogProperty VectorDataFile_;  ///< Path of the saved image file.
    ButtonProperty clearFile_;           ///< Executes clearImage().

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBVECTORDATAREADERPROCESSOR_H

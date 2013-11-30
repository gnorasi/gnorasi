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

#ifndef OTBVECTORDATAWRITERPROCESSOR_H
#define OTBVECTORDATAWRITERPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../../ports/otbvectordataport.h"
#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"

namespace voreen {

class OTBVectorDataWriterProcessor : public Processor {
public:
    OTBVectorDataWriterProcessor();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "Vector Data File Writer"; }
    virtual std::string getCategory() const  { return "Image IO"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; } //STABLE, TESTING, EXPERIMENTAL
    virtual std::string getProcessorInfo() const;

    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

    typedef otb::VectorData<double,2> VectorDataType;
    typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;
    VectorDataWriterType::Pointer writer;

     /**
     * Saves the vector data.
     *
     * @note The processor must have been initialized
     *       before calling this function.
     */
    void saveVectorData();

    /**
     * Clears the loaded vector data. Processor needs to be initialized when calling this function.
     */
    void clearVectorData();


protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    virtual void process();
    virtual void initialize() throw (VoreenException);
    virtual void deinitialize() throw (VoreenException);

private:
    bool hasVectorData;

    OTBVectorDataPort inPort_;
    FileDialogProperty vectorDataFile_;     ///< Path of the saved vector data file.
    ButtonProperty clearVectorData_;        ///< Executes clearVectorData().
    ButtonProperty saveVectorDataButton_;   ///< saves the vector data.

    static const std::string loggerCat_;    ///< category used in logging
};

} // namespace

#endif // OTBVECTORDATAWRITERPROCESSOR_H

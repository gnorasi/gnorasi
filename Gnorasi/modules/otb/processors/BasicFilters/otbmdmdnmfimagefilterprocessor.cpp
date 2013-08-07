/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#include "otbmdmdnmfimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBMDMDNMFImageFilterProcessor::loggerCat_("voreen.OTBMDMDNMFImageFilterProcessor");

OTBMDMDNMFImageFilterProcessor::OTBMDMDNMFImageFilterProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "IN MultiBand Image", 0),
      outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)
{
      addProperty(enableSwitch_);
      addPort(inPort_);
      addPort(outPort_);

      filter = FilterType::New();
      endMember2Matrix = VectorImageToMatrixImageFilterType::New();
}

Processor* OTBMDMDNMFImageFilterProcessor::create() const {
    return new OTBMDMDNMFImageFilterProcessor();
}

OTBMDMDNMFImageFilterProcessor::~OTBMDMDNMFImageFilterProcessor() {

}

void OTBMDMDNMFImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBMDMDNMFImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBMDMDNMFImageFilterProcessor::getProcessorInfo() const {
    return "MDMDNMF Image Filter";
}

void OTBMDMDNMFImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    try
    {

        endMember2Matrix->SetInput(inPort_.getData());
        endMember2Matrix->Update();
//        LINFO("Endmembers matrix: " << endMember2Matrix->GetMatrix());

        filter->SetInput(inPort_.getData());
        filter->SetEndmembersMatrix(endMember2Matrix->GetMatrix());

//        filter->SetAbortGenerateData(false);
//        filter->SetCritStopValue(10.0);
//        filter->SetDelt(1.0);
//        filter->SetLambdD(10.0);
//        filter->SetLambdS(1.0);
//        filter->SetMaxIter(3);
//        filter->SetNumberOfThreads(1);
//        filter->SetReferenceCount(2);
//        filter->SetReleaseDataBeforeUpdateFlag(false);
//        filter->SetReleaseDataFlag(false);

        /// FIXME: Update() on MDMDNMF crashes GNORASIS on module connection
        /// bypassing Update() gets distorted output
        filter->Update();
        filter->UpdateLargestPossibleRegion();
        outPort_.setData(filter->GetOutput());

        LINFO("MDMDNMF Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in MDMDNMF Image Filter");
        return;
    }

}

void OTBMDMDNMFImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace


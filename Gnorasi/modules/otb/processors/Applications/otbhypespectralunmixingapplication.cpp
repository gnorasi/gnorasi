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

#include "otbhypespectralunmixingapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBHypespectralUnmixingApplication::loggerCat_("voreen.OTBHypespectralUnmixingApplication");

OTBHypespectralUnmixingApplication::OTBHypespectralUnmixingApplication()
    :OTBImageFilterProcessor(),
      filter_("selectFilter", "Unmixing algorithm"),
      inPort_(Port::INPORT, "IN Vector Image", 0),
      endmembersInPort_(Port::INPORT, "Endmembers Multispectral Image", 0),
      outPort_(Port::OUTPORT, "OUT Vector Image", 0)
{
    addProperty(enableSwitch_);
    addProperty(filter_);
    addPort(inPort_);
    addPort(endmembersInPort_);
    addPort(outPort_);

    filter_.addOption("blank", "[Select filter:]");
    filter_.addOption("UCLS", "Unconstrained Least Square (UCLS)");
    filter_.addOption("NCLS", "Non-negative constrained Least Square (NCLS)");
    filter_.addOption("ISRA", "Image Space Reconstruction Algorithm (ISRA)");
    filter_.addOption("MDMDNMF", "Minimum Dispertion Constrained Non Negative Matrix Factorization (MDMD-NMF)");

    UCLSunmixer = UCLSUnmixingFilterType::New();
    ISRAunmixer = ISRAUnmixingFilterType::New();
    NCLSunmixer = NCLSUnmixingFilterType::New();
    MDMDNMFunmixer = MDMDNMFUnmixingFilterType::New();
    endMember2Matrix = VectorImageToMatrixImageFilterType::New();
}

Processor* OTBHypespectralUnmixingApplication::create() const {
    return new OTBHypespectralUnmixingApplication();
}

OTBHypespectralUnmixingApplication::~OTBHypespectralUnmixingApplication() {

}

void OTBHypespectralUnmixingApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBHypespectralUnmixingApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBHypespectralUnmixingApplication::getProcessorInfo() const {
    return "Hypespectral Unmixing Application";
}

void OTBHypespectralUnmixingApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        endMember2Matrix->SetInput(endmembersInPort_.getData());
        endMember2Matrix->Update();

        LINFO("Endmembers matrix: ");
        LINFO(endMember2Matrix->GetMatrix());

        if (filter_.get()=="UCLS")
        {
            LINFO("UCLS Unmixing");

            UCLSunmixer->SetInput(inPort_.getData());
            UCLSunmixer->SetMatrix(endMember2Matrix->GetMatrix());
            //              UCLSunmixer->SetNumberOfThreads(1); // FIXME : currently buggy
            UCLSunmixer->Update();
            outPort_.setData(UCLSunmixer->GetOutput());
        }
        else if (filter_.get()=="ISRA")
        {
            LINFO("ISRA Unmixing");

            ISRAunmixer->SetInput(inPort_.getData());
            ISRAunmixer->SetEndmembersMatrix(endMember2Matrix->GetMatrix());
            ISRAunmixer->Update();
            outPort_.setData(ISRAunmixer->GetOutput());

        }
        else if (filter_.get()=="NCLS")
        {
            LINFO("NCLS Unmixing");

            NCLSunmixer->SetInput(inPort_.getData());
            NCLSunmixer->SetEndmembersMatrix(endMember2Matrix->GetMatrix());
            NCLSunmixer->Update();
            outPort_.setData(NCLSunmixer->GetOutput());

        }
        else if (filter_.get()=="MDMDNMF")
        {
            LINFO("MDMD-NMF Unmixing");

            MDMDNMFunmixer->SetInput(inPort_.getData());
            MDMDNMFunmixer->SetEndmembersMatrix(endMember2Matrix->GetMatrix());
            MDMDNMFunmixer->Update();
            outPort_.setData(MDMDNMFunmixer->GetOutput());

        }
        else if (filter_.get()=="blank")
        {
            LERROR("No unmixing filter selected!");
        }

        LINFO("Hypespectral Unmixing Application Connected");

    }
    catch (int e)
    {
        LERROR("Error in Hypespectral Unmixing Application");
        return;
    }

}

void OTBHypespectralUnmixingApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace

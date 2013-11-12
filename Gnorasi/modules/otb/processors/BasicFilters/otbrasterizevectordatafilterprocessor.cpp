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

#include "otbrasterizevectordatafilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBRasterizeVectorDataFilterProcessor::loggerCat_("voreen.OTBRasterizeVectorDataFilterProcessor");

OTBRasterizeVectorDataFilterProcessor::OTBRasterizeVectorDataFilterProcessor()
    :OTBImageFilterProcessor(),
      useAddColor_("useAddColor_", "Enable Add Color:", false),
      burnValuePix_("burnValuePix_", "Burn Value Pix:", 1, 1, 10),
      inVectorDataPort_(Port::INPORT, "Vector Data Inport",0),
      inPort_(Port::INPORT, "OTB Image Inport",0),
      outPort_(Port::OUTPORT, "OTB Image Outport",0)
{
    addProperty(enableSwitch_);
    addProperty(useAddColor_);
    addProperty(burnValuePix_);

    addPort(inPort_);
    addPort(inVectorDataPort_);
    addPort(outPort_);

    burnValuePix_.setVisible(false);
    useAddColor_.onChange(CallMemberAction<OTBRasterizeVectorDataFilterProcessor>(this, &OTBRasterizeVectorDataFilterProcessor::update));

    filter = RasterizeVectorDataFilterType::New();
}

Processor* OTBRasterizeVectorDataFilterProcessor::create() const {
    return new OTBRasterizeVectorDataFilterProcessor();
}

OTBRasterizeVectorDataFilterProcessor::~OTBRasterizeVectorDataFilterProcessor() {

}

void OTBRasterizeVectorDataFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBRasterizeVectorDataFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBRasterizeVectorDataFilterProcessor::getProcessorInfo() const {
    return "Rasterize Vector Data Filter Processor";
}

void OTBRasterizeVectorDataFilterProcessor::update() {
    if (useAddColor_.get()) {
        burnValuePix_.setVisible(true);
    } else {
        burnValuePix_.setVisible(false);
    }

}

void OTBRasterizeVectorDataFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Rasterize Vector Data Filter Enabled!");

    try
    {
        int nbBands = inPort_.getData()->GetNumberOfComponentsPerPixel();
        LINFO("Number of bands detected: " << nbBands);

        filter->SetInput(inPort_.getData());
        filter->AddVectorData(inVectorDataPort_.getData());

        if (useAddColor_.get()) {
            int burnValue = burnValuePix_.get();
            LINFO("AddColor enabled! [" << burnValue << "]");

            if (burnValue>nbBands) {
                LWARNING("Number of bands detected: " << nbBands);
                LWARNING(burnValue << " band does not exist.");
            }
            filter->AddColor(burnValue);
        }

        filter->Update();
        outPort_.setData(filter->GetOutput());

        LINFO("Rasterize Vector Data Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Rasterize Vector Data Filter!");
        return;
    }
}

void OTBRasterizeVectorDataFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace


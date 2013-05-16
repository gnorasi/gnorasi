/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#include "otbmorphologicalwatershedimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBMorphologicalWatershedImageFilterProcessor::loggerCat_("voreen.OTBMorphologicalWatershedImageFilterProcessor");

OTBMorphologicalWatershedImageFilterProcessor::OTBMorphologicalWatershedImageFilterProcessor()
    :OTBImageFilterProcessor(),
      fullyConnected_("Face connectivity or by face+edge+vertex connecticity.", "Set Fully Connected:", false),
      markWatershedLine_("Watershed pixel marked or not.", "Set Mark Watershed Line:", true),
      waterShedLevel_("waterShedLevel", "Level:", 0.005f, 0.0f, 1.0f),
      inPort_(Port::INPORT, "IN OTB Image Port",0),
      outPort_(Port::OUTPORT, "OUT Label Image Port",0)
{
    addProperty(enableSwitch_);
    addProperty(fullyConnected_);
    addProperty(markWatershedLine_);
    addProperty(waterShedLevel_);
    addPort(inPort_);
    addPort(outPort_);

    filter = MorphologicalWatershedType::New();
}

Processor* OTBMorphologicalWatershedImageFilterProcessor::create() const {
    return new OTBMorphologicalWatershedImageFilterProcessor();
}

OTBMorphologicalWatershedImageFilterProcessor::~OTBMorphologicalWatershedImageFilterProcessor() {

}

void OTBMorphologicalWatershedImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBMorphologicalWatershedImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBMorphologicalWatershedImageFilterProcessor::getProcessorInfo() const {
    return "Grayscale Connected Closing Image Filter Processor";
}

void OTBMorphologicalWatershedImageFilterProcessor::process() {

    //LINFO("Morphological Watershed Image Filter Enabled!");
    //check bypass switch
//    if (!enableSwitch_.get()) {
//        bypass(&inPort_, &outPort_);
//        return;
//    }

    try
    {
        filter->SetLevel(waterShedLevel_.get());
        filter->SetInput(inPort_.getData());
        filter->SetFullyConnected(fullyConnected_.get());
        //filter->SetMarkWateshedLine(markWatershedLine_.get());
        //filter->SetMarkWateshedLine(true);
        filter->Update();

        //outPort_.setData(filter->GetOutput());

        LINFO("Morphological Watershed Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Morphological Watershed Image Filter");
        return;
    }


}

//void OTBMorphologicalWatershedImageFilterProcessor::bypass(OTBImagePort *inport, OTBLabelImagePort *outport) {

//    outport->setData(inport->getData());
//}
}   // namespace



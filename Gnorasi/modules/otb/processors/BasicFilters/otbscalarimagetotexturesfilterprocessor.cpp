/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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

#include "otbscalarimagetotexturesfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBScalarImageToTexturesFilterProcessor::loggerCat_("voreen.OTBScalarImageToTexturesFilterProcessor");

OTBScalarImageToTexturesFilterProcessor::OTBScalarImageToTexturesFilterProcessor()
    :OTBImageFilterProcessor(),
    radius_("radiusWindowValue", "Radius Window/Neighborhood X*X", 3, 3, 25),
    offSetX_("OffsetX", "Offset Array used in X", 3, 0, 4096),
    offSetY_("OffsetY", "Offset Array used in Y", 3, 0, 4096),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(radius_);
    addProperty(offSetX_);
    addProperty(offSetY_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
}

Processor* OTBScalarImageToTexturesFilterProcessor::create() const {
return new OTBScalarImageToTexturesFilterProcessor();
}

OTBScalarImageToTexturesFilterProcessor::~OTBScalarImageToTexturesFilterProcessor() {

}

void OTBScalarImageToTexturesFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBScalarImageToTexturesFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBScalarImageToTexturesFilterProcessor::getProcessorInfo() const {

return "ScalarImageToAdvancedTexturesFilter Processor";
}

void OTBScalarImageToTexturesFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    //The texture filters takes at least 2 parameters: the radius of the neighborhood on which the texture will be computed and the offset used. Texture features are bivariate statistics, that is, they are computed using pair of pixels. Each texture feature is defined for an offset defining the pixel pair.
    //The radius parameter can be passed to the filter as a scalar parameter if the neighborhood is square, or as SizeType in any case.
    //The offset is always an array of N values, where N is the number of dimensions of the image.
    //The textures filter will automatically derive the optimal bin size for co-occurences histogram, but they need to know the input image minimum and maximum. These values can be set like this :
    //texturesFilter->SetInputImageMinimum(0);
    //texturesFilter->SetInputImageMaximum(255);
    //To tune co-occurence histogram resolution, you can use the SetNumberOfBinsPerAxis() method.
    try
    {
        filter->SetInput(inPort_.getData());

        FilterType::SizeType Radius;
        Radius[0] = radius_.get();
        Radius[1] = radius_.get();
        filter->SetRadius(Radius);


        OffsetType offset;
        offset[0] = offSetX_.get();
        offset[1] = offSetY_.get();

        filter->SetOffset(offset);

        //CHECK TO DO HERE GET Minimum & Maximum
        //automatically from the input image
        filter->SetInputImageMinimum(0);
        filter->SetInputImageMaximum(255);

        filter->SetInput(inPort_.getData());

        outPort_.setData(filter->GetInertiaOutput());
    }
    catch (int e)
    {
        LERROR("Problem with ScalarImageToTextures Filter!");
    return;
    }
}

} //namespace





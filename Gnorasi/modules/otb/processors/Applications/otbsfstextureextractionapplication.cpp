/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#include "otbsfstextureextractionapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBSFSTextureExtractionApplication::loggerCat_("voreen.OTBSFSTextureExtractionApplication");

OTBSFSTextureExtractionApplication::OTBSFSTextureExtractionApplication()
    :OTBImageFilterProcessor(),
      channel_("The selected channel index", "Selected Channel", 1),
      spethre_("Spectral Threshold", "Spectral Threshold", 50.0f, 1.0f, 100.f),
      spathre_("Spatial Threshold", "Spatial Threshold", 100, 1, 500),
      nbdir_("Number of Direction", "Number of Direction", 20, 1, 200),
      alpha_("Alpha", "Alpha", 1.0f, 0.0f, 10.f),
      ratioMaxCons_("Ratio Maximum Consideration Number", "Ratio Maximum Consideration Number", 5, 1, 10),
      inPort_(Port::INPORT, "IN Vector Image", 0),
      outPort_(Port::OUTPORT, "OUT Vector Image", 0)

{

      addProperty(enableSwitch_);
      addProperty(channel_);
      addProperty(spethre_);
      addProperty(spathre_);
      addProperty(nbdir_);
      addProperty(alpha_);
      addProperty(ratioMaxCons_);
      addPort(inPort_);
      addPort(outPort_);

      extractor = ExtractorFilterType::New();
      TexFilter = SFSTexturesFilterType::New();
      ImageListFilter = ImageListType::New();
      Concatener = ImageListToVectorImageFilterType::New();
}

Processor* OTBSFSTextureExtractionApplication::create() const {
    return new OTBSFSTextureExtractionApplication();
}

OTBSFSTextureExtractionApplication::~OTBSFSTextureExtractionApplication() {

}

void OTBSFSTextureExtractionApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBSFSTextureExtractionApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBSFSTextureExtractionApplication::getProcessorInfo() const {
    return "SFS Texture Extraction Application";
}

void OTBSFSTextureExtractionApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        extractor->SetInput(inPort_.getData());
        extractor->SetStartX(inPort_.getData()->GetLargestPossibleRegion().GetIndex(0));
        extractor->SetStartY(inPort_.getData()->GetLargestPossibleRegion().GetIndex(1));
        extractor->SetSizeX(inPort_.getData()->GetLargestPossibleRegion().GetSize(0));
        extractor->SetSizeY(inPort_.getData()->GetLargestPossibleRegion().GetSize(1));
        extractor->SetChannel(channel_.get());
        extractor->UpdateOutputInformation();

        TexFilter->SetInput(const_cast<ImageType*>(extractor->GetOutput()));
        TexFilter->SetSpectralThreshold(spethre_.get());
        TexFilter->SetSpatialThreshold(spathre_.get());
        TexFilter->SetNumberOfDirections(nbdir_.get());
        TexFilter->SetAlpha(alpha_.get());
        TexFilter->SetRatioMaxConsiderationNumber(ratioMaxCons_.get());
        TexFilter->SetFeatureStatus(SFSTexturesFilterType::LENGTH, true);
        TexFilter->SetFeatureStatus(SFSTexturesFilterType::WIDTH, true);
        TexFilter->SetFeatureStatus(SFSTexturesFilterType::PSI, true);
        TexFilter->SetFeatureStatus(SFSTexturesFilterType::WMEAN, true);
        TexFilter->SetFeatureStatus(SFSTexturesFilterType::RATIO, true);
        TexFilter->SetFeatureStatus(SFSTexturesFilterType::SD, true);
        TexFilter->UpdateOutputInformation();

        ImageListFilter->PushBack(TexFilter->GetLengthOutput());
        ImageListFilter->PushBack(TexFilter->GetWidthOutput());
        ImageListFilter->PushBack(TexFilter->GetPSIOutput());
        ImageListFilter->PushBack(TexFilter->GetWMeanOutput());
        ImageListFilter->PushBack(TexFilter->GetRatioOutput());
        ImageListFilter->PushBack(TexFilter->GetSDOutput());

        Concatener->SetInput(ImageListFilter);
        outPort_.setData(Concatener->GetOutput());

        LINFO("SFS Texture Extraction Application Connected");
    }
    catch (int e)
    {
        LERROR("Error in SFS Texture Extraction Application");
        return;
    }

}

void OTBSFSTextureExtractionApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace

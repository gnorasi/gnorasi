/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "volumecollectionsave.h"
#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/utils/stringconversion.h"
#include "tgt/filesystem.h"

namespace voreen {

const std::string VolumeCollectionSave::loggerCat_("voreen.base.VolumeCollectionSave");

VolumeCollectionSave::VolumeCollectionSave()
    : VolumeProcessor(),
    inport_(Port::INPORT, "volumehandle.input", false, Processor::INVALID_RESULT),
    outputDirectory_("outputDirectory", "Directory", "Select directory...",
        "", "", FileDialogProperty::DIRECTORY, Processor::VALID),
    continousSave_("continousSave", "Save continuously", false, Processor::VALID),
    saveButton_("save", "Save"),
    volumeSerializerPopulator_(0)
{

    outputDirectory_.onChange(CallMemberAction<VolumeCollectionSave>(this, &VolumeCollectionSave::saveCollection));
    addProperty(outputDirectory_);

    addProperty(continousSave_);

    saveButton_.onChange(CallMemberAction<VolumeCollectionSave>(this, &VolumeCollectionSave::saveCollection));
    addProperty(saveButton_);

    addPort(inport_);
}

VolumeCollectionSave::~VolumeCollectionSave() {
    delete volumeSerializerPopulator_;
}

Processor* VolumeCollectionSave::create() const {
    return new VolumeCollectionSave();
}

void VolumeCollectionSave::initialize() throw (tgt::Exception) {

    VolumeProcessor::initialize();

    tgtAssert(!volumeSerializerPopulator_, "serializer populator already created");
    volumeSerializerPopulator_ = new VolumeSerializerPopulator();
}

void VolumeCollectionSave::process() {
    if (inport_.hasChanged() && continousSave_.get())
        saveCollection();
}

void VolumeCollectionSave::saveCollection() {
    if (!isInitialized())
        return;

    tgtAssert(volumeSerializerPopulator_, "no populator");

    if (!inport_.hasData() || inport_.getData()->empty())
        return;
    if (outputDirectory_.get() == "")
        return;

    std::string directory = outputDirectory_.get();
    const VolumeSerializer* serializer = volumeSerializerPopulator_->getVolumeSerializer();
    const VolumeCollection* inputCollection = inport_.getData();
    tgtAssert(inputCollection, "no collection");
    try {
        for (size_t i=0; i<inputCollection->size(); i++) {    
            std::string volFilename = "";
            if(dynamic_cast<VolumeHandle*>(inputCollection->at(i)))
                volFilename = static_cast<VolumeHandle*>(inputCollection->at(i))->getOrigin().getFilename();
            if (volFilename == "") {
                volFilename = "volume";
                if (i < 10)
                    volFilename += "00";
                else if (i < 100)
                    volFilename += "0";
                volFilename += itos(i) + ".dat";
            }
            else 
                volFilename = tgt::FileSystem::fileName(volFilename);
            std::string outputFilename = directory + "/" + volFilename;
            serializer->write(outputFilename, inputCollection->at(i));
        }
    } 
    catch (tgt::FileException e) {
        LERROR("Failed to save volume collection: " << e.what());
        //basename_.set("");
    }
}

}   // namespace

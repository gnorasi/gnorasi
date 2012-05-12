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

#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumehash.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/io/progressbar.h"
#include "voreen/core/datastructures/volume/modality.h"
#include "voreen/core/io/serialization/meta/primitivemetadata.h"

#include "tgt/filesystem.h"

#include <algorithm>
#include <string>
#include <cctype>

using std::string;
using tgt::vec3;
using tgt::mat4;

namespace {
int lower_case(int c) {
    return tolower(c);
}
}
namespace voreen {

const std::string VolumeHandleBase::loggerCat_("voreen.VolumeHandleBase");
const std::string VolumeHandle::loggerCat_("voreen.VolumeHandle");
const std::string VolumeOrigin::loggerCat_("voreen.VolumeOrigin");

VolumeOrigin::VolumeOrigin()
    : url_("")
{}

VolumeOrigin::VolumeOrigin(const VolumeOrigin& rhs)
    : Serializable()
{
    url_ = rhs.getURL();
    metaDataContainer_ = rhs.getMetaDataContainer();
}

VolumeOrigin::VolumeOrigin(const std::string& filepath)
    : url_(filepath)
{
    cleanupURL();
}

VolumeOrigin::~VolumeOrigin() {
}

VolumeOrigin& VolumeOrigin::operator=(const VolumeOrigin& rhs) {
    url_ = rhs.getURL();
    metaDataContainer_ = rhs.getMetaDataContainer();
    return *this;
}

bool VolumeOrigin::operator==(const VolumeOrigin& rhs) const {
    //return (tgt::FileSystem::comparePaths(getPath(), rhs.getPath()));
    return (getURL() == rhs.getURL());
}


VolumeOrigin::VolumeOrigin(const std::string& protocol, const std::string& filepath, const std::string& searchString) {

    // TODO: validate parameters

    url_ = filepath;

    if (!protocol.empty())
        url_.insert(0, protocol + "://");

    if (!searchString.empty())
        url_.append("?" + searchString);

    cleanupURL();
}

void VolumeOrigin::serialize(XmlSerializer& s) const {
   std::string basePath = tgt::FileSystem::dirName(s.getDocumentPath());
   if (!basePath.empty()) {
        VolumeSerializerPopulator serializerPopulator;
        VolumeOrigin originConv;
        try {
            originConv = serializerPopulator.getVolumeSerializer()->convertOriginToRelativePath(*this, basePath);
        }
        catch (tgt::UnsupportedFormatException& e) {
            throw SerializationException(std::string(e.what()));
        }
        s.serialize("url", originConv.getURL());
    }
    else {
        s.serialize("url", getURL());
    }
}

void VolumeOrigin::deserialize(XmlDeserializer& s) {
    bool found = false;
    try {
        s.deserialize("url", url_);
        found = true;
    }
    catch (XmlSerializationNoSuchDataException&) {
        s.removeLastError();
    }
    // look for alternative attribute name 'filename' (legacy)
    if (!found) {
        try {
            s.deserialize("filename", url_);
            found = true;
        }
        catch (XmlSerializationNoSuchDataException&) {
            s.removeLastError();
        }
    }
    if (!found) 
        throw XmlSerializationNoSuchDataException("VolumeOrigin: neither attribute 'filename' nor attribute 'url' found");

    std::string basePath = tgt::FileSystem::dirName(s.getDocumentPath());
    if (!basePath.empty()) {
        VolumeSerializerPopulator serializerPopulator;

        VolumeOrigin originConv;
        try {
            originConv = serializerPopulator.getVolumeSerializer()->convertOriginToAbsolutePath(*this, basePath);
        }
        catch (tgt::UnsupportedFormatException& e) {
            throw SerializationException(std::string(e.what()));
        }
        url_ = originConv.getURL();
    }

    cleanupURL();
}

const std::string& VolumeOrigin::getURL() const {
    return url_;
}

std::string VolumeOrigin::getPath() const {

    if (url_.empty()) {
        LWARNING("Empty origin URL");
        return "";
    }

    std::string fullPath; // path + searchstring

    // remove protocol portion
    string::size_type sep_pos = url_.find("://");
    if (sep_pos == std::string::npos) {
        // URL does not contain protocol specifier
        fullPath = url_;
    }
    else {
        // return substring after protocol separator
        if (url_.size() < sep_pos + 1) {
            LWARNING("Origin URL does not contain a path");
            return "";
        }
        fullPath = url_.substr(sep_pos + 3);
    }

    // remove search string
    sep_pos = fullPath.find("?");
    if (sep_pos == std::string::npos) {
        // URL does not contain search string
        return fullPath;
    }
    else {
        // return substring before search string separator
        return fullPath.substr(0, sep_pos);
    }

}

std::string VolumeOrigin::getFilename() const {

    if (url_.empty()) {
        LWARNING("Empty origin URL");
        return "";
    }

    return tgt::FileSystem::fileName(getPath());
}

std::string VolumeOrigin::getSearchString() const {

    if (url_.empty()) {
        LWARNING("Empty origin URL");
        return "";
    }

    // find search string separator
    string::size_type sep_pos = url_.find("?");
    if (sep_pos == std::string::npos) {
        // URL does not contain a search string
        return "";
    }
    else {
        // return substring after search string separator
        return url_.substr(sep_pos + 1);
    }

}

std::string VolumeOrigin::getProtocol() const {

    string::size_type sep_pos = url_.find("://");
    if (sep_pos == std::string::npos) {
        // URL does not contain protocol specifier
        return "";
    }
    else {
        // return substring before protocol separator
        return url_.substr(0, sep_pos);
    }

}

void VolumeOrigin::cleanupURL() {
    // replace backslashes in path
    string::size_type posSearchString = url_.find("?");
    string::size_type pos = url_.find("\\");
    while (pos < posSearchString && pos != string::npos) {
        url_[pos] = '/';
        pos = url_.find("\\");
    }
}

void VolumeOrigin::addSearchParameter(const std::string& key, const std::string& value) {

    if (key.empty() || value.empty()) {
        LWARNING("Search key or value empty.");
        return;
    }

    if (url_.empty()) {
        LWARNING("Empty origin URL");
        return;
    }

    // start search string, if not present
    if (getSearchString().empty())
        url_.append("?");
    else
        url_.append("&");

    // append search parameter
    url_.append(key);
    url_.append("=");
    url_.append(value);

}

std::string VolumeOrigin::getSearchParameter(const std::string& k, bool caseSensitive) const {

    std::string searchString = getSearchString();
    std::string key = k;
    if (searchString.empty())
        return "";

    if (!caseSensitive) {
        transform(searchString.begin(), searchString.end(), searchString.begin(), lower_case);
        transform(key.begin(), key.end(), key.begin(), lower_case);
    }

    // insert parameter separator at front of search string (eases the following search operation)
    searchString.insert(0, "&");

    // find key
    key.insert(0, "&");
    key.append("=");
    string::size_type param_pos = searchString.find(key);
    if (param_pos == std::string::npos) {
        // not found
        return "";
    }
    else {
        // found, detect position of next parameter in search string
        string::size_type next_pos = searchString.find("&", param_pos + key.size());
        if (next_pos == std::string::npos) {
            // parameter is the last one in search string
            return searchString.substr(param_pos + key.size());
        }
        else {
            // return substring till next parameter
            return searchString.substr(param_pos + key.size(), next_pos - param_pos - key.size());
        }
    }
}

MetaDataContainer& VolumeOrigin::getMetaDataContainer() {
    return metaDataContainer_;
}

const MetaDataContainer& VolumeOrigin::getMetaDataContainer() const {
    return metaDataContainer_;
}

// ----------------------------------------------------------------------------

VolumeHandleBase::~VolumeHandleBase() {
    notifyDelete();
    clearDerivedData();
}

VolumeHandle* VolumeHandleBase::clone() const throw (std::bad_alloc) {
    Volume* v = getRepresentation<Volume>()->clone();
    return new VolumeHandle(v, this);
}

std::string VolumeHandleBase::getHash() const {
    return getDerivedData<VolumeHash>()->getHash();
}

tgt::vec3 VolumeHandleBase::getCubeSize() const {
    vec3 cubeSize = vec3(getDimensions()) * getSpacing();
    return cubeSize;
}

vec3 VolumeHandleBase::getLLF() const {
    return getOffset();
}

vec3 VolumeHandleBase::getURB() const {
    return getOffset()+getCubeSize();
}

tgt::mat4 VolumeHandleBase::getVoxelToWorldMatrix() const {
    return getPhysicalToWorldMatrix() * getVoxelToPhysicalMatrix();
}

tgt::mat4 VolumeHandleBase::getWorldToVoxelMatrix() const {
    tgt::mat4 result = tgt::mat4::identity;
    getVoxelToWorldMatrix().invert(result);
    return result;
}

tgt::mat4 VolumeHandleBase::getWorldToTextureMatrix() const {
    tgt::mat4 result = tgt::mat4::identity;
    getTextureToWorldMatrix().invert(result);
    return result;
}

tgt::mat4 VolumeHandleBase::getTextureToWorldMatrix() const {
    return getVoxelToWorldMatrix() * getTextureToVoxelMatrix();
}

tgt::mat4 VolumeHandleBase::getVoxelToPhysicalMatrix() const {
    // 1. Multiply by spacing 2. Apply offset
    return tgt::mat4::createTranslation(getOffset()) * tgt::mat4::createScale(getSpacing());
}

tgt::mat4 VolumeHandleBase::getPhysicalToVoxelMatrix() const {
    return tgt::mat4::createScale(1.0f/getSpacing()) * tgt::mat4::createTranslation(-getOffset());
}

tgt::mat4 VolumeHandleBase::getPhysicalToWorldMatrix() const {
    return getMetaDataValue<Mat4MetaData>("Transformation", mat4::identity);
}

tgt::mat4 VolumeHandleBase::getWorldToPhysicalMatrix() const {
    tgt::mat4 result = tgt::mat4::identity;
    getPhysicalToWorldMatrix().invert(result);
    return result;
}

tgt::mat4 VolumeHandleBase::getTextureToPhysicalMatrix() const {
    return getVoxelToPhysicalMatrix() * getTextureToVoxelMatrix();
}

tgt::mat4 VolumeHandleBase::getPhysicalToTextureMatrix() const {
    return getVoxelToTextureMatrix() * getPhysicalToVoxelMatrix();
}

tgt::mat4 VolumeHandleBase::getTextureToVoxelMatrix() const {
    return tgt::mat4::createScale(getDimensions());
}

tgt::mat4 VolumeHandleBase::getVoxelToTextureMatrix() const {
    return tgt::mat4::createScale(1.0f/vec3(getDimensions()));
}

std::vector<tgt::vec3> VolumeHandleBase::getCubeVertices() const {
    std::vector<tgt::vec3> cubeVertices;
    vec3 llf = getLLF();
    vec3 urb = getURB();

    cubeVertices.push_back(vec3(llf.x, llf.y, urb.z));// llb 0
    cubeVertices.push_back(vec3(urb.x, llf.y, urb.z));// lrb 1
    cubeVertices.push_back(vec3(urb.x, urb.y, urb.z));// urb 2
    cubeVertices.push_back(vec3(llf.x, urb.y, urb.z));// ulb 3

    cubeVertices.push_back(vec3(llf.x, llf.y, llf.z));// llf 4
    cubeVertices.push_back(vec3(urb.x, llf.y, llf.z));// lrf 5
    cubeVertices.push_back(vec3(urb.x, urb.y, llf.z));// urf 6
    cubeVertices.push_back(vec3(llf.x, urb.y, llf.z));// ulf 7
    return cubeVertices;
}

MeshGeometry VolumeHandleBase::getBoundingBox(bool applyTransformation) const {
    MeshGeometry boundingBox = MeshGeometry::createCube(getLLF(), getURB(), tgt::vec3(0.f), tgt::vec3(1.f));
    if (applyTransformation)
        boundingBox.transform(getPhysicalToWorldMatrix());
    return boundingBox;
}

void VolumeHandleBase::clearDerivedData() {
    for (std::set<VolumeDerivedData*>::iterator it=derivedData_.begin(); it!=derivedData_.end(); ++it) {
        delete *it;
    }
    derivedData_.clear();
}

const VolumeOrigin& VolumeHandleBase::getOrigin() const {
    return origin_;
}

void VolumeHandleBase::setOrigin(const VolumeOrigin& origin) {
    origin_ = origin;
}

float VolumeHandleBase::getTimestep() const {
    return getMetaDataValue<FloatMetaData>("timestep", 0.0f);
}

size_t VolumeHandleBase::getNumChannels() const {
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->getNumChannels();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return 0;
    }
}

tgt::svec3 VolumeHandleBase::getDimensions() const {
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->getDimensions();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return tgt::svec3(0, 0, 0);
    }
}

tgt::svec3 VolumeHandleBase::getOriginalDimensions() const {
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->getOriginalDimensions();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return tgt::svec3(0, 0, 0);
    }
}

int VolumeHandleBase::getBitsStored() const {
    //const VolumeRepresentation* rep = getRepresentation(0);
    const Volume* rep = getRepresentation<Volume>();

    if (rep)
        return rep->getBitsStored();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return 0;
    }
}

size_t VolumeHandleBase::getNumVoxels() const {
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->getNumVoxels();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return 0;
    }
}

tgt::svec3 VolumeHandleBase::getBorderLLF() const {
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->getBorderLLF();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return tgt::svec3(0, 0, 0);
    }
}

tgt::svec3 VolumeHandleBase::getBorderURB() const{
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->getBorderURB();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return tgt::svec3(0, 0, 0);
    }
}

bool VolumeHandleBase::hasBorder() const{
    const VolumeRepresentation* rep = getRepresentation(0);
    if (rep)
        return rep->hasBorder();
    else {
        tgtAssert(false, "VolumeHandle has no representation!");
        return false;
    }
}

vec3 VolumeHandleBase::getSpacing() const {
    return getMetaDataValue<Vec3MetaData>("Spacing", vec3(1.0f));
}

RealWorldMapping VolumeHandleBase::getRealWorldMapping() const {
    return getMetaDataValue<RealWorldMappingMetaData>("RealWorldMapping", RealWorldMapping());
}

vec3 VolumeHandleBase::getOffset() const {
    return getMetaDataValue<Vec3MetaData>("Offset", vec3(0.0f));
}

Modality VolumeHandleBase::getModality() const {
    return Modality(getMetaDataValue<StringMetaData, std::string>("Modality", "unknown"));
}

void VolumeHandleBase::notifyDelete() {
    std::vector<VolumeHandleObserver*> observers = getObservers();
    for (size_t i=0; i<observers.size(); ++i)
        observers[i]->volumeHandleDelete(this);
}

void VolumeHandleBase::notifyReload() {
    std::vector<VolumeHandleObserver*> observers = getObservers();
    for (size_t i=0; i<observers.size(); ++i)
        observers[i]->volumeChange(this);
}

// ----------------------------------------------------------------------------

VolumeHandle::VolumeHandle(VolumeRepresentation* const volume, const tgt::vec3& spacing, const tgt::vec3& offset, const tgt::mat4& transformation)
{
    setSpacing(spacing);
    setOffset(offset);
    setPhysicalToWorldMatrix(transformation);
    addRepresentation(volume);
}

VolumeHandle::VolumeHandle(VolumeRepresentation* const volume, const VolumeHandleBase* vh) 
{
    std::vector<std::string> keys = vh->getMetaDataKeys();
    for(size_t i=0; i<keys.size(); i++) {
        const MetaDataBase* md = vh->getMetaData(keys[i]);
        if (md) {
            metaData_.addMetaData(keys[i], md->clone());
        }
    }
    addRepresentation(volume);
}

VolumeHandle::VolumeHandle(VolumeRepresentation* const volume, const MetaDataContainer* mdc) {
    std::vector<std::string> keys = mdc->getKeys();
    for(size_t i=0; i<keys.size(); i++) {
        const MetaDataBase* md = mdc->getMetaData(keys[i]);
        if (md) {
            metaData_.addMetaData(keys[i], md->clone());
        }
    }
    //metaData_ = *md;
    addRepresentation(volume);
}

VolumeHandle::VolumeHandle()
{
}

VolumeHandle::~VolumeHandle() {
    notifyDelete();
    deleteAllRepresentations();
}

void VolumeHandle::releaseVolumes() {
   representations_.clear();
}

void VolumeHandle::setVolume(Volume* const volume) {
    if (!volume) {
        LERROR("Tried to set null volume!");
        tgtAssert(false, "Tried to set null volume!");
        return;
    }

    if (VolumeHandleBase::hasRepresentation<Volume>()) {
        const Volume* v = VolumeHandleBase::getRepresentation<Volume>();

        if (v != volume) {
            if (v->getDimensions() != volume->getDimensions()) {
                LERROR("Tried to set volume with different dimensions!");
                tgtAssert(false, "Tried to set volume with different dimensions!");
                return;
            }

            deleteAllRepresentations();
            addRepresentation(volume);
        }
    }
    else {
        addRepresentation(volume);
        makeRepresentationExclusive<Volume>();
    }
}

bool VolumeHandle::reloadVolume() {

    ProgressBar* progressDialog = VoreenApplication::app()->createProgressDialog();
    if (progressDialog) {
        progressDialog->setTitle("Loading volume");
        progressDialog->setMessage("Loading volume ...");
    }
    VolumeSerializerPopulator populator(progressDialog);

    // try to load volume from origin
    VolumeHandleBase* handle = 0;
    try {
        handle = populator.getVolumeSerializer()->read(origin_);
    }
    catch (tgt::FileException& e) {
        LWARNING(e.what());
    }
    catch (std::bad_alloc&) {
        LWARNING("std::Error BAD ALLOCATION");
    }
    delete progressDialog;
    progressDialog = 0;

    if (!handle || !handle->getRepresentation<Volume>()) {
        delete handle;
        return false;
    }

    VolumeHandle* vh = static_cast<VolumeHandle*>(handle);
    if (VolumeHandleBase::hasRepresentation<Volume>()) {
        deleteAllRepresentations();
        
        addRepresentation(vh->getWritableRepresentation<Volume>());
        vh->releaseAllRepresentations();
        delete handle;
    }

    // inform observers
    notifyReload();
    return true;
}

#ifndef VRN_NO_OPENGL
//VolumeGL* VolumeHandle::getRepresentation<VolumeGL>() {
    //return const_cast<VolumeGL*>(getRepresentation<VolumeGL>());
//}
#endif // VRN_NO_OPENGL

void VolumeHandle::serialize(XmlSerializer& s) const {
    s.serialize("Origin", origin_);
    //metaData_.serialize(s);
}

void VolumeHandle::deserialize(XmlDeserializer& s) {
    // deserialize Origin
    try {
        s.deserialize("Origin", origin_);
    }
    catch (SerializationException& e) {
        s.addError(std::string(e.what()));
        LWARNING(std::string(e.what()));
        return;
    }

    // deserialize meta data
    //metaData_.deserialize(s);

    ProgressBar* progressDialog = VoreenApplication::app()->createProgressDialog();
    if (progressDialog) {
        progressDialog->setTitle("Loading volume");
        progressDialog->setMessage("Loading volume ...");
    }
    VolumeSerializerPopulator populator(progressDialog);

    // load volume from origin
    VolumeHandle* handle = 0;
    try {
        VolumeHandleBase* vhb = populator.getVolumeSerializer()->read(origin_);
        if (vhb) {
            handle = static_cast<VolumeHandle*>(vhb);
            // copy over loaded volume from temporary handle and free it
            if (handle) {
                setVolume(handle->getWritableRepresentation<Volume>());
                handle->releaseVolumes();

                metaData_ = handle->getMetaDataContainer();

                delete handle;
            }
        }
    }
    catch (tgt::FileException& e) {
        s.addError(e);
        LWARNING(e.what());
    }
    catch (std::bad_alloc& /*e*/) {
        s.addError("Unable to load " + origin_.getPath() + ": bad allocation");
        LWARNING("bad allocation when reading " << origin_.getPath() + " (URL: " + origin_.getURL() + ")");
    }
    delete progressDialog;
}

void VolumeHandle::setHash(const std::string& hash) const {
    addDerivedDataInternal<VolumeHash>(new VolumeHash(hash));
}

void VolumeHandle::setModality(Modality modality) {
    setMetaDataValue<StringMetaData>("Modality", modality.getName());
}

void VolumeHandle::setRealWorldMapping(RealWorldMapping rwm) {
    setMetaDataValue<RealWorldMappingMetaData>("RealWorldMapping", rwm);
}

const MetaDataContainer& VolumeHandle::getMetaDataContainer() const {
    return metaData_;
}

MetaDataContainer& VolumeHandle::getMetaDataContainer() {
    return metaData_;
}

void VolumeHandle::setTimestep(float timestep) {
    setMetaDataValue<FloatMetaData>("Timestep", timestep);
}

void VolumeHandle::setSpacing(const tgt::vec3 spacing) {
    setMetaDataValue<Vec3MetaData>("Spacing", spacing);
}

void VolumeHandle::setOffset(const tgt::vec3 offset) {
    setMetaDataValue<Vec3MetaData>("Offset", offset);
}

void VolumeHandle::setPhysicalToWorldMatrix(const tgt::mat4& transformationMatrix) {
    return setMetaDataValue<Mat4MetaData>("Transformation", transformationMatrix);
}

//---------------------------------------------------------------

void oldVolumePosition(VolumeHandle* vh) {
    //correct old spacing:
    vec3 sp = vh->getSpacing();
    vec3 cubeSize = sp * vec3(vh->getDimensions());

    float scale = 2.0f / max(cubeSize);
    sp *= scale;
    vh->setSpacing(sp);

    //set origin to center volume:
    cubeSize = sp * vec3(vh->getDimensions());
    vh->setOffset(-cubeSize/2.0f);
}

void centerVolume(VolumeHandle* vh) {
    vec3 sp = vh->getSpacing();
    vec3 cubeSize = sp * vec3(vh->getDimensions());

    //set origin to center volume:
    vh->setOffset(-cubeSize/2.0f);
}

} // namespace

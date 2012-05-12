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

#include "voreen/core/datastructures/volume/histogram.h"
#include "voreen/core/datastructures/volume/volumehandle.h"

#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"

namespace voreen {

//Histogram1D createHistogram1DFromVolume(const VolumeHandleBase* handle, int bucketCount) {
//    const Volume* vol = handle->getRepresentation<Volume>();
//    RealWorldMapping rwm = handle->getRealWorldMapping();
//
//    float min = vol->minValue();
//    float max = vol->maxValue();
//    min = rwm.normalizedToRealWorld(min);
//    max = rwm.normalizedToRealWorld(max);
//
//    Histogram1D h(min, max, bucketCount);
//
//    for(size_t i=0; i<vol->getNumVoxels(); i++) {
//        float v = vol->getVoxelFloat(i);
//        v = rwm.normalizedToRealWorld(v);
//
//        h.addSample(v);
//    }
//
//    return h;
//}

//-----------------------------------------------------------------------------

HistogramIntensity::HistogramIntensity() :
    VolumeDerivedData(),
    maxValue_(-1)
{}

HistogramIntensity::HistogramIntensity(const Volume* volume, int bucketCount) :
    VolumeDerivedData()
{
    tgtAssert(volume, "HistogramIntensity: No volume");
    tgtAssert(bucketCount > 0, "HistogramIntensity: Invalid bucket count");

    // Limit to 16 bit
    if (bucketCount > 65536)
        bucketCount = 65536;

    // set all buckets to zero
    histValues_.assign(bucketCount, 0);

    maxValue_ = 0;

    const Volume* currentVolume = volume;

    const VolumeUInt8* sourceDataset8Bit = dynamic_cast<const VolumeUInt8*>(currentVolume);
    if (sourceDataset8Bit) {
        int bucket;
        float m = (bucketCount - 1.f) / 255.f;

        for (size_t i=0; i < sourceDataset8Bit->getNumVoxels(); ++i) {
            bucket = static_cast<int>(floor(sourceDataset8Bit->voxel(i) * m));
            int value = histValues_[bucket] + 1;
            histValues_[bucket] = value;

            if (value > maxValue_)
                maxValue_ = value;
        }
    }

    const Volume4xUInt8* sourceDataset32Bit = dynamic_cast<const Volume4xUInt8*>(currentVolume);
    if (sourceDataset32Bit) {
        int bucket;
        float m = (bucketCount - 1.f) / 255.f;

        for (size_t i=0; i < sourceDataset32Bit->getNumVoxels(); ++i) {
            bucket = static_cast<int>(floor(sourceDataset32Bit->voxel(i)[3] * m));
            int value = histValues_[bucket] + 1;
            histValues_[bucket] = value;

            if (value > maxValue_)
                maxValue_ = value;
        }
    }

    const VolumeUInt16* sourceDataset16Bit = dynamic_cast<const VolumeUInt16*>(currentVolume);
    if (sourceDataset16Bit) {
        int bucket;
        float maxValue = (sourceDataset16Bit->getBitsStored() == 12) ? 4095.f : 65535.f;
        float m = (bucketCount - 1.f) / maxValue;

        for (size_t i=0; i < sourceDataset16Bit->getNumVoxels(); ++i) {
            bucket = static_cast<int>(floor(sourceDataset16Bit->voxel(i) * m));
            if (bucket < static_cast<int>(histValues_.size())) {
                int value = histValues_[bucket] + 1;
                histValues_[bucket] = value;

                if (value > maxValue_)
                    maxValue_ = value;
            }
        }
    }

    const VolumeFloat* sourceDatasetFloat = dynamic_cast<const VolumeFloat*>(currentVolume);
    if (sourceDatasetFloat) {
        float m = (bucketCount - 1.f);
        int count = static_cast<int>(histValues_.size());

        for (size_t i=0; i < sourceDatasetFloat->getNumVoxels(); ++i) {
            int bucket = static_cast<int>(floor(sourceDatasetFloat->voxel(i) * m));
            if (bucket < count && bucket >= 0) {
                int value = histValues_[bucket] + 1;
                histValues_[bucket] = value;

                if (value > maxValue_)
                    maxValue_ = value;
            }
        }
    }
}

HistogramIntensity::HistogramIntensity(const HistogramIntensity& h) {
    histValues_ = h.histValues_;
    maxValue_ = h.maxValue_;
}

VolumeDerivedData* HistogramIntensity::createFrom(const VolumeHandleBase* handle) const {
    tgtAssert(handle, "no volume handle");
    HistogramIntensity* histogram = new HistogramIntensity(handle->getRepresentation<Volume>(), 256);
    return histogram;
}

size_t HistogramIntensity::getBucketCount() const {
    return histValues_.size();
}

int HistogramIntensity::getValue(int i) const {
    return histValues_[i];
}

int HistogramIntensity::getValue(size_t i) const {
    return getValue(static_cast<int>(i));
}

int HistogramIntensity::getValue(float i) const {
    size_t bucketCount = histValues_.size();
    float m = (bucketCount - 1.f);
    int bucket = static_cast<int>(floor(i * m));
    return getValue(bucket);
}

float HistogramIntensity::getNormalized(int i) const {
    return (static_cast<float>(histValues_[i]) / static_cast<float>(maxValue_));
}

float HistogramIntensity::getNormalized(float i) const {
    size_t bucketCount = histValues_.size();
    float m = (bucketCount - 1.f);
    int bucket = static_cast<int>(floor(i * m));
    return getNormalized(bucket);
}

float HistogramIntensity::getLogNormalized(int i) const {
     return (logf(static_cast<float>(1+histValues_[i]) ) / log( static_cast<float>(1+maxValue_)));
}

float HistogramIntensity::getLogNormalized(float i) const {
    size_t bucketCount = histValues_.size();
    float m = (bucketCount - 1.f);
    int bucket = static_cast<int>(floor(i * m));
    return getLogNormalized(bucket);
}

void HistogramIntensity::serialize(XmlSerializer& s) const  {
    s.serialize("values", histValues_);
    s.serialize("maxValue", maxValue_);
}

void HistogramIntensity::deserialize(XmlDeserializer& s) {
    s.deserialize("values", histValues_);
    s.deserialize("maxValue", maxValue_);
}

//-----------------------------------------------------------------------------

HistogramIntensityGradient::HistogramIntensityGradient(const VolumeHandleBase* volumeHandleGrad, const VolumeHandleBase* volumeHandleIntensity,
                                                       int bucketCounti, int bucketCountg, bool scale)
    : scaleFactor_(1.f)
{
    const Volume* volumeGrad = volumeHandleGrad->getRepresentation<Volume>();
    const Volume* volumeIntensity = volumeHandleIntensity->getRepresentation<Volume>();

    if (dynamic_cast<const Volume3xUInt8*>(volumeGrad)) {
        calcHG(static_cast<const Volume3xUInt8*>(volumeGrad), volumeIntensity, bucketCounti, bucketCountg, scale);
    }
    else if (dynamic_cast<const Volume4xUInt8*>(volumeGrad)) {
        calcHG(static_cast<const Volume4xUInt8*>(volumeGrad), volumeIntensity, bucketCounti, bucketCountg, scale);
    }
    else if (dynamic_cast<const Volume3xUInt16*>(volumeGrad)) {
        calcHG(static_cast<const Volume3xUInt16*>(volumeGrad), volumeIntensity, bucketCounti, bucketCountg, scale);
    }
    else if (dynamic_cast<const Volume4xUInt16*>(volumeGrad)) {
        calcHG(static_cast<const Volume4xUInt16*>(volumeGrad), volumeIntensity, bucketCounti, bucketCountg, scale);
    }
    else {
        LERRORC("voreen.HistogramIntensityGradient",
                "HistogramIntensityGradient needs 24 or 32 bit DS as input!");
    }
}

HistogramIntensityGradient::HistogramIntensityGradient() :
    VolumeDerivedData(),
    maxValue_(-1),
    significantRangeIntensity_(-1),
    significantRangeGradient_(-1),
    scaleFactor_(-1.f)
{}

VolumeDerivedData* HistogramIntensityGradient::createFrom(const VolumeHandleBase* /*handle*/) const {
    // unable to create 2D histogram without gradient volume
    return 0;
}

size_t HistogramIntensityGradient::getBucketCountIntensity() const {
    return histValues_.size();
}

size_t HistogramIntensityGradient::getBucketCountGradient() const {
    return histValues_[0].size();
}

int HistogramIntensityGradient::getValue(int i, int g) const {
    return histValues_[i][g];
}

float HistogramIntensityGradient::getNormalized(int i, int g) const {
    return (static_cast<float>(histValues_[i][g]) / static_cast<float>(maxValue_));
}

float HistogramIntensityGradient::getLogNormalized(int i, int g) const {
    return (logf(static_cast<float>(1+histValues_[i][g]) ) / log(static_cast<float>(1+maxValue_)));
}

int HistogramIntensityGradient::getMaxValue() const {
    return maxValue_;
}

tgt::ivec2 HistogramIntensityGradient::getSignificantRangeIntensity() const {
    return significantRangeIntensity_;
}

tgt::ivec2 HistogramIntensityGradient::getSignificantRangeGradient() const {
    return significantRangeGradient_;
}

float HistogramIntensityGradient::getScaleFactor() const {
    return scaleFactor_;
}

template<class U>
void HistogramIntensityGradient::calcHG(const VolumeAtomic<U>* volumeGrad, const Volume* volumeIntensity, int bucketCounti, int bucketCountg, bool scale) {
    // bits used for gradients
    int bitsG = volumeGrad->getBitsStored() / volumeGrad->getNumChannels();
    float halfMax = (pow(2.f, bitsG) - 1.f) / 2.f;
    const tgt::ivec3 gradDim = volumeGrad->getDimensions();

    std::vector<float> gradientLengths;
    float maxGradientLength = 0.f;

    // calculate length of all gradients
    for (int z = 0; z < gradDim.z; ++z) {
        for (int y = 0; y < gradDim.y; ++y) {
            for (int x = 0; x < gradDim.x; ++x) {
                float gx = volumeGrad->voxel(x,y,z).r - halfMax;
                float gy = volumeGrad->voxel(x,y,z).g - halfMax;
                float gz = volumeGrad->voxel(x,y,z).b - halfMax;

                float nlength = tgt::length(tgt::vec3(gx, gy, gz));

                if (nlength > maxGradientLength)
                    maxGradientLength = nlength;

                gradientLengths.push_back(nlength);
            }
        }
    }

    // maximum length of a gradient
    float maxLength;
    if (scale) {
        maxLength = maxGradientLength;
        scaleFactor_ = maxLength / (halfMax * sqrt(3.f));
    }
    else
        maxLength = halfMax * sqrt(3.f);

    // init histogram with 0 values
    for (int j = 0; j < bucketCounti; j++)
        histValues_.push_back(std::vector<int>(bucketCountg));


    int bucketg, bucketi;
    maxValue_ = 0;
    significantRangeIntensity_ = tgt::ivec2(bucketCounti, -1);
    significantRangeGradient_ = tgt::ivec2(bucketCountg, -1);

    for (int z = 0; z < gradDim.z; ++z) {
        for (int y = 0; y < gradDim.y; ++y) {
            for (int x = 0; x < gradDim.x; ++x) {
                float intensity;
                if (volumeIntensity)
                    intensity = volumeIntensity->getVoxelFloat(x,y,z);
                else
                    intensity = volumeGrad->getVoxelFloat(x,y,z,volumeGrad->getNumChannels()-1);

                if (intensity > 1.f)
                    intensity = 1.f;

                int pos = z * gradDim.y * gradDim.x + y * gradDim.x + x;

                bucketi = tgt::ifloor(intensity * (bucketCounti - 1));
                bucketg = tgt::ifloor((gradientLengths[pos] / maxLength) * (bucketCountg - 1));

                histValues_[bucketi][bucketg]++;
                if ((histValues_[bucketi][bucketg]) > maxValue_)
                    maxValue_ = histValues_[bucketi][bucketg];

                if (bucketi < significantRangeIntensity_.x)
                    significantRangeIntensity_.x = bucketi;
                if (bucketi > significantRangeIntensity_.y)
                    significantRangeIntensity_.y = bucketi;
                if (bucketg < significantRangeGradient_.x)
                    significantRangeGradient_.x = bucketg;
                if (bucketg > significantRangeGradient_.y)
                    significantRangeGradient_.y = bucketg;
            }
        }
    }
}

void HistogramIntensityGradient::serialize(XmlSerializer& s) const {
    s.serialize("values", histValues_);
    s.serialize("maxValue", maxValue_);
    s.serialize("significantRangeIntensity", significantRangeIntensity_);
    s.serialize("significantRangeGradient", significantRangeGradient_);
    s.serialize("scaleFactor", scaleFactor_);
}

void HistogramIntensityGradient::deserialize(XmlDeserializer& s) {
    s.deserialize("values", histValues_);
    s.deserialize("maxValue", maxValue_);
    s.deserialize("significantRangeIntensity", significantRangeIntensity_);
    s.deserialize("significantRangeGradient", significantRangeGradient_);
    s.deserialize("scaleFactor", scaleFactor_);
}

} // namespace voreen

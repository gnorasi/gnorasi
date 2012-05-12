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

#ifndef VRN_HISTOGRAM_H
#define VRN_HISTOGRAM_H

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumederiveddata.h"

#include <string>
#include <iostream>
#include <fstream>

namespace voreen {

//template<typename T, int ND>
//    class HistogramGeneric {
//    public:
//        int getNumBuckets(int dim) const { 
//            if((dim >= 0) && (dim < ND))
//                return bucketCounts_[dim]; 
//            else {
//                tgtAssert(false, "Dimension-index out of range!");
//                return 0;
//            }
//        }
//
//        int getNumBuckets() const {  
//            return numBuckets_;
//        }
//        uint64_t getNumSamples() const {
//            return numSamples_;
//        }
//
//        uint64_t getBucket(int b) const { 
//            if(b < numBuckets_)
//                return buckets_[b]; 
//            else {
//                tgtAssert(false, "Index out of range!");
//                return 0;
//            }
//        }
//
//        void increaseBucket(int b) {
//            if(b < numBuckets_) {
//                buckets_[b]++; 
//                numSamples_++;
//            }
//            else 
//                tgtAssert(false, "Index out of range!");
//        }
//
//        T getMinValue(int dim) const {
//            return minValues_[dim];
//        }
//
//        T getMaxValue(int dim) const {
//            return minValues_[dim];
//        }
//
//        uint64_t getMaxBucket() {
//            //TODO: optimize?
//            uint64_t max = 0;
//            for(int i=0; i<getNumBuckets(); i++) 
//                if(buckets_[i] > max)
//                    max = buckets_[i];
//
//            return max;
//        }
//
//    protected:
//        void addSample(...) {
//            va_list args;
//            int nd = ND;
//            va_start(args, nd); 
//
//            T values[ND];
//            for(int i=0; i<ND; i++) {
//                values[i] = va_arg(args, T);
//            }
//
//            va_end(args);
//
//            int c[ND];
//            for(int i=0; i<ND; i++) 
//                c[i] = mapValueToBucket(values[i], i);
//
//            int b = getBucketNumber(c);
//            increaseBucket(b);
//        }
//
//        int mapValueToBucket(T v, int dim) const {
//            if(v < minValues_[dim]) {
//                //TODO: out of range
//                return 0;
//            }
//            else if(v > maxValues_[dim]) {
//                //TODO: out of range
//                return (bucketCounts_[dim] - 1);
//            }
//            else {
//                v -= minValues_[dim];
//                return static_cast<int>(bucketCounts_[dim] * (v / (maxValues_[dim] - minValues_[dim])));
//                //TODO: clamp?
//            }
//        }
//
//        int getBucketNumber(const int* c) const {
//            int n = 0;
//            int helper = 1;
//            for(int i=0; i<ND; i++) {
//                if((c[i] >= 0) && (c[i] < bucketCounts_[i])) {
//                    n += helper * c[i];
//                    helper *= bucketCounts_[i];
//                }
//                else {
//                    //TODO
//                }
//            }
//            return n;
//        }
//
//        HistogramGeneric(.../* T minValue, T maxValue, int bucketCounts for each dimension */) : numBuckets_(0), buckets_(0), numSamples_(0) {
//            va_list args;
//            int nd = ND*3;
//            va_start(args, nd); 
//
//            for(int i=0; i<ND; i++) {
//                minValues_[i] = va_arg(args, T);
//                maxValues_[i] = va_arg(args, T);
//                bucketCounts_[i] = va_arg(args, int);
//            }
//
//            va_end(args);
//
//            numBuckets_ = 1;
//            for(int i=0; i<ND; i++) 
//                numBuckets_ *= bucketCounts_[i];
//
//            buckets_ = new uint64_t[getNumBuckets()];
//
//            for(int i=0; i<getNumBuckets(); i++) 
//                buckets_[i] = 0;
//        }
//
//    private:
//        T minValues_[ND];
//        T maxValues_[ND];
//        int bucketCounts_[ND];
//        int numBuckets_;
//
//        uint64_t* buckets_;
//        uint64_t numSamples_;
//    };
//
//template <typename T>
//class Histogram1DGeneric : public HistogramGeneric<T, 1> {
//    public:
//        Histogram1DGeneric(T minValue, T maxValue, int bucketCount) : HistogramGeneric<T, 1>(minValue, maxValue, bucketCount) {}
//
//        void addSample(T value) {
//            HistogramGeneric<T, 1>::addSample(value);
//        }
//        T getMinValue() const {
//            return getMinValue(0);
//        }
//
//        T getMaxValue() const {
//            return getMaxValue(0);
//        }
//    private:
//};
//
//class Histogram1D : public Histogram1DGeneric<float> {
//    public:
//        Histogram1D(float minValue, float maxValue, int bucketCount) : Histogram1DGeneric<float>(minValue, maxValue, bucketCount) {}
//};
//
//Histogram1D createHistogram1DFromVolume(const VolumeHandleBase* handle, int bucketCount);

//--------------------------------------------------------------------------
//Old Historam classes (will be replaced)

/// 1D Intensity Histogram.
class VRN_CORE_API HistogramIntensity : public VolumeDerivedData {
public:
    /// Create new histogram with bucketCount buckets from volume
    HistogramIntensity(const Volume* volume, int bucketCount);

    /// Copy constructor.
    HistogramIntensity(const HistogramIntensity& h);

    /// Empty default constructor required by VolumeDerivedData interface.
    HistogramIntensity();

    /**
     * Creates a histogram with a bucket count of 256.
     *
     * @see VolumeDerivedData
     */
    virtual VolumeDerivedData* createFrom(const VolumeHandleBase* handle) const;

    size_t getBucketCount() const;

    /// get value in bucket i
    int getValue(int i) const;

    /// get value in bucket i
    int getValue(size_t i) const;

    /// get value in bucket nearest to i
    int getValue(float i) const;

    /// Returns normalized (with max.) histogram value at bucket i
    float getNormalized(int i) const;

    /// Returns normalized (with max.) histogram value at bucket nearest to i
    float getNormalized(float i) const;

    /// Returns normalized logarithmic histogram value at bucket i
    float getLogNormalized(int i) const;

    /// Returns normalized logarithmic histogram value at bucket nearest to i
    float getLogNormalized(float i) const;

    /// @see VolumeDerivedData
    virtual void serialize(XmlSerializer& s) const;

    /// @see VolumeDerivedData
    virtual void deserialize(XmlDeserializer& s);

protected:
    std::vector<int> histValues_;
    int maxValue_;
};

// ----------------------------------------------------------------------------

/// 2D histogram using intensity and gradient length.
class VRN_CORE_API HistogramIntensityGradient : public VolumeDerivedData {
public:
    /**
     * Calculate 2D Histogram.
     *
     * @param volumeGrad Holds the gradients
     * @param volumeIntensity Holds the intensity
     * @param bucketCounti Intensity bucket count
     * @param bucketCountg Gradient strength bucket count
     * @param scale should the histogram scaled to maximum gradient length in the dataset?
     */
    HistogramIntensityGradient(const VolumeHandleBase* volumeGrad, const VolumeHandleBase* volumeIntensity,
                               int bucketCounti, int bucketCountg, bool scale = false);

    /// Empty default constructor required by VolumeDerivedData interface.
    HistogramIntensityGradient();

    /**
     * Returns 0, since histogram construction requires an additional gradient volume.
     *
     * @see VolumeDerivedData
     */
    virtual VolumeDerivedData* createFrom(const VolumeHandleBase* handle) const;

    /// Returns voxels in bucket.
    int getValue(int i, int g) const;

    /// Returns normalized (with max.) histogram value
    float getNormalized(int i, int g) const;

    /// Returns normalized logarithmic histogram value
    float getLogNormalized(int i, int g) const;

    /// Returns the maximal bucket value in the histogram
    int getMaxValue() const;

    /// Returns the significant intensity range of the histogram, i.e.
    /// the minimal / maximal bucket row
    tgt::ivec2 getSignificantRangeIntensity() const;

    /// Returns the significant gradient range of the histogram, i.e.
    /// the minimal / maximal bucket column
    tgt::ivec2 getSignificantRangeGradient() const;

    size_t getBucketCountIntensity() const;
    size_t getBucketCountGradient() const;

    float getScaleFactor() const;

    /// @see VolumeDerivedData (currently unimplemented)
    virtual void serialize(XmlSerializer& s) const;

    /// @see VolumeDerivedData (currently unimplemented)
    virtual void deserialize(XmlDeserializer& s);

protected:
    std::vector<std::vector<int> > histValues_;  ///< 2D array representing the histogram (first index = intensity)
    int maxValue_;
    tgt::ivec2 significantRangeIntensity_;
    tgt::ivec2 significantRangeGradient_;

    float scaleFactor_;

    // calculate the histogram
    template<class U>
    void calcHG(const VolumeAtomic<U>* volumeGrad, const Volume* volumeIntensity, int bucketCounti, int bucketCountg, bool scale);
};

} // namespace voreen

#endif // VRN_HISTOGRAM_H

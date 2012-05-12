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

#include "voreen/qt/widgets/volumeviewhelper.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumeatomic.h"

#include <QDir>

namespace voreen {

std::string VolumeViewHelper::getVolumeType(const Volume* volume) {
    
    if (dynamic_cast<const VolumeUInt8*>(volume))       return "UInt8";
    if (dynamic_cast<const VolumeUInt16*>(volume))      return "UInt16";
    if (dynamic_cast<const VolumeUInt32*>(volume))      return "UInt32";
    if (dynamic_cast<const VolumeUInt64*>(volume))      return "UInt64";
    if (dynamic_cast<const VolumeInt8*>(volume))        return "Int8";
    if (dynamic_cast<const VolumeInt16*>(volume))       return "Int16";
    if (dynamic_cast<const VolumeInt32*>(volume))       return "Int32";
    if (dynamic_cast<const VolumeInt64*>(volume))       return "Int64";
    if (dynamic_cast<const VolumeFloat*>(volume))       return "Float";
    if (dynamic_cast<const VolumeDouble*>(volume))      return "Double";

    if (dynamic_cast<const Volume2xUInt8*>(volume))     return "2xUInt8";
    if (dynamic_cast<const Volume2xUInt16*>(volume))    return "2xUInt16";
    if (dynamic_cast<const Volume2xUInt32*>(volume))    return "2xUInt32";
    if (dynamic_cast<const Volume2xUInt64*>(volume))    return "2xUInt64";
    if (dynamic_cast<const Volume2xInt8*>(volume))      return "2xInt8";
    if (dynamic_cast<const Volume2xInt16*>(volume))     return "2xInt16";
    if (dynamic_cast<const Volume2xInt32*>(volume))     return "2xInt32";
    if (dynamic_cast<const Volume2xInt64*>(volume))     return "2xInt64";
    if (dynamic_cast<const Volume2xFloat*>(volume))     return "2xFloat";
    if (dynamic_cast<const Volume2xDouble*>(volume))    return "2xDouble";

    if (dynamic_cast<const Volume3xUInt8*>(volume))     return "3xUInt8";
    if (dynamic_cast<const Volume3xUInt16*>(volume))    return "3xUInt16";
    if (dynamic_cast<const Volume3xUInt32*>(volume))    return "3xUInt32";
    if (dynamic_cast<const Volume3xUInt64*>(volume))    return "3xUInt64";
    if (dynamic_cast<const Volume3xInt8*>(volume))      return "3xInt8";
    if (dynamic_cast<const Volume3xInt16*>(volume))     return "3xInt16";
    if (dynamic_cast<const Volume3xInt32*>(volume))     return "3xInt32";
    if (dynamic_cast<const Volume3xInt64*>(volume))     return "3xInt64";
    if (dynamic_cast<const Volume3xFloat*>(volume))     return "3xFloat";
    if (dynamic_cast<const Volume3xDouble*>(volume))    return "3xDouble";

    if (dynamic_cast<const Volume4xUInt8*>(volume))     return "4xUInt8";
    if (dynamic_cast<const Volume4xUInt16*>(volume))    return "4xUInt16";
    if (dynamic_cast<const Volume4xUInt32*>(volume))    return "4xUInt32";
    if (dynamic_cast<const Volume4xUInt64*>(volume))    return "4xUInt64";
    if (dynamic_cast<const Volume4xInt8*>(volume))      return "4xInt8";
    if (dynamic_cast<const Volume4xInt16*>(volume))     return "4xInt16";
    if (dynamic_cast<const Volume4xInt32*>(volume))     return "4xInt32";
    if (dynamic_cast<const Volume4xInt64*>(volume))     return "4xInt64";
    if (dynamic_cast<const Volume4xFloat*>(volume))     return "4xFloat";
    if (dynamic_cast<const Volume4xDouble*>(volume))    return "4xDouble";
    
    return "<unknown>";
}

QPixmap VolumeViewHelper::generatePreview(const VolumeHandleBase* volume, int height) {
    return generateBorderedPreview(volume, height, 0);
}

QPixmap VolumeViewHelper::generateBorderedPreview(const VolumeHandleBase* handle, int height, int border) {
    
    // gamma correction factor (amplifies low gray values)
    const float GAMMA = 1.8f;

    const Volume* volume = handle->getRepresentation<Volume>();
    float xSpacing = handle->getSpacing()[0];
    float ySpacing = handle->getSpacing()[1];
    float xDimension = static_cast<float>(volume->getDimensions()[0]);
    float yDimension = static_cast<float>(volume->getDimensions()[1]);
    int zDimension = static_cast<int>(volume->getDimensions()[2]/2);

    // determine offsets and scale factors for non-uniform aspect ratios
    float aspectRatio = (yDimension * ySpacing) / (xDimension * xSpacing);
    float xOffset, yOffset, xScale, yScale;
    if (aspectRatio <= 1.f) {
        xOffset = 0.f;
        yOffset = ((height-1.f) * (1.f - aspectRatio)) / 2.f;
        xScale = 1.f;
        yScale = 1.f / aspectRatio;
    }
    else {
        xOffset = ((height-1.f) * (1.f - 1.f/aspectRatio)) / 2.f;
        yOffset = 0.f;
        xScale = aspectRatio;
        yScale = 1.f;
    }

    // generate preview in float buffer
    float* previewFloat = new float[height*height];
    float minVal = volume->elementRange().y;
    float maxVal = volume->elementRange().x;
    for (int y = 0; y < height; y++){
        for (int x = 0; x < height; x++){
            tgt::vec3 position;
            position.x = ((x-xOffset) / (height-1)) * xScale * (xDimension-1.f);
            position.y = ((y-yOffset) / (height-1)) * yScale * (yDimension-1.f);
            position.z = zDimension;
            int previewIndex = y*height + x;
            float val = 0.f;
            if (position.x >= 0 && position.y >= 0 && position.x < xDimension && position.y < yDimension)
                val = volume->getVoxelFloatLinear(position);
            previewFloat[previewIndex] = val;
            minVal = std::min(minVal, val);
            maxVal = std::max(maxVal, val);
        }
    }

    // histogram equalization
    float valOffset = minVal;
    float valScale = maxVal - minVal;
    if (valScale > 0.f) {
        for (int y=0; y<height; y++) {
            for (int x=0; x<height; x++) {
                int previewIndex = y*height + x;
                previewFloat[previewIndex] = (previewFloat[previewIndex] - valOffset)  / valScale;     
            }
        }
    }

    // gamma correction
    for (int i=0; i<height*height; i++) {
        previewFloat[i] = pow(previewFloat[i], 1.f/GAMMA);
    }

    // create QImage from float buffer
    QImage* preview = new QImage(height , height, QImage::Format_ARGB32);
    for (int y=0; y<height; y++) {
        for (int x=0; x<height; x++) {
            int previewIndex = y*height + x;
            int greyVal = tgt::ifloor(previewFloat[previewIndex] * 255.f);
            preview->setPixel(x, y, qRgb(greyVal, greyVal, greyVal));
        }
    }
    delete previewFloat;

    // draw border
    for (int y = 0; y < preview->height(); y++){
        for (int x = 0; x < border; x++)
            preview->setPixel(preview->width()-x-1, y, qRgb(255, 255, 255));
    }
    for (int x = 0; x < preview->width(); x++){
        for (int y = 0; y < border; y++)
               preview->setPixel(x, preview->height()-y-1, qRgb(255, 255, 255));
    }

    QPixmap pixmap = QPixmap::fromImage(*preview);
    delete preview;

    return pixmap;
}

std::string VolumeViewHelper::volumeInfoString(const VolumeHandleBase* handle) {
    std::string outString;
    if (handle && handle->getRepresentation<Volume>()) {

        const Volume* volume = handle->getRepresentation<Volume>();

        std::string filename = getStrippedVolumeName(handle);
        if(filename.length() > 13) {
            QString ext = QString::fromStdString(filename);
            filename.resize(13);
            filename+="...";
            filename+=ext.section('.',-1).toStdString();
        }
        std::string spacing = getVolumeSpacing(handle);
        std::string dimension = getVolumeDimension(handle);
        std::string type = getVolumeType(volume);
        outString+="<style type='text/css'> table {margin-left:0px; margin-top:1px; font-size: 9px;}</style><table><tr><td>"+filename+"</td><td></td></tr><tr><td>type </td><td>" \
            +type+"</td></tr><tr><td>dimension </td><td>" \
            +dimension+"</td></tr><tr><td>spacing </td><td>" \
            +spacing+"</td></tr><tr><td>memSize </td><td>" \
    +getVolumeMemorySize(volume)+" bytes</td></tr> </table>";

    }
    else {
        outString = "<strong>Volume:</strong> no Volume";
    }
    return outString;
}

std::string VolumeViewHelper::getVolumeName(const VolumeHandleBase* handle){
    std::string volumeName;
    if (handle) {
        const VolumeHandle* vh = dynamic_cast<const VolumeHandle*>(handle);
        if(vh)
            volumeName = vh->getOrigin().getPath();
    }
    else {
        volumeName = "no Volume";
    }
    return volumeName;
}

std::string VolumeViewHelper::getStrippedVolumeName(const VolumeHandleBase* handle) {
    if (!handle)
        return "no volume";
    else {
        std::string volumePath = getVolumeName(handle);

        //  remove trailing slash (dicom)
        std::string::size_type separator = volumePath.find_last_of("/\\");
        if (separator == volumePath.size()-1)
            volumePath = volumePath.substr(0, volumePath.size()-1);

        return tgt::FileSystem::fileName(volumePath);
    }
}

std::string VolumeViewHelper::getVolumePath(const VolumeHandleBase* handle) {
    std::string volumeName;
    if (handle) {
        const VolumeHandle* vh = dynamic_cast<const VolumeHandle*>(handle);
        if(vh) {
            QString fn = QString::fromStdString(vh->getOrigin().getPath());
            volumeName = fn.section('/',0, -2).toStdString();
        }
    }
    else {
        volumeName = "no Volume";
    }
    return volumeName;

}

std::string VolumeViewHelper::getVolumeTimestep(const VolumeHandleBase* handle) {
    std::stringstream out;
    if (handle) {
        if (handle->getTimestep() != 0.f)
            out << " @" << handle->getTimestep();
    }
    return out.str();
}

std::string VolumeViewHelper::getVolumeDimension(const VolumeHandleBase* volume) {
    std::stringstream out;

    out << volume->getDimensions()[0] << " x " << volume->getDimensions()[1] << " x " << volume->getDimensions()[2];

    return out.str();
}

std::string VolumeViewHelper::getVolumeSpacing(const VolumeHandleBase* volume) {
    std::stringstream out;
    out << volume->getSpacing()[0] << " x " << volume->getSpacing()[1] << " x " << volume->getSpacing()[2];
    return out.str();
}

std::string VolumeViewHelper::getVolumeMemorySize(const Volume* volume) {
    std::stringstream out;

    size_t bytes = volume->getNumBytes();
    float mb = tgt::round(bytes/104857.6f) / 10.f;    //calculate mb with 0.1f precision
    float kb = tgt::round(bytes/102.4f) / 10.f;
    if (mb >= 0.5f) {
        out << mb << " MB";
    }
    else if (kb >= 0.5f) {
        out << kb << " kB";
    }
    else {
        out << bytes << " bytes";
    }
    return out.str();
}


size_t VolumeViewHelper::getVolumeMemorySizeByte(const Volume* volume) {
    size_t volumeBytes = 0;

    volumeBytes = volume->getNumBytes();
    return volumeBytes;

}

} // namespace

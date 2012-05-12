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

#include "quadhidacvolumereader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>

#include "tgt/exception.h"

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorconvert.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatormirror.h"
#include "voreen/core/utils/stringconversion.h"

/*
// excerpt of quadhidac file
Title=Dynamic PET Image Sequence.
;% Using List-mode Expectation Maximisation

Subsets=98
Rho=1.200000
ntf=1
dxyr=0.400000
dt=[900]

nbx=150.000000
nby=150.000000
nbz=276.000000

FrameTime=900

-------------
quadhidac files with title KS PET have a mixed x- and y-dimension
*/

namespace voreen {

const std::string QuadHidacVolumeReader::loggerCat_ = "voreen.io.VolumeReader.QuadHidac";

QuadHidacVolumeReader::QuadHidacVolumeReader(ProgressBar* progress) :
    VolumeReader(progress)
{
    extensions_.push_back("i4d");
    protocols_.push_back("i4d");
}

VolumeCollection* QuadHidacVolumeReader::read(const std::string &url)
    throw (tgt::FileException, std::bad_alloc)
{
    const VolumeOrigin origin(url);
    std::string fileName = origin.getPath();

    std::string title;
    int subsets;
    int ntf;
    float dxyr;
    float rho;
    std::vector<float> dt;
    tgt::ivec3 dims;
    tgt::ivec3 croppedDims(0);
    unsigned int frameTime;
    tgt::mat4 transformationMatrix = tgt::mat4::identity;

    std::fstream file(fileName.c_str(), std::ios::in | std::ios::binary);

    if (!file)
        throw tgt::FileNotFoundException("reading; " + url);

    LINFO(fileName);
    char ch = 0;
    std::string typeStr;
    std::string argumentsStr;
    bool type = true;
    bool comment = false;
    bool error = false;
    int pos=-1;

    while ((ch != 0x0c) && (!file.eof())) {
        file.read(&ch, 1);

        if ((ch != 0x0c) && (ch != 0x0a)) {
            if (ch == '=') {
                type = false;
                LINFO("Type: " << typeStr);
            } else if (ch == ';') {
                comment = true;
                type = false;
            } else {
                if (type)
                    typeStr += ch;
                else
                    argumentsStr += ch;
            }
        } else {
            if (comment) {
                LINFO("Comment: " << argumentsStr);
            } else {
                std::stringstream argStream(argumentsStr);

                if (typeStr == "") {
                } else if (typeStr == "Title") {
                    title = argumentsStr;
                    pos = static_cast<int>(title.find("KS PET"));
                    LINFO("Value: " << title);
                } else if (typeStr == "Subsets") {
                    argStream >> subsets;
                    LINFO("Value: " << subsets);
                } else if (typeStr == "Rho") {
                    argStream >> rho;
                    LINFO("Value: " << rho);
                } else if (typeStr == "ntf") {
                    argStream >> ntf;
                    LINFO("Value: " << ntf);
                } else if (typeStr == "dxyr") {
                    argStream >> dxyr;
                    LINFO("Value: " << dxyr);
                } else if (typeStr == "row1") {
                    argStream >> transformationMatrix.t00;
                    argStream >> transformationMatrix.t01;
                    argStream >> transformationMatrix.t02;
                    argStream >> transformationMatrix.t03;

                } else if (typeStr == "row2") {
                    argStream >> transformationMatrix.t10;
                    argStream >> transformationMatrix.t11;
                    argStream >> transformationMatrix.t12;
                    argStream >> transformationMatrix.t13;
                } else if (typeStr == "row3") {
                    argStream >> transformationMatrix.t20;
                    argStream >> transformationMatrix.t21;
                    argStream >> transformationMatrix.t22;
                    argStream >> transformationMatrix.t23;
                } else if (typeStr == "row4") {
                    argStream >> transformationMatrix.t30;
                    argStream >> transformationMatrix.t31;
                    argStream >> transformationMatrix.t32;
                    argStream >> transformationMatrix.t33;
                } else if (typeStr == "dt") {
                    // expected format: [float[,float]], e.g. '[30.0]' or '[2.0,2.0,30.0]' 
                    std::string dtStr;
                    argStream >> dtStr;
                    LINFO("String: " << dtStr);

                    // split dt string into tokens
                    std::vector<std::string> dtTokens;
                    if (dtStr.size() < 2) 
                        goto dt_malformed;
                    else if (dtStr.substr(0, 1) != "[" || dtStr.substr(dtStr.size()-1, 1) != "]")
                        goto dt_malformed;
                    dtTokens = strSplit(dtStr.substr(1, dtStr.size() - 2), ',');
                    if (dtTokens.empty())
                        goto dt_malformed;

                    // convert string tokens to float
                    for (size_t i=0; i<dtTokens.size(); i++)
                        dt.push_back(stof(dtTokens.at(i)));
                    goto dt_finished;

dt_malformed:       LWARNING("String malformed! Expected: '[float,...]'");
                    continue;

dt_finished:        LINFO("Parsed (" << dt.size() << " values" << "): " << "[" << strJoin(dt, ",") << "]");

                } else if (typeStr == "nbx") {
                    // if a KS PET file is read then pos = 0
                    if (pos == -1){
                        argStream >> dims.x;
                        LINFO("Value: " << dims.x);

                    }else { argStream >> dims.y;
                        LINFO("Value: " << dims.y);
                        LINFO("nbx and nby are twisted");
                    }

                } else if (typeStr == "nby") {
                    //argStream >> dims.y;
                    if (pos == -1){
                        argStream >> dims.y;
                        LINFO("Value: " << dims.y);
                    }else { argStream >> dims.x;
                        LINFO("Value: " << dims.x);
                    }
                } else if (typeStr == "nbz") {
                    argStream >> dims.z;
                    LINFO("Value: " << dims.z);

                } else if (typeStr == "croppednbx") {
                    // if a KS PET file is read then pos = 0
                    if (pos == -1) {
                        argStream >> croppedDims.x;
                    } else {
                        argStream >> croppedDims.y;
                    }

                } else if (typeStr == "croppednby") {
                    // if a KS PET file is read then pos = 0
                    if (pos == -1) {
                        argStream >> croppedDims.y;
                    } else {
                        argStream >> croppedDims.x;
                    }

                } else if (typeStr == "croppednbz") {
                    argStream >> croppedDims.z;

                } else if (typeStr == "FrameTime") {
                    argStream >> frameTime;
                    LINFO("Value: " << frameTime);
                } else {
                    LWARNING("Unknown parameter: " << typeStr.substr(0, 20) << " (value: " << argumentsStr << ")");
                }

                if (argStream.fail()) {
                    LERROR("Format error");
                    error = true;
                }
            }

            typeStr = "";
            argumentsStr = "";
            type = true;
            comment = false;
        }
    }

    if (error)
        return 0;

    if (ntf != static_cast<int>(dt.size()))
        LWARNING("Number of time frames (" << ntf << ") does not match size of 'dt' vector (" << dt.size() << ")");

    // if croppedDims is available, use it, nicer version is in work
    if (croppedDims.z > 0)
        dims = croppedDims;
    
    // detect frame range to load (all frames or single one)
    int minFrame = 0;
    int maxFrame = ntf-1; /*std::min(ntf, 5);*/ 
    if (origin.getSearchParameter("frame") != "") {
        int frameID = stoi(origin.getSearchParameter("frame"));
        if (frameID >= 0 && frameID < ntf)
            minFrame = maxFrame = frameID;
        else
            LWARNING("Invalid frame id: " << frameID);
    }

    tgt::vec2 sequenceIntensityRange(-1.f);
    if (origin.getSearchParameter("minIntensity") != "" &&
            origin.getSearchParameter("maxIntensity") != "") {
       sequenceIntensityRange.x = stof(origin.getSearchParameter("minIntensity")); 
       sequenceIntensityRange.y = stof(origin.getSearchParameter("maxIntensity")); 
       LINFO("Using intensity range read from origin: " << sequenceIntensityRange);
    }
    else if (ntf > 1) {
        LINFO("Detecting intensity range of frame sequence for float conversion ...");
        
        if (getProgressBar()) {
            getProgressBar()->setTitle("Quad-HIDAC Reader");
            getProgressBar()->setMessage("Detecting intensity range of frame sequence ...");
            getProgressBar()->show();
            getProgressBar()->setProgress(0.f);
            getProgressBar()->forceUpdate();
        }

        // save stream position
        std::ifstream::pos_type dataPos = file.tellg();
        
        // read sequence frame-wise, compute min/max values for each frame 
        // and derive global min/max from frame values
        sequenceIntensityRange = tgt::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::min());
        for (int frame=0; frame <= maxFrame; frame++) {
            float* scalars = 0;
            VolumeFloat* volumeFloat = 0;
            try {
                scalars = new float[hmul(dims)];
                file.read(reinterpret_cast<char*>(scalars), hmul(dims)*4);
                volumeFloat = new VolumeFloat(scalars, dims);
                sequenceIntensityRange.x = std::min(sequenceIntensityRange.x, volumeFloat->min());
                sequenceIntensityRange.y = std::max(sequenceIntensityRange.y, volumeFloat->max());
                delete volumeFloat; //< also frees scalar data
            }
            catch (const std::exception& e) {
                if (volumeFloat)
                    delete volumeFloat;
                else
                    delete[] scalars;
                file.close();

                throw new VoreenException(
                    std::string("QuadHidacVolumeReader: failure during intensity range detection: ") + 
                    std::string(e.what()));
            }
        }
        LINFO("Intensity range: " << sequenceIntensityRange);

        // reset stream position to beginning of data block
        file.seekg(dataPos);
    }


    // result collection
    VolumeCollection* volumeCollection = new VolumeCollection();

    if (getProgressBar()) {
        getProgressBar()->setTitle("Quad-HIDAC Reader");
        getProgressBar()->show();
    }

    // load frames
    for (int frame=0; frame <= maxFrame; frame++) {

        // skip frames
        if (frame < minFrame) {
            file.seekg(hmul(dims)*4, std::ios::cur);
            continue;
        }
        else {
            LINFO("Reading frame " << frame+1 << "/" << ntf << " ...");
            if (getProgressBar()) {
                getProgressBar()->setMessage("Loading frame " + itos(frame+1) + "/" + itos(ntf) + " ...");
                getProgressBar()->setProgress(static_cast<float>(frame - minFrame)/(maxFrame - minFrame + 1.f));  
            }
        }

        // read frame data (float)
        VolumeHandle* volumeFloat = 0;
        float* scalars = new float[hmul(dims)];
        try {
            file.read(reinterpret_cast<char*>(scalars), hmul(dims)*4);

            //reduce spacing:
            dxyr *= 0.01f;
            volumeFloat = new VolumeHandle(new VolumeFloat(scalars, dims), tgt::vec3(dxyr), tgt::vec3(0.0f));
            centerVolume(volumeFloat);
            //VolumeHandle* t = VolumeOperatorMirrorX::APPLY_OP(volumeFloat);
            //delete volumeFloat;
            //volumeFloat = t;
        }
        catch (...) {
            delete[] scalars;
            file.close();
            // do not throw exception, but return frames that have been loaded so far
            LERROR("Failed to read frame data from file!");
            return volumeCollection;
        }

        // convert float volume to uint16
        VolumeHandle* volumeUInt16 = 0;
        try {
            VolumeOperatorConvert voConvert;
            if (sequenceIntensityRange != tgt::vec2(-1.f))
                voConvert.setInputIntensityRange(sequenceIntensityRange);

            volumeUInt16 = voConvert.apply<uint16_t>(volumeFloat);
            //delete volumeFloat;
        }
        catch (std::bad_alloc&) {
            delete volumeFloat;
            file.close();
            // do not throw exception, but return frames that have been loaded so far
            LERROR("Failed to convert frame to UInt16 (bad allocation)!");
            return volumeCollection;
        }
        //volumeUInt16->setPhysicalToWorldMatrix(transformationMatrix);
        tgt::mat4 m = tgt::mat4(0.0f, 1.0f, 0.0f, 0.0f,
                                1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);
        volumeUInt16->setPhysicalToWorldMatrix(m);

        if ((size_t)frame < dt.size())
            volumeUInt16->setTimestep(dt.at(frame));
        VolumeOrigin frameOrigin("i4d", fileName);
        frameOrigin.addSearchParameter("frame", itos(frame));
        if (sequenceIntensityRange != tgt::vec2(-1.f)) {
            frameOrigin.addSearchParameter("minIntensity", ftos(sequenceIntensityRange.x));
            frameOrigin.addSearchParameter("maxIntensity", ftos(sequenceIntensityRange.y));
        }
        volumeUInt16->setOrigin(frameOrigin);
        volumeCollection->add(volumeUInt16);
    }

    if (getProgressBar())
        getProgressBar()->hide();

    // clean up
    file.close();

    return volumeCollection;
}

VolumeHandleBase* QuadHidacVolumeReader::read(const VolumeOrigin& origin) 
    throw (tgt::FileException, std::bad_alloc) 
{
    if (origin.getSearchParameter("frame") == "")
        LWARNING("Origin does not specify frame id");

    VolumeCollection* collection = read(origin.getURL());
    if (collection != 0 && !collection->empty()) {
        if (collection->size() > 1)
            LWARNING("More than one volume loaded from origin");
        VolumeHandleBase* volume = collection->first();
        delete collection;
        return volume;
    }
    else
        return 0;
}

VolumeReader* QuadHidacVolumeReader::create(ProgressBar* progress) const {
    return new QuadHidacVolumeReader(progress);
}

} // namespace voreen

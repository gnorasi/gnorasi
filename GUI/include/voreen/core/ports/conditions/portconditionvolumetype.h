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

#ifndef VRN_PORTCONDITION_VOLUMETYPE_H
#define VRN_PORTCONDITION_VOLUMETYPE_H

#include "voreen/core/ports/conditions/portcondition.h"
#include "voreen/core/ports/volumeport.h"

#include "voreen/core/datastructures/volume/volumeatomic.h"

namespace voreen {

/**
 * Generic base class for port conditions that check for a specific volume type T. 
 */
template<typename T>
class PortConditionVolumeType : public PortCondition {
public:
    PortConditionVolumeType(const std::string& volTypeName);

    virtual ~PortConditionVolumeType();

    /**
     * Returns true, if the inport has data and 
     * the encapsulated volume is of type T.
     */
    virtual bool acceptsPortData() const;

protected:
    virtual void setCheckedPort(const Port* checkedPort);

    const VolumePort* volumePort_;
};

// ------------------------------------------------------------------------------------------------

/**
 * Port condition that allows OpenGL-compatible volume types,
 * i.e., volumes that can be converted into an OpenGL texture.
 * These are all standard volume types except 64 integer and double:
 * - VolumeUInt8, VolumeInt8
 * - VolumeUInt16, VolumeInt16
 * - VolumeUInt32, VolumeInt32
 * - VolumeFloat
 * - Volume2xUInt8, Volume2xInt8
 * - Volume2xUInt16, Volume2xInt16
 * - Volume2xUInt32, Volume2xInt32
 * - Volume2xFloat
 * - Volume3xUInt8, Volume3xInt8
 * - Volume3xUInt16, Volume3xInt16
 * - Volume3xUInt32, Volume3xInt32
 * - Volume3xFloat
 * - Volume4xUInt8, Volume4xInt8
 * - Volume4xUInt16, Volume4xInt16
 * - Volume4xUInt32, Volume4xInt32
 * - Volume4xFloat
 */
class PortConditionVolumeTypeGL : public PortCondition {
public:
    PortConditionVolumeTypeGL();

    virtual bool acceptsPortData() const;    

protected:
    virtual void setCheckedPort(const Port* checkedPort);
    PortConditionLogicalOr orCondition_;
};

// ------------------------------------------------------------------------------------------------

/**
 * Port condition that allows float and double volume types:
 * - VolumeFloat, VolumeDouble
 * - Volume2xFloat, Volume2xDouble
 * - Volume3xFloat, Volume3xDouble
 * - Volume4xFloat, Volume4xDouble
 */
class PortConditionVolumeTypeReal : public PortCondition {
public:
    PortConditionVolumeTypeReal();

    virtual bool acceptsPortData() const;    

protected:
    virtual void setCheckedPort(const Port* checkedPort);
    PortConditionLogicalOr orCondition_;
};

// ------------------------------------------------------------------------------------------------

/**
 * Port condition that allows integer volume types: 
 * - VolumeUInt8, VolumeInt8
 * - VolumeUInt16, VolumeInt16
 * - VolumeUInt32, VolumeInt32
 * - VolumeUInt64, VolumeInt64
 * - Volume2xUInt8, Volume2xInt8
 * - Volume2xUInt16, Volume2xInt16
 * - Volume2xUInt32, Volume2xInt32
 * - Volume2xUInt64, Volume2xInt64
 * - Volume3xUInt8, Volume3xInt8
 * - Volume3xUInt16, Volume3xInt16
 * - Volume3xUInt32, Volume3xInt32
 * - Volume3xUInt64, Volume3xInt64
 * - Volume4xUInt8, Volume4xInt8
 * - Volume4xUInt16, Volume4xInt16
 * - Volume4xUInt32, Volume4xInt32
 * - Volume4xUInt64, Volume4xInt64
 */
class PortConditionVolumeTypeInteger : public PortCondition {
public:
    PortConditionVolumeTypeInteger();

    virtual bool acceptsPortData() const;    

protected:
    virtual void setCheckedPort(const Port* checkedPort);
    PortConditionLogicalOr orCondition_;
};

// ------------------------------------------------------------------------------------------------

/**
 * Port condition that allows volumes with a specific number of channels.
 */
class PortConditionVolumeChannelCount : public PortCondition {
public: 
    PortConditionVolumeChannelCount(size_t numChannels);

    virtual bool acceptsPortData() const;  

    virtual std::string getDescription() const;

protected:
    virtual void setCheckedPort(const Port* checkedPort);

    const VolumePort* volumePort_;
    size_t numChannels_;
};


// ------------------------------------------------------------------------------------------------
// concrete subtypes of PortConditionVolumeType

// single channel

class PortConditionVolumeTypeUInt8 : public PortConditionVolumeType<VolumeUInt8> {
public:
    PortConditionVolumeTypeUInt8() : 
      PortConditionVolumeType<VolumeUInt8>("VolumeUInt8") {};
};

class PortConditionVolumeTypeInt8 : public PortConditionVolumeType<VolumeInt8> {
public:
    PortConditionVolumeTypeInt8() : 
      PortConditionVolumeType<VolumeInt8>("VolumeInt8") {};
};

class PortConditionVolumeTypeUInt16 : public PortConditionVolumeType<VolumeUInt16> {
public:
    PortConditionVolumeTypeUInt16() : 
      PortConditionVolumeType<VolumeUInt16>("VolumeUInt16") {};
};

class PortConditionVolumeTypeInt16 : public PortConditionVolumeType<VolumeInt16> {
public:
    PortConditionVolumeTypeInt16() :
      PortConditionVolumeType<VolumeInt16>("VolumeInt16") {};
};

class PortConditionVolumeTypeUInt32 : public PortConditionVolumeType<VolumeUInt32> {
public:
    PortConditionVolumeTypeUInt32() : 
      PortConditionVolumeType<VolumeUInt32>("VolumeUInt32") {};
};

class PortConditionVolumeTypeInt32 : public PortConditionVolumeType<VolumeInt32> {
public:
    PortConditionVolumeTypeInt32() : 
      PortConditionVolumeType<VolumeInt32>("VolumeInt32") {};
};

class PortConditionVolumeTypeUInt64 : public PortConditionVolumeType<VolumeUInt64> {
public:
    PortConditionVolumeTypeUInt64() : 
      PortConditionVolumeType<VolumeUInt64>("VolumeUInt64") {};
};

class PortConditionVolumeTypeInt64 : public PortConditionVolumeType<VolumeInt64> {
public:
    PortConditionVolumeTypeInt64() : 
      PortConditionVolumeType<VolumeInt64>("VolumeInt64") {};
};

class PortConditionVolumeTypeFloat : public PortConditionVolumeType<VolumeFloat> {
public:
    PortConditionVolumeTypeFloat() : 
      PortConditionVolumeType<VolumeFloat>("VolumeFloat") {};
};

class PortConditionVolumeTypeDouble : public PortConditionVolumeType<VolumeDouble> {
public:
    PortConditionVolumeTypeDouble() : 
      PortConditionVolumeType<VolumeDouble>("VolumeDouble") {};
};

// two channels

class PortConditionVolumeType2xUInt8 : public PortConditionVolumeType<Volume2xUInt8> {
public:
    PortConditionVolumeType2xUInt8() : 
      PortConditionVolumeType<Volume2xUInt8>("Volume2xUInt8") {};
};

class PortConditionVolumeType2xInt8 : public PortConditionVolumeType<Volume2xInt8> {
public:
    PortConditionVolumeType2xInt8() : 
      PortConditionVolumeType<Volume2xInt8>("Volume2xInt8") {};
};

class PortConditionVolumeType2xUInt16 : public PortConditionVolumeType<Volume2xUInt16> {
public:
    PortConditionVolumeType2xUInt16() : 
      PortConditionVolumeType<Volume2xUInt16>("Volume2xUInt16") {};
};

class PortConditionVolumeType2xInt16 : public PortConditionVolumeType<Volume2xInt16> {
public:
    PortConditionVolumeType2xInt16() :
      PortConditionVolumeType<Volume2xInt16>("Volume2xInt16") {};
};

class PortConditionVolumeType2xUInt32 : public PortConditionVolumeType<Volume2xUInt32> {
public:
    PortConditionVolumeType2xUInt32() : 
      PortConditionVolumeType<Volume2xUInt32>("Volume2xUInt32") {};
}; 

class PortConditionVolumeType2xInt32 : public PortConditionVolumeType<Volume2xInt32> {
public:
    PortConditionVolumeType2xInt32() : 
      PortConditionVolumeType<Volume2xInt32>("Volume2xInt32") {};
}; 

class PortConditionVolumeType2xUInt64 : public PortConditionVolumeType<Volume2xUInt64> {
public:
    PortConditionVolumeType2xUInt64() : 
      PortConditionVolumeType<Volume2xUInt64>("Volume2xUInt64") {};
}; 

class PortConditionVolumeType2xInt64 : public PortConditionVolumeType<Volume2xInt64> {
public:
    PortConditionVolumeType2xInt64() : 
      PortConditionVolumeType<Volume2xInt64>("Volume2xInt64") {};
}; 

class PortConditionVolumeType2xFloat : public PortConditionVolumeType<Volume2xFloat> {
public:
    PortConditionVolumeType2xFloat() : 
      PortConditionVolumeType<Volume2xFloat>("Volume2xFloat") {};
};

class PortConditionVolumeType2xDouble : public PortConditionVolumeType<Volume2xDouble> {
public:
    PortConditionVolumeType2xDouble() : 
      PortConditionVolumeType<Volume2xDouble>("Volume2xDouble") {};
};

// three channels

class PortConditionVolumeType3xUInt8 : public PortConditionVolumeType<Volume3xUInt8> {
public:
    PortConditionVolumeType3xUInt8() : 
      PortConditionVolumeType<Volume3xUInt8>("Volume3xUInt8") {};
};

class PortConditionVolumeType3xInt8 : public PortConditionVolumeType<Volume3xInt8> {
public:
    PortConditionVolumeType3xInt8() : 
      PortConditionVolumeType<Volume3xInt8>("Volume3xInt8") {};
};

class PortConditionVolumeType3xUInt16 : public PortConditionVolumeType<Volume3xUInt16> {
public:
    PortConditionVolumeType3xUInt16() : 
      PortConditionVolumeType<Volume3xUInt16>("Volume3xUInt16") {};
};

class PortConditionVolumeType3xInt16 : public PortConditionVolumeType<Volume3xInt16> {
public:
    PortConditionVolumeType3xInt16() :
      PortConditionVolumeType<Volume3xInt16>("Volume3xInt16") {};
};

class PortConditionVolumeType3xUInt32 : public PortConditionVolumeType<Volume3xUInt32> {
public:
    PortConditionVolumeType3xUInt32() : 
      PortConditionVolumeType<Volume3xUInt32>("Volume3xUInt32") {};
}; 

class PortConditionVolumeType3xInt32 : public PortConditionVolumeType<Volume3xInt32> {
public:
    PortConditionVolumeType3xInt32() : 
      PortConditionVolumeType<Volume3xInt32>("Volume3xInt32") {};
}; 

class PortConditionVolumeType3xUInt64 : public PortConditionVolumeType<Volume3xUInt64> {
public:
    PortConditionVolumeType3xUInt64() : 
      PortConditionVolumeType<Volume3xUInt64>("Volume3xUInt64") {};
}; 

class PortConditionVolumeType3xInt64 : public PortConditionVolumeType<Volume3xInt64> {
public:
    PortConditionVolumeType3xInt64() : 
      PortConditionVolumeType<Volume3xInt64>("Volume3xInt64") {};
}; 

class PortConditionVolumeType3xFloat : public PortConditionVolumeType<Volume3xFloat> {
public:
    PortConditionVolumeType3xFloat() : 
      PortConditionVolumeType<Volume3xFloat>("Volume3xFloat") {};
};

class PortConditionVolumeType3xDouble : public PortConditionVolumeType<Volume3xDouble> {
public:
    PortConditionVolumeType3xDouble() : 
      PortConditionVolumeType<Volume3xDouble>("Volume3xDouble") {};
};

// four channels

class PortConditionVolumeType4xUInt8 : public PortConditionVolumeType<Volume4xUInt8> {
public:
    PortConditionVolumeType4xUInt8() : 
      PortConditionVolumeType<Volume4xUInt8>("Volume4xUInt8") {};
};

class PortConditionVolumeType4xInt8 : public PortConditionVolumeType<Volume4xInt8> {
public:
    PortConditionVolumeType4xInt8() : 
      PortConditionVolumeType<Volume4xInt8>("Volume4xInt8") {};
};

class PortConditionVolumeType4xUInt16 : public PortConditionVolumeType<Volume4xUInt16> {
public:
    PortConditionVolumeType4xUInt16() : 
      PortConditionVolumeType<Volume4xUInt16>("Volume4xUInt16") {};
};

class PortConditionVolumeType4xInt16 : public PortConditionVolumeType<Volume4xInt16> {
public:
    PortConditionVolumeType4xInt16() :
      PortConditionVolumeType<Volume4xInt16>("Volume4xInt16") {};
};

class PortConditionVolumeType4xUInt32 : public PortConditionVolumeType<Volume4xUInt32> {
public:
    PortConditionVolumeType4xUInt32() : 
      PortConditionVolumeType<Volume4xUInt32>("Volume4xUInt32") {};
}; 

class PortConditionVolumeType4xInt32 : public PortConditionVolumeType<Volume4xInt32> {
public:
    PortConditionVolumeType4xInt32() : 
      PortConditionVolumeType<Volume4xInt32>("Volume4xInt32") {};
}; 

class PortConditionVolumeType4xUInt64 : public PortConditionVolumeType<Volume4xUInt64> {
public:
    PortConditionVolumeType4xUInt64() : 
      PortConditionVolumeType<Volume4xUInt64>("Volume4xUInt64") {};
}; 

class PortConditionVolumeType4xInt64 : public PortConditionVolumeType<Volume4xInt64> {
public:
    PortConditionVolumeType4xInt64() : 
      PortConditionVolumeType<Volume4xInt64>("Volume4xInt64") {};
}; 

class PortConditionVolumeType4xFloat : public PortConditionVolumeType<Volume4xFloat> {
public:
    PortConditionVolumeType4xFloat() : 
      PortConditionVolumeType<Volume4xFloat>("Volume4xFloat") {};
};

class PortConditionVolumeType4xDouble : public PortConditionVolumeType<Volume4xDouble> {
public:
    PortConditionVolumeType4xDouble() : 
      PortConditionVolumeType<Volume4xDouble>("Volume4xDouble") {};
};


// -----------------------------------------------------------------------------------------------
// template definitions

template<typename T>
voreen::PortConditionVolumeType<T>::PortConditionVolumeType(const std::string& volTypeName) 
    : PortCondition("Volume of type " + volTypeName + " expected")
{}

template<typename T>
voreen::PortConditionVolumeType<T>::~PortConditionVolumeType() 
{}

template<typename T>
bool voreen::PortConditionVolumeType<T>::acceptsPortData() const  {
    if (!volumePort_ || !volumePort_->hasData())
        return false;

    const Volume* volume = volumePort_->getData()->getRepresentation<Volume>();
    return (dynamic_cast<const T*>(volume));
}

template<typename T>
void voreen::PortConditionVolumeType<T>::setCheckedPort(const Port* checkedPort) {
    if (!dynamic_cast<const VolumePort*>(checkedPort)) {
        LERRORC("voreen.PortConditionVolumeType", "Assigned port is not a volume port");
    }
    else {
        volumePort_ = static_cast<const VolumePort*>(checkedPort);
    }
}

} // namespace

#endif // VRN_PORTCONDITION_VOLUMETYPE_H

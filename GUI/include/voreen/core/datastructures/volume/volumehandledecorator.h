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

#ifndef VRN_VOLUMEHANDLEDECORATOR_H
#define VRN_VOLUMEHANDLEDECORATOR_H

#include "voreen/core/datastructures/volume/volumehandle.h"

namespace voreen {

///Basic decorator:
class VolumeHandleDecoratorIdentity : public VolumeHandleBase, public VolumeHandleObserver {
    public:
        VolumeHandleDecoratorIdentity(const VolumeHandleBase* vhb);
        ~VolumeHandleDecoratorIdentity() {}

        virtual const VolumeHandleBase* getDecorated() const {
            return base_;
        }

        virtual size_t getNumRepresentations() const {
            return base_->getNumRepresentations();
        }

        virtual const VolumeRepresentation* getRepresentation(size_t i) const {
            return base_->getRepresentation(i);
        }

        virtual const VolumeRepresentation* useConverter(const RepresentationConverterBase* converter) const {
            return base_->useConverter(converter);
        }

        virtual std::vector<std::string> getMetaDataKeys() const {
            return base_->getMetaDataKeys();
        }

        virtual const MetaDataBase* getMetaData(const std::string& key) const {
            return base_->getMetaData(key);
        }

        virtual bool hasMetaData(const std::string& key) const {
            return base_->hasMetaData(key);
        }

        virtual Modality getModality() const {
            return base_->getModality();
        }

        //VolumeHandleObserver implementation:
        virtual void volumeHandleDelete(const VolumeHandleBase* /*source*/) {
            // not ideal, but the best we can do here:
            base_ = 0;
            notifyDelete();
        }

        virtual void volumeChange(const VolumeHandleBase* /*source*/) {
            notifyReload();
        }

    protected:
        const VolumeHandleBase* base_;
};

class VolumeHandleDecoratorReplace : public VolumeHandleDecoratorIdentity {
    public:
        /** 
         * @brief Decorates a volumehandle, replacing a metadata item.
         * 
         * @param vhb The VolumeHandle to decorate
         * @param key Key of the MetaData item to replace.
         * @param value New Value. The decorator takes ownership.
         */
        VolumeHandleDecoratorReplace(const VolumeHandleBase* vhb, std::string key, MetaDataBase* value) : VolumeHandleDecoratorIdentity(vhb), key_(key), value_(value) {}
        ~VolumeHandleDecoratorReplace() { delete value_; }

        virtual std::vector<std::string> getMetaDataKeys() const {
            std::vector<std::string> keys = base_->getMetaDataKeys();

            if(!base_->hasMetaData(key_))
                keys.push_back(key_);

            return keys;
        }

        virtual const MetaDataBase* getMetaData(const std::string& key) const {
            if(key == key_)
                return value_;
            else
                return base_->getMetaData(key);
        }

        virtual bool hasMetaData(const std::string& key) const {
            if(key == key_)
                return true;
            else
                return base_->hasMetaData(key);
        }

        MetaDataBase* getValue() const {
            return value_;
        }

        void setValue(MetaDataBase* value) {
            delete value_;
            value_ = value;
        }

    protected:
        std::string key_;
        MetaDataBase* value_;
};

} // namespace

#endif

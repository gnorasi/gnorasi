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

#include "metadataextractor.h"

#include "voreen/core/utils/stringconversion.h"

using std::string;
using std::vector;

namespace voreen {

const std::string MetaDataExtractor::loggerCat_("voreen.MetaDataExtractor");

MetaDataExtractor::MetaDataExtractor()
    : Processor(),
    inport_(Port::INPORT, "volume.inport", false, Processor::INVALID_RESULT),
    outport_(Port::OUTPORT, "text", 0),
    property_("text", "Text")
{
    addPort(inport_);
    addPort(outport_);
    addProperty(property_);
}

MetaDataExtractor::~MetaDataExtractor() {}

Processor* MetaDataExtractor::create() const {
    return new MetaDataExtractor();
}

bool MetaDataExtractor::isReady() const {
    if (!isInitialized())
        return false;
    //if (!outport_.isReady())
    //    return false;

    return true;
}

void MetaDataExtractor::process() {

    std::stringstream output;

    //erase the items of the StringExpressionProperty
    property_.eraseItems();

    if (inport_.isReady()) {        

        //get all Meta Data keys
        vector<string> keys = inport_.getData()->getMetaDataKeys();

        //add items to the StringExpressionProptery
        vector<string>::const_iterator iter;
        for (iter = keys.begin(); iter != keys.end(); ++iter) {
            property_.addPlaceHolder(*iter, property_.makePlaceHolder(*iter));
        }

        //get the Meta Data keys to be replaced
        std::set<string> placeholders = property_.getPlaceholdersInText();
        //construct replacement map for this keys
        std::map<string, string> replacements;
        std::set<string>::const_iterator i;
        for (i = placeholders.begin(); i != placeholders.end(); ++i) {
            const MetaDataBase* m = inport_.getData()->getMetaData(*i);
            if (m) {
                string data = m->toString();
                data = trim(data);
                //replacements.insert(std::make_pair(*i,m->toString()));
                replacements.insert(std::make_pair(*i, data));
            }

        }

        output << property_.replacePlaceHoldersInText(replacements);

    }
    outport_.setData(output.str());

}

}   // namespace

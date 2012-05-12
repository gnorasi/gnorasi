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

#ifndef VRN_CACHING_H
#define VRN_CACHING_H

#include <vector>
#include <string>

#include "voreen/core/processors/processor.h"

namespace voreen {

class Cache {
public:
    Cache(Processor* proc);

    void addInport(Port* inport);
    void addAllInports();

    void addOutport(Port* outport);
    void addAllOutports();

    void addProperty(Property* prop);
    void addAllProperties();

    void initialize();
    bool isInitialized() const { return initialized_; }

    std::string getCurrentCacheDir();
    void clearCache();

    std::string getAllInportHashes();
    std::string getPropertyState();
    std::string getPropertyStateHash();

    bool store();
    bool restore();

    bool restoreOutportsFromDir(const std::string& dir);
    bool storeOutportsToDir(const std::string& dir);
protected:
    std::string getInterfaceString();
    bool stringEqualsFileContent(std::string str, std::string fname);

    /// category used in logging
    static const std::string loggerCat_;

private:
    Processor* processor_;
    bool initialized_;

    std::vector<std::string> inports_;
    std::vector<std::string> outports_;

    std::vector<std::string> properties_;
};

}  // namespace voreen

#endif

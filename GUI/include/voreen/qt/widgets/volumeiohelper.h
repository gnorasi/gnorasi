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

#ifndef VRN_VOLUMEIOHELPER_H
#define VRN_VOLUMEIOHELPER_H

#include "voreen/qt/voreenqtdefine.h"

#include "voreen/core/datastructures/volume/volumehandle.h"

#include <vector>
#include <QToolButton>

namespace voreen {

class VolumeHandleBase;
class VolumeContainer;
class VolumeReader;
class VolumeWriter;
class ProgressBar;
class VolumeSerializerPopulator;
class VolumeReaderSelectionDialog;
class VolumeListingDialog;
class VolumeCollection;

/**
 * Helper class for loading and saving volumes.
 * All loaded volumes are added to the assigned VolumeContainer.
 */
class VRN_QT_API VolumeIOHelper : public QObject {
    Q_OBJECT
public:
    VolumeIOHelper(VolumeContainer* vc, QWidget* parent = 0);
    ~VolumeIOHelper();

    /**
     * Assigns the volume container loaded volumes should be added to.
     */
    void setVolumeContainer(VolumeContainer* vc);

signals:
    /**
     * Is emitted after a volume has been loaded
     * and added to the assigned VolumeContainer. 
     *
     * @param collection contains the loaded volumes.
     *  note: is deleted after the signal call.
     */
    void volumeLoaded(const VolumeHandleBase* handle);

public slots:
    /**
     * Presents a file open dialog to the user
     * with filters for all supported volume formats. 
     * Additionally, filters for loading raw volumes 
     * and raw slice stacks are added. 
     * The user selected files are loaded then
     * using the resp. volume readers.
     */
    void showFileOpenDialog();

    /**
     * Presents a file save dialog to the user
     * with filters for all supported volume formats. 
     * The passed volume is saved using the user selected
     * volume writer under the user specified path.
     */
    void showFileSaveDialog(VolumeHandleBase* volume);

    /**
     * Loads volume(s) from the passed URL using the passed reader
     * and adds them to the assigned VolumeContainer. If the
     * URL references a container file with more than one volume,
     * a VolumeListingDialog is shown for selecting the volume(s) to load.
     */
    void loadURL(const std::string& url, VolumeReader* reader);

    /**
     * Loads the volume from the passed origin using the passed reader
     * and adds it to the assigned VolumeContainer.
     */
    void loadOrigin(const VolumeOrigin& origin, VolumeReader* reader);

    /**
     * Helper slot: passes all origins and the reader to loadOrigin().
     */
    void loadOrigins(const std::vector<VolumeOrigin>& origins, VolumeReader* reader);

    /**
     * Loads the raw volume with the passed filename and
     * adds it to the assigned VolumeContainer.
     * The required parameters are queried from the user
     * via a RawVolumeWidget.
     */
    void loadRawVolume(const std::string& filename);

    /**
     * Creates a volume from the passed raw slice files
     * and adds it to the assigned VolumeContainer.
     * The required parameters are queried from the user
     * via a RawVolumeWidget.
     */
    void loadRawSlices(const std::vector<std::string>& sliceFiles);

    /**
     * Saves the passed volume using the passed writer to the passed path.
     */
    void saveVolumeToPath(VolumeHandleBase* volume, VolumeWriter* writer, std::string filepath); 

private:
    /// Returns the filter string used in the file open dialog.
    std::string getVolumeReaderFilterString() const;

    QWidget* parent_;
    VolumeContainer* volumeContainer_;
    
    VolumeSerializerPopulator* volumeSerializerPopulator_;
    ProgressBar* progressBar_;

    VolumeReaderSelectionDialog* readerSelectionDialog_;
    VolumeListingDialog* volumeListingDialog_;

    static const std::string loggerCat_;
};

} // namespace

#endif // VRN_VOLUMEIOHELPER_H

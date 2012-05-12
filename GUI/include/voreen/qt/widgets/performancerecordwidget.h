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

#ifndef VRN_PERFORMANCERECORDWIDGET_H
#define VRN_PERFORMANCERECORDWIDGET_H

#include <QContextMenuEvent>
#include <QSettings>
#include <QTreeWidget>
#include <QWidget>

#include "voreen/qt/widgets/lineeditresetwidget.h"
#include "voreen/core/network/networkevaluator.h"
#include "voreen/qt/voreenqtdefine.h"

#include <vector>
#include <map>

class QMouseEvent;
class QTreeWidgetItem;
class QTextBrowser;
class QSplitter;
class QContextMenuEvent;

namespace voreen {

class NetworkEvaluator;
class PerformanceRecordTreeWidget;
class Processor;

class VRN_QT_API PerformanceRecordWidget : public QWidget, public NetworkEvaluator::ProcessWrapper {
Q_OBJECT
public:
    PerformanceRecordWidget(QWidget* parent = 0);
    ~PerformanceRecordWidget();
    void setEvaluator(NetworkEvaluator* evaluator);

    std::vector<const PerformanceRecord*> collectPerformanceRecords() const;
    void clearPerformanceRecords();

    /// Update after network has been processed.
    void afterNetworkProcess();

public slots:
    void processorsSelected(const QList<Processor*>& processors);
    void loadSettings();
    void resetSettings();

protected:
    PerformanceRecordTreeWidget* tree_;
    QIcon resetIcon_;
    QSplitter* splitter_;

    QAction* useAverageTime_;            // use average time of the performance records
    QAction* useLastTime_;               // use last time of the performance records

    QAction* sortByTotalProcessTime_;   // sort by total process time
    QAction* sortByBeforeProcessTime_;  // sort by before process time
    QAction* sortByProcessTime_;        // sort by process time
    QAction* sortByAfterProcessTime_;   // sort by after process time

    QAction* showTotalProcessTime_;     // show total process time on top
    QAction* showBeforeProcessTime_;    // show before process time on top
    QAction* showProcessTime_;          // show process time on top
    QAction* showAfterProcessTime_;     // show after process time on top

    QAction* clearTreeWhenRecordsUpdate_; // clear the tree when new updates are added

    QAction* resetPerformanceRecords_;  // reset performance records

    LineEditResetWidget* edit_;

    NetworkEvaluator* evaluator_;

    QList<Processor*> selectedProcessors_;

protected slots:
    void sortMenu();
    void saveSettings();

signals:
    void sort(int);
    void show(int);
    void averageTime(bool);
    void clearRecords();

private:
    bool resetSettings_;
};

class VRN_QT_API PerformanceRecordTreeWidget : public QTreeWidget {
Q_OBJECT
friend class PerformanceRecordWidget;
public:
    enum TimeType {
        TimeTypeTotalProcess,
        TimeTypeBeforeProcess,
        TimeTypeProcess,
        TimeTypeAfterProcess
    };

    PerformanceRecordTreeWidget(PerformanceRecordWidget* performanceRecordWidget, bool averageTime = false, PerformanceRecordTreeWidget::TimeType sortType = PerformanceRecordTreeWidget::TimeTypeTotalProcess, PerformanceRecordTreeWidget::TimeType showType = PerformanceRecordTreeWidget::TimeTypeTotalProcess, bool clearOnUpdate = false, QWidget* parent = 0);

public slots:
    void update();

    void filter(const QString& text);

    /// Sorts the performance records by process time, either total, before, after or the actual process time.
    void sort(int);

    /// Shows the performance record process time, either total, before, after or the actual process time on top.
    void show(int);

    /// Use average or last time measurements.
    void averageTime(bool);

    /// Clear all after dialog confirmation.
    void clearRecords();

protected:
    void mousePressEvent(QMouseEvent*);

private:
    QList<QTreeWidgetItem*> topItems_;
    std::map<std::string, QTreeWidgetItem*> items_;
    PerformanceRecordWidget* performanceRecordWidget_;

    /// Recursively add the samples as new tree items in the list.
    void createSampleHierarchy(PerformanceSample*, QTreeWidgetItem*);

    /// Update ListTreeWidget with all the samples.
    void updateTree();

    void collectVisibleParentPerformanceSamples();
    void clearPerformanceRecords();

    void sortByTotalProcessTime();
    void sortByBeforeProcessTime();
    void sortByProcessTime();
    void sortByAfterProcessTime();

    static bool compare_highest_totalprocess_time(PerformanceSample first, PerformanceSample second) { return first.getChildrenTime() > second.getChildrenTime(); }
    static bool compare_highest_beforeprocess_time(PerformanceSample first, PerformanceSample second) { return first.getChildTime("beforeprocess") > second.getChildTime("beforeprocess"); }
    static bool compare_highest_process_time(PerformanceSample first, PerformanceSample second) { return first.getChildTime("process") > second.getChildTime("process"); }
    static bool compare_highest_afterprocess_time(PerformanceSample first, PerformanceSample second) { return first.getChildTime("afterprocess") > second.getChildTime("afterprocess"); }

    bool clearTreeOnUpdate_;
    bool averageTime_;
    TimeType sortType_;
    TimeType showType_;
    std::string filterText_;

    std::vector<PerformanceSample> performanceSamples_;

};

} //namespace voreen

#endif // VRN_PERFORMANCERECORDWIDGET_H

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

#include "voreen/core/voreenmodule.h"

#include "voreen/qt/widgets/documentationwidget.h"
#include "voreen/qt/widgets/performancerecordwidget.h"
#include "voreen/qt/widgets/lineeditresetwidget.h"
#include "voreen/qt/voreenapplicationqt.h"

#include "voreen/core/utils/stringconversion.h"

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QSplitter>
#include <QStringList>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QMap>
#include <QList>

namespace {
    QString whatsThisInfo = "<h3>Performance Record Widget</h3><p>This widget allows access to the performance data stored in each \
                            processor. Every processor in the network is listed here with timings. On the top level of the tree \
                            the total execution time is shown, but the view can be expanded to show the timings for the \
                            \"beforeProcess\", \"afterProcess\" and \"process\" methods. The next granularity is unique for each \
                            processor and is defined by the processor designer.</p>\
                            <p>The <img src=\":voreenve/icons/configure.png\" width=\"20\"> button provides commonly used actions \
                            for this list.</p>";
}

namespace voreen {

PerformanceRecordWidget::PerformanceRecordWidget(QWidget* parent)
    : QWidget(parent)
    , resetIcon_()
    , splitter_(new QSplitter(Qt::Vertical))
    , resetSettings_(false)
{
    setWhatsThis(whatsThisInfo);

    QPushButton* sortButton = new QPushButton(this);
    sortButton->setIcon(QIcon(":/voreenve/icons/configure.png"));
    sortButton->setGeometry(0,0,32,32);
    sortButton->setFlat(true);
    sortButton->setToolTip(tr("Sort performance records"));

    edit_ = new LineEditResetWidget(this);
    edit_->setToolTip(tr("Filter performance records"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* filterSortLayout = new QHBoxLayout();

    useAverageTime_ = new QAction("Calculate Average Time", this);
    useLastTime_ = new QAction("Use Last Time Record", this);
    useAverageTime_->setCheckable(true);
    useLastTime_->setCheckable(true);

    sortByTotalProcessTime_ = new QAction("Sort by Total Process Time", this);
    sortByBeforeProcessTime_ = new QAction("Sort by Before Process Time", this);
    sortByProcessTime_ = new QAction("Sort by Process Time", this);
    sortByAfterProcessTime_ = new QAction("Sort by After Process Time", this);

    sortByTotalProcessTime_->setCheckable(true);
    sortByBeforeProcessTime_->setCheckable(true);
    sortByProcessTime_->setCheckable(true);
    sortByAfterProcessTime_->setCheckable(true);

    showTotalProcessTime_ = new QAction("Show Total Process Time On Top", this);
    showBeforeProcessTime_ = new QAction("Show Before Process Time On Top", this);
    showProcessTime_ = new QAction("Show Process Time On Top", this);
    showAfterProcessTime_ = new QAction("Show After Process Time On Top", this);
    
    showTotalProcessTime_->setCheckable(true);
    showBeforeProcessTime_->setCheckable(true);
    showProcessTime_->setCheckable(true);
    showAfterProcessTime_->setCheckable(true);

    clearTreeWhenRecordsUpdate_ = new QAction("Clear Records on Update", this);
    clearTreeWhenRecordsUpdate_->setCheckable(true);

    resetPerformanceRecords_ = new QAction("Clear All Performance Records", this);

    loadSettings();

    connect(edit_, SIGNAL(textChanged(const QString&)), tree_, SLOT(filter(const QString&)));
    connect(sortButton, SIGNAL(clicked()), this, SLOT(sortMenu()));
    connect(this, SIGNAL(sort(int)), tree_, SLOT(sort(int)));
    connect(this, SIGNAL(show(int)), tree_, SLOT(show(int)));
    connect(this, SIGNAL(averageTime(bool)), tree_, SLOT(averageTime(bool)));
    connect(this, SIGNAL(clearRecords()), tree_, SLOT(clearRecords()));

    filterSortLayout->addWidget(edit_);
    filterSortLayout->addWidget(sortButton);
    splitter_->addWidget(tree_);
    QList<int> widgetSpacing;
    widgetSpacing << 500 << 100;
    splitter_->setSizes(widgetSpacing);
    mainLayout->addLayout(filterSortLayout);
    mainLayout->addWidget(splitter_);
}

PerformanceRecordWidget::~PerformanceRecordWidget() {
    if (!resetSettings_)
        saveSettings();
}

void PerformanceRecordWidget::setEvaluator(NetworkEvaluator* evaluator) {
    setMouseTracking(false);
    evaluator_ = evaluator;

    if (evaluator_) {
        evaluator_->addProcessWrapper(this);
        if (evaluator_->getProcessorNetwork()) {
            setMouseTracking(true);
        }
    }
}

void PerformanceRecordWidget::afterNetworkProcess() {
    if (isVisible())
        tree_->update();
}

std::vector<const PerformanceRecord*> PerformanceRecordWidget::collectPerformanceRecords() const {
    if (evaluator_)
        return evaluator_->collectPerformanceRecords();
    else{
        std::vector<const PerformanceRecord*> no_records;
        return no_records;
    }
}

void PerformanceRecordWidget::clearPerformanceRecords() {
    if (evaluator_)
        evaluator_->clearPerformanceRecords();
}

void PerformanceRecordWidget::processorsSelected(const QList<Processor*>& processors) {
    selectedProcessors_ = processors;
}

void PerformanceRecordWidget::loadSettings()
{
    bool averageT = false;
    std::string text = "";
    PerformanceRecordTreeWidget::TimeType sortT = PerformanceRecordTreeWidget::TimeTypeTotalProcess;
    PerformanceRecordTreeWidget::TimeType showT = PerformanceRecordTreeWidget::TimeTypeTotalProcess;
    QSettings settings;
    settings.beginGroup("PerformanceRecordWidget");
    if(!settings.contains("uat")) {      // first time load without any data written in the settings
        useLastTime_->setChecked(true);
        sortByTotalProcessTime_->setChecked(true);
        showTotalProcessTime_->setChecked(true);
        clearTreeWhenRecordsUpdate_->setChecked(false);
    }
    else{
        if (settings.value("uat").toBool()) {
            useAverageTime_->setChecked(true);
            averageT = true;
        }
        else if(settings.value("ult").toBool()) {
            useLastTime_->setChecked(true);
        }

        if (settings.value("sbt").toBool()) {
            sortByTotalProcessTime_->setChecked(true);
        }
        else if(settings.value("sbb").toBool()) {
            sortByBeforeProcessTime_->setChecked(true);
            sortT = PerformanceRecordTreeWidget::TimeTypeBeforeProcess;
        }
        else if(settings.value("sbp").toBool()) {
            sortByProcessTime_->setChecked(true);
            sortT = PerformanceRecordTreeWidget::TimeTypeProcess;
        }
        else if(settings.value("sba").toBool()) {
            sortByAfterProcessTime_->setChecked(true);
            sortT = PerformanceRecordTreeWidget::TimeTypeAfterProcess;
        }

        if (settings.value("sht").toBool()) {
            showTotalProcessTime_->setChecked(true);
        }
        else if(settings.value("shb").toBool()) {
            showBeforeProcessTime_->setChecked(true);
            showT = PerformanceRecordTreeWidget::TimeTypeBeforeProcess;
        }
        else if(settings.value("shp").toBool()) {
            showProcessTime_->setChecked(true);
            showT = PerformanceRecordTreeWidget::TimeTypeProcess;
        }
        else if(settings.value("sha").toBool()) {
            showAfterProcessTime_->setChecked(true);
            showT = PerformanceRecordTreeWidget::TimeTypeAfterProcess;
        }

        clearTreeWhenRecordsUpdate_->setChecked(settings.value("cou").toBool());

        text = settings.value("filtertext").toString().toStdString();
    }

    settings.endGroup();

    tree_ = new PerformanceRecordTreeWidget(this, averageT, sortT, showT, clearTreeWhenRecordsUpdate_->isChecked());
    tree_->filterText_ = text;
    edit_->setText(QString::fromStdString(tree_->filterText_));

}

void PerformanceRecordWidget::saveSettings() {
    QSettings settings;
    settings.beginGroup("PerformanceRecordWidget");

    settings.setValue("filtertext", QString::fromStdString(tree_->filterText_));

    settings.setValue("uat", useAverageTime_->isChecked());
    settings.setValue("ult", useLastTime_->isChecked());

    settings.setValue("sbt", sortByTotalProcessTime_->isChecked());
    settings.setValue("sbb", sortByBeforeProcessTime_->isChecked());
    settings.setValue("sbp", sortByProcessTime_->isChecked());
    settings.setValue("sba", sortByAfterProcessTime_->isChecked());

    settings.setValue("sht", showTotalProcessTime_->isChecked());
    settings.setValue("shb", showBeforeProcessTime_->isChecked());
    settings.setValue("shp", showProcessTime_->isChecked());
    settings.setValue("sha", showAfterProcessTime_->isChecked());

    settings.setValue("cou", clearTreeWhenRecordsUpdate_->isChecked());

    settings.endGroup();
}

void PerformanceRecordWidget::sortMenu() {
    QMenu* menu = new QMenu();
    QActionGroup* timeActions = new QActionGroup(this);
    QActionGroup* sortActions = new QActionGroup(this);
    QActionGroup* showActions = new QActionGroup(this);

    timeActions->addAction(useAverageTime_);
    timeActions->addAction(useLastTime_);

    menu->addAction(useAverageTime_);
    menu->addAction(useLastTime_);

    menu->addSeparator();

    sortActions->addAction(sortByTotalProcessTime_);
    sortActions->addAction(sortByBeforeProcessTime_);
    sortActions->addAction(sortByProcessTime_);
    sortActions->addAction(sortByAfterProcessTime_);

    menu->addAction(sortByTotalProcessTime_);
    menu->addAction(sortByBeforeProcessTime_);
    menu->addAction(sortByProcessTime_);
    menu->addAction(sortByAfterProcessTime_);

    menu->addSeparator();

    showActions->addAction(showTotalProcessTime_);
    showActions->addAction(showBeforeProcessTime_);
    showActions->addAction(showProcessTime_);
    showActions->addAction(showAfterProcessTime_);

    menu->addAction(showTotalProcessTime_);
    menu->addAction(showBeforeProcessTime_);
    menu->addAction(showProcessTime_);
    menu->addAction(showAfterProcessTime_);

    menu->addSeparator();

    menu->addAction(clearTreeWhenRecordsUpdate_);

    menu->addSeparator();

    menu->addAction(resetPerformanceRecords_);

    QAction* action = menu->exec(QCursor::pos());
    if (action) {
        if(action == sortByTotalProcessTime_) {
            emit sort(PerformanceRecordTreeWidget::TimeTypeTotalProcess);
        }
        else if (action == sortByBeforeProcessTime_) {
            emit sort(PerformanceRecordTreeWidget::TimeTypeBeforeProcess);
        }
        else if (action == sortByProcessTime_) {
            emit sort(PerformanceRecordTreeWidget::TimeTypeProcess);
        }
        else if (action == sortByAfterProcessTime_) {
            emit sort(PerformanceRecordTreeWidget::TimeTypeAfterProcess);
        }
        if (action == showTotalProcessTime_) {
            emit show(PerformanceRecordTreeWidget::TimeTypeTotalProcess);
        }
        else if (action == showBeforeProcessTime_) {
            emit show(PerformanceRecordTreeWidget::TimeTypeBeforeProcess);
        }
        else if (action == showProcessTime_) {
            emit show(PerformanceRecordTreeWidget::TimeTypeProcess);
        }
        else if (action == showAfterProcessTime_) {
            emit show(PerformanceRecordTreeWidget::TimeTypeAfterProcess);
        }
        if (action == useAverageTime_ || action == useLastTime_) {
            emit averageTime(useAverageTime_->isChecked());
        }
        if (action == clearTreeWhenRecordsUpdate_) {
            tree_->clearTreeOnUpdate_ = clearTreeWhenRecordsUpdate_->isChecked();
        }
        if (action == resetPerformanceRecords_) {
            emit clearRecords();
        }
        saveSettings();
    }
}

void PerformanceRecordWidget::resetSettings() {
    QSettings settings;
    settings.remove("PerformanceRecordWidget");

    resetSettings_ = true;
}

// ----------------------------------------------------------------------------

PerformanceRecordTreeWidget::PerformanceRecordTreeWidget(PerformanceRecordWidget* performanceRecordWidget, bool averageTime, PerformanceRecordTreeWidget::TimeType sortType, PerformanceRecordTreeWidget::TimeType showType, bool clearOnUpdate, QWidget* parent)
    : QTreeWidget(parent)
    , performanceRecordWidget_(performanceRecordWidget)
    , averageTime_(averageTime)
    , sortType_(sortType)
    , showType_(showType)
    , clearTreeOnUpdate_(clearOnUpdate)
{
    QStringList headeritems = QStringList();
    headeritems << "Processor" << "Time (ms)";
    setHeaderLabels(headeritems);

    header()->setResizeMode(0, QHeaderView::Stretch);
    header()->setResizeMode(1, QHeaderView::ResizeToContents);
    header()->setStretchLastSection(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setColumnCount(2);
}

void PerformanceRecordTreeWidget::update() {
    if(sortType_ == PerformanceRecordTreeWidget::TimeTypeTotalProcess)
        sortByTotalProcessTime();
    else if(sortType_ == PerformanceRecordTreeWidget::TimeTypeBeforeProcess)
        sortByBeforeProcessTime();
    else if(sortType_ == PerformanceRecordTreeWidget::TimeTypeProcess)
        sortByProcessTime();
    else if(sortType_ == PerformanceRecordTreeWidget::TimeTypeAfterProcess)
        sortByAfterProcessTime();
}

void PerformanceRecordTreeWidget::filter(const QString& text) {
    topItems_.clear();
    items_.clear();
    clear();
    filterText_ = text.toStdString();
    update();
}

void PerformanceRecordTreeWidget::createSampleHierarchy(PerformanceSample* parentSample, QTreeWidgetItem* parentItem) {
    std::vector<PerformanceSample*> theChildren = parentSample->getChildren();
    QString numStr;
    QTreeWidgetItem* sampleItem;
    std::map<std::string, QTreeWidgetItem*>::iterator sampleMapIterator;
    std::string sampleName;
    for (size_t i = 0; i < theChildren.size(); ++i) {
        numStr.sprintf("%4.4f", theChildren[i]->getTime()*1000.f);
        sampleName = parentSample->getName()+"."+theChildren[i]->getName();

        sampleMapIterator = items_.lower_bound(sampleName);
        if(sampleMapIterator != items_.end() && !(items_.key_comp()(sampleName, sampleMapIterator->first))){ //exists
            sampleItem = sampleMapIterator->second;
            sampleItem->setText(1, numStr);
        }
        else{
            sampleItem = new QTreeWidgetItem(QStringList() << QString::fromStdString(theChildren[i]->getName()) << numStr);
            items_.insert(std::pair<std::string, QTreeWidgetItem*>(sampleName, sampleItem));
            parentItem->addChild(sampleItem);
        }

        createSampleHierarchy(theChildren[i], sampleItem);
    }
}

void PerformanceRecordTreeWidget::updateTree() {
    // clear top level items
    topItems_.clear();
    if(clearTreeOnUpdate_){
        items_.clear();
        clear();
    }

    // create/update tree item
    QString numStr;
    QTreeWidgetItem* recordItem;
    std::map<std::string, QTreeWidgetItem*>::iterator sampleMapIterator;
    PerformanceSample* theSample;
    std::string nameStr = "";
    std::string subStr = "";
    float num = 0.f;
    for (size_t i = 0; i < performanceSamples_.size(); ++i) {
        nameStr = performanceSamples_[i].getName();
        if(showType_ == PerformanceRecordTreeWidget::TimeTypeTotalProcess){
            theSample = &performanceSamples_[i];
            num = theSample->getChildrenTime();
        }
        else{
            if(showType_ == PerformanceRecordTreeWidget::TimeTypeBeforeProcess)
                subStr = "beforeprocess";
            else if(showType_ == PerformanceRecordTreeWidget::TimeTypeProcess)
                subStr = "process";
            else
                subStr = "afterprocess";

            theSample = performanceSamples_[i].getChild(subStr);
            num = theSample->getTime();
            nameStr += "." + subStr;
        }

        numStr.sprintf("%4.4f", num*1000.f);

        sampleMapIterator = items_.lower_bound(nameStr);
        if(sampleMapIterator != items_.end() && !(items_.key_comp()(nameStr, sampleMapIterator->first))){ //exists
            recordItem = sampleMapIterator->second;
            recordItem->setText(1, numStr);
            createSampleHierarchy(theSample, recordItem);
        }
        else{ //doesn't exist
            recordItem = new QTreeWidgetItem(QStringList() << QString::fromStdString(nameStr) << numStr);
            items_.insert(std::pair<std::string, QTreeWidgetItem*>(nameStr, recordItem));
            createSampleHierarchy(theSample, recordItem);
        }
        topItems_.append(recordItem);
    }

    addTopLevelItems(topItems_);
}

void PerformanceRecordTreeWidget::sortByTotalProcessTime() {
    sortType_ = PerformanceRecordTreeWidget::TimeTypeTotalProcess;

    // collect performance records
    collectVisibleParentPerformanceSamples();

    // sort items by total process time
    std::sort(performanceSamples_.begin(), performanceSamples_.end(), compare_highest_totalprocess_time);

    // update tree
    updateTree();
}

void PerformanceRecordTreeWidget::sortByBeforeProcessTime() {
    sortType_ = PerformanceRecordTreeWidget::TimeTypeBeforeProcess;
    
    // collect performance records
    collectVisibleParentPerformanceSamples();

    // sort items by total process time
    std::sort(performanceSamples_.begin(), performanceSamples_.end(), compare_highest_beforeprocess_time);

    // update tree
    updateTree();
}

void PerformanceRecordTreeWidget::sortByProcessTime() {
    sortType_ = PerformanceRecordTreeWidget::TimeTypeProcess;
    
    // collect performance records
    collectVisibleParentPerformanceSamples();

    // sort items by total process time
    std::sort(performanceSamples_.begin(), performanceSamples_.end(), compare_highest_process_time);

    // update tree
    updateTree();
}

void PerformanceRecordTreeWidget::sortByAfterProcessTime() {
    sortType_ = PerformanceRecordTreeWidget::TimeTypeAfterProcess;
    
    // collect performance records
    collectVisibleParentPerformanceSamples();

    // sort items by total process time
    std::sort(performanceSamples_.begin(), performanceSamples_.end(), compare_highest_afterprocess_time);

    // update tree
    updateTree();
}

void PerformanceRecordTreeWidget::collectVisibleParentPerformanceSamples() {
    std::map<std::string, PerformanceSample> tempSampleMap;
    std::map<std::string, PerformanceSample>::iterator tempSampleMapIterator;
    std::vector<PerformanceSample*>::const_reverse_iterator sampleReverseVectorIterator;
    std::string lastName;
    if(performanceRecordWidget_) {
        std::vector<const PerformanceRecord*> records = performanceRecordWidget_->collectPerformanceRecords();
        performanceSamples_.clear();
        foreach(const PerformanceRecord* record, records) {
            if(QString::fromStdString(record->getName()).indexOf(QString::fromStdString(filterText_), 0, Qt::CaseInsensitive) != -1){
                 tempSampleMap.clear();
                 const std::vector<PerformanceSample*> samples = record->getSamples();
                 PerformanceSample parent_sample(NULL, record->getName());
                 if(!averageTime_){
                     if(samples.size()>0){
                         sampleReverseVectorIterator = samples.rbegin();
                         lastName = samples.back()->getName();
                         while(sampleReverseVectorIterator < samples.rend() && (samples.rbegin() == sampleReverseVectorIterator || (*sampleReverseVectorIterator)->getName() != lastName)){
                            parent_sample.addChild(*(*sampleReverseVectorIterator));
                             ++sampleReverseVectorIterator;
                         }
                     }
                 }
                 else{
                     foreach(PerformanceSample* sample, samples){
                        tempSampleMapIterator = tempSampleMap.lower_bound(sample->getName());
                        if(tempSampleMapIterator != tempSampleMap.end() && !(tempSampleMap.key_comp()(sample->getName(), tempSampleMapIterator->first))){
                            tempSampleMapIterator->second = tempSampleMapIterator->second + (*sample);
                        }
                        else {
                            tempSampleMap.insert(std::pair<std::string, PerformanceSample>(sample->getName(), (*sample)));
                        }
                     }
                     for (tempSampleMapIterator=tempSampleMap.begin() ; tempSampleMapIterator != tempSampleMap.end(); tempSampleMapIterator++){
                         PerformanceSample* child_sample = parent_sample.addChild(tempSampleMapIterator->second);
                         child_sample->normalize();
                     }
                 }
                 performanceSamples_.push_back(parent_sample);
            }
        }
    }
}

void PerformanceRecordTreeWidget::clearPerformanceRecords() {
    performanceRecordWidget_->clearPerformanceRecords();
    topItems_.clear();
    items_.clear();
    performanceSamples_.clear();
    clear();
}

void PerformanceRecordTreeWidget::mousePressEvent(QMouseEvent *event) {
    QTreeWidget::mousePressEvent(event);
}

void PerformanceRecordTreeWidget::clearRecords() {
    QMessageBox msgBox;
    msgBox.setText("Delete performance records.");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setInformativeText("Do you want to perform this operation?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes)
        clearPerformanceRecords();
}

void PerformanceRecordTreeWidget::averageTime(bool value) {
    averageTime_ = value;
    update();
}

void PerformanceRecordTreeWidget::sort(int type) {
    topItems_.clear();
    items_.clear();
    clear();
    sortType_ = static_cast<PerformanceRecordTreeWidget::TimeType>(type);
    update();
}

void PerformanceRecordTreeWidget::show(int type) {
    topItems_.clear();
    items_.clear();
    clear();
    showType_ = static_cast<PerformanceRecordTreeWidget::TimeType>(type);
    update();
}

} //namespace voreen

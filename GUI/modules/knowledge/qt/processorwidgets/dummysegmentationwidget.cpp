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

#include "dummysegmentationwidget.h"
#include "voreen/qt/voreenapplicationqt.h"

#include <QGridLayout>
#include <QMainWindow>
#include <QLabel>
#include <QtGui>


namespace voreen {

const std::string DummySegmentationWidget::loggerCat_("voreen.DummySegmentationWidget");

DummySegmentationWidget::DummySegmentationWidget(QWidget* parent, DummySegmentationProcessor* dummySegmentationProcessor)
    : QProcessorWidget(dummySegmentationProcessor, parent)
{
    tgtAssert(dummySegmentationProcessor, "No DummySegmentationProcessor processor");

    setWindowTitle(QString::fromStdString(dummySegmentationProcessor->getName()));
    resize(400, 280);
    setPosition(20,20);
}

DummySegmentationWidget::~DummySegmentationWidget() {
    //delete plugin_;
}

void DummySegmentationWidget::initialize() {
    QProcessorWidget::initialize();

    DummySegmentationProcessor* dummySegmentationProcessor = dynamic_cast<DummySegmentationProcessor*>(processor_);

    QString s = dummySegmentationProcessor->getName().c_str();
    QLabel* label = new QLabel();
    label->setText(s);
    const QString dummyXML = "<regions>" \
                     "<region id=\"1\"><msavi>1.23</msavi></region>"\
                     "<region id=\"2\"><msavi>2.33</msavi></region>"\
                     "<region id=\"3\"><msavi>3.33</msavi></region>"\
                     "<region id=\"4\"><msavi>4.33</msavi></region>"\
                     "<region id=\"5\"><msavi>5.33</msavi></region>"\
                     "<region id=\"6\"><msavi>6.33</msavi></region>"\
                     "<region id=\"7\"><msavi>7.33</msavi></region>"\
                     "<region id=\"8\"><msavi>8.33</msavi></region>"\
                     "<region id=\"9\"><msavi>9.33</msavi></region>"\
                     "<region id=\"10\"><msavi>10.33</msavi></region>"\
                     "</regions>";
    QPlainTextEdit* editor = new QPlainTextEdit();
    editor->appendPlainText(dummyXML);
    //plugin_ = new ShaderPlugin(dummySegmentationProcessor->getShader(), parentWidget());
    //plugin_->createWidgets();
    //plugin_->createConnections();

    ////connect(plugin_, SIGNAL(modified()), this, SIGNAL(modified()));

    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(label);
    layout->addWidget(editor);
    //layout->addWidget(plugin_);


    setLayout(layout);

    //show();
    hide();

    initialized_ = true;
}

void DummySegmentationWidget::updateFromProcessor() {
    //plugin_->updateFromProperty();
}

} //namespace voreen


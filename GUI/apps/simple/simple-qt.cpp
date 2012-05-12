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

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>

#include "tgt/init.h"
#include "tgt/logmanager.h"
#include "tgt/qt/qtcanvas.h"
#include "tgt/camera.h"
#include "tgt/shadermanager.h"

#include "voreen/core/utils/voreenpainter.h"
#include "voreen/core/io/serialization/serialization.h"
#include "voreen/core/network/networkevaluator.h"
#include "voreen/core/network/workspace.h"
#include "voreen/core/network/processornetwork.h"
#include "voreen/core/processors/canvasrenderer.h"
#include "voreen/qt/voreenapplicationqt.h"

using namespace voreen;

/*
    This is a small program that demonstrates a little of what the voreen_core lib can do: We load a
    dataset which can be rotated and zoomed using the mouse.  This is the qt-version of this sample,
    there are others in the simple/-folder, like a glut-version.
*/
int main(int argc, char* argv[]) {
    // init both Qt and Voreen application (does not require OpenGL context)
    QApplication myapp(argc, argv);
    VoreenApplicationQt vapp("simple-qt", "Simple-Qt", argc, argv, 
        VoreenApplication::ApplicationFeatures(VoreenApplication::APP_ALL &~ VoreenApplication::APP_PROCESSOR_WIDGETS));
    vapp.initialize();

    // create the mainwindow and assign a canvas to it as central widget
    QMainWindow* mainwindow = new QMainWindow();
    mainwindow->setWindowTitle("Voreen - The Volume Rendering Engine (Simple-Qt)");
    VoreenApplicationQt::qtApp()->setMainWindow(mainwindow);
    tgt::QtCanvas* canvas = new tgt::QtCanvas();
    mainwindow->setCentralWidget(canvas);
    mainwindow->resize(512, 512);
    mainwindow->show();

    // must be called *after* a OpenGL context has been created (canvas)
    // and *before* any further OpenGL access
    vapp.initializeGL();

    // load workspace from disc
    Workspace* workspace = new Workspace();
    try {
        workspace->load(VoreenApplication::app()->getWorkspacePath("/standard.vws"));
    }
    catch (SerializationException& e) {
        QMessageBox::critical(mainwindow, "Loading Workspace Failed", QString::fromStdString(e.what()));
        return EXIT_FAILURE;
    }

    // initialize the network evaluator and retrieve the CanvasRenderer processor from the loaded network
    NetworkEvaluator* networkEvaluator = new NetworkEvaluator();
    ProcessorNetwork* network = workspace->getProcessorNetwork();
    std::vector<CanvasRenderer*> canvasRenderer = network->getProcessorsByType<CanvasRenderer>();
    if (canvasRenderer.empty()) {
        QMessageBox::critical(mainwindow, "Invalid Workspace", "Loaded workspace does not contain a CanvasRenderer.");
        return EXIT_FAILURE;
    }

    // init painter and connect it to canvas, evaluator and canvas renderer
    VoreenPainter* painter = new VoreenPainter(canvas, networkEvaluator, canvasRenderer[0]);
    canvas->setPainter(painter);
    canvasRenderer[0]->setCanvas(canvas);

    // pass the network to the network evaluator, which also initializes the processors
    networkEvaluator->setProcessorNetwork(network);

    // start the event process; the program runs as long as theres no exit-event
    myapp.exec();

    // we're done as soon as myapp.exec() returns, so we can delete everything
    delete painter;
    delete workspace;
    delete networkEvaluator;
    delete mainwindow;

    vapp.deinitializeGL();
    vapp.deinitialize();
    return 0;
}

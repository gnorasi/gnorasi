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

#include "voreen/qt/widgets/animation/animationexportwidget.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/canvasrenderer.h"
#include "voreen/core/network/networkevaluator.h"

#include <QApplication>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTimeEdit>

namespace voreen {

AnimationExportWidget::AnimationExportWidget(QWidget* parent, Animation* animation, NetworkEvaluator* network,
                                             float duration, int startframe, int endframe, float stretchFactor)
    : QDialog(parent)
    , animation_(animation)
    , network_(network)
    , duration_(duration)
    , startframe_(startframe)
    , endframe_(endframe)
    , canvas_(0)
    , painter_(0)
    , canvasSize_(256, 256)
    , renderingVideo_(false)
    , recordPathName_("")
    , fpsFactor_(stretchFactor)
    , renderState_(Inactive)
{
    setWindowTitle(tr("Export Animation"));
    setObjectName(tr("Export Optionen"));
    createWidgets();
    createConnections();
}

AnimationExportWidget::~AnimationExportWidget(){
    comboCanvases_->disconnect();
    comboCamerasCubeMap_->disconnect();
    comboCamerasStereo_->disconnect();
    renderBox_->disconnect();
}
void AnimationExportWidget::networkChanged() {
    if (network_ == 0)
        return;

    canvas_ = 0;
    painter_ = 0;
    allCanvases_.clear();
    allCameraPropertys_.clear();

    const std::vector<Processor*>& processors = network_->getProcessorNetwork()->getProcessors();
    for (size_t i = 0; i < processors.size(); ++i) {
        std::vector<CameraProperty*> camPropsProcessor = processors[i]->getPropertiesByType<CameraProperty>();
        if (!camPropsProcessor.empty())
            allCameraPropertys_.insert(std::make_pair(camPropsProcessor[0], processors[i]->getName()));

        CanvasRenderer* cr = dynamic_cast<CanvasRenderer*>(processors[i]);
        if (cr != 0) {
            tgt::QtCanvas* canvas = dynamic_cast<tgt::QtCanvas*>(cr->getCanvas());
            if (canvas != 0)
                allCanvases_.insert(std::make_pair(canvas, cr->getName()));
        }
    }

    if (!allCanvases_.empty()) {
        canvas_ = allCanvases_.begin()->first;
        canvasSize_ = canvas_->getSize();
        if (canvas_ != 0)
            painter_ = dynamic_cast<VoreenPainter*>(canvas_->getPainter());
    }

    refreshComboBoxes();
}

void AnimationExportWidget::controlledCanvasChanged(int index) {
    if ((comboCanvases_ == 0) || (index < 0) || (index >= comboCanvases_->count()))
        return;
    canvas_ = reinterpret_cast<tgt::QtCanvas*>(comboCanvases_->itemData(index).toULongLong());
    if (canvas_ != 0)
        painter_ = dynamic_cast<VoreenPainter*>(canvas_->getPainter());

}

void AnimationExportWidget::createWidgets(){

    mainLayout_ = new QVBoxLayout();
    renderBox_ = createAnimationRenderBox(this);
    mainLayout_->addWidget(renderBox_);

    setLayout(mainLayout_);
    setWidgetState();
}

void AnimationExportWidget::closeEvent(QCloseEvent* e) {
    currentFrame_ = static_cast<int>(duration_ / fpsFactor_ + 1);
    endRendering();
    QDialog::closeEvent(e);
}

void AnimationExportWidget::createConnections() {
    connect(comboCanvases_, SIGNAL(currentIndexChanged(int)), this, SLOT(controlledCanvasChanged(int)));
    connect(saveAsFrameSequenceButton_, SIGNAL(clicked()), this, SLOT(recordAnimationFrameSeq()));
    connect(saveAsVideoButton_, SIGNAL(clicked()), this, SLOT(recordAnimationVideo()));
    connect(saveSpecificFrame_, SIGNAL(clicked()), this, SLOT(saveSpecificFrame()));
    connect(videoSetupButton_, SIGNAL(clicked()), this, SLOT(videoSetup()));
    connect(cubeMapImages_, SIGNAL(clicked()), this, SLOT(setWidgetState()));
    connect(stereoImages_, SIGNAL(clicked()), this, SLOT(setWidgetState()));
    connect(spinStartTime_, SIGNAL(timeChanged(const QTime&)), this, SLOT(updateFramesSpinBox()));
    connect(spinEndTime_, SIGNAL(timeChanged(const QTime&)), this, SLOT(updateFramesSpinBox()));
    connect(spinRecordingFPS_, SIGNAL(valueChanged(int)), this, SLOT(updateFramesSpinBox()));
    connect(setSpecificFrameAsFirst_, SIGNAL(clicked()), this, SLOT(setFirstFrameAsSpecificFrame()));
    connect(setSpecificFrameAsLast_, SIGNAL(clicked()), this, SLOT(setLastFrameAsSpecificFrame()));
}

void AnimationExportWidget::setWidgetState() {
    comboCanvases_->setEnabled(true);
    comboCamerasCubeMap_->setEnabled(cubeMapImages_->isChecked());
    comboCamerasStereo_->setEnabled(stereoImages_->isChecked());
    cubeMapFront_->setEnabled(cubeMapImages_->isChecked());
    cubeMapBack_->setEnabled(cubeMapImages_->isChecked());
    cubeMapLeft_->setEnabled(cubeMapImages_->isChecked());
    cubeMapRight_->setEnabled(cubeMapImages_->isChecked());
    cubeMapTop_->setEnabled(cubeMapImages_->isChecked());
    cubeMapBottom_->setEnabled(cubeMapImages_->isChecked());
    eyeSeparation_->setEnabled(stereoImages_->isChecked());
    eyeSeparationLabel_->setEnabled(stereoImages_->isChecked());
    saveAsVideoButton_->setEnabled(!cubeMapImages_->isChecked() && !stereoImages_->isChecked());
    videoSetupButton_->setEnabled(!cubeMapImages_->isChecked() && !stereoImages_->isChecked());
}

void AnimationExportWidget::refreshComboBoxes() {
    if (comboCanvases_ != 0) {
        comboCanvases_->clear();
        for (CanvasMap::const_iterator it = allCanvases_.begin(); it != allCanvases_.end(); ++it)
            comboCanvases_->addItem(tr(it->second.c_str()), reinterpret_cast<qulonglong>(it->first));
    }
    if (comboCamerasCubeMap_ != 0) {
        comboCamerasCubeMap_->clear();
        for (CameraPropertyMap::const_iterator it = allCameraPropertys_.begin(); it != allCameraPropertys_.end(); ++it)
            comboCamerasCubeMap_->addItem(tr(it->second.c_str()), reinterpret_cast<qulonglong>(it->first));
    }
    if (comboCamerasStereo_ != 0) {
        comboCamerasStereo_->clear();
        for (CameraPropertyMap::const_iterator it = allCameraPropertys_.begin(); it != allCameraPropertys_.end(); ++it)
            comboCamerasStereo_->addItem(tr(it->second.c_str()), reinterpret_cast<qulonglong>(it->first));
    }
}

void AnimationExportWidget::updateFramesSpinBox(){
    bool lastFrameIsChoosen = (specificFrame_->value() == specificFrame_->maximum());
    specificFrame_->setMaximum(spinStartTime_->time().secsTo(spinEndTime_->time())*spinRecordingFPS_->value());
    if(lastFrameIsChoosen)
        setLastFrameAsSpecificFrame();
}

void AnimationExportWidget::setFirstFrameAsSpecificFrame(){
    specificFrame_->setValue(std::max(specificFrame_->minimum(), 1));
}

void AnimationExportWidget::setLastFrameAsSpecificFrame(){
    specificFrame_->setValue(specificFrame_->maximum());
}

QGroupBox* AnimationExportWidget::createAnimationRenderBox(QWidget* parent) {
    QGroupBox* recordBox = new QGroupBox(tr("Recording"), parent);
    QVBoxLayout* layout = new QVBoxLayout();
    
    QHBoxLayout* canvasOptLayout = new QHBoxLayout();
    comboCanvases_ = new QComboBox(recordBox);
    canvasOptLayout->addWidget(new QLabel(tr("Canvas:"), recordBox));
    canvasOptLayout->addWidget(comboCanvases_);
    layout->addLayout(canvasOptLayout);

    // render meta settings
    QHBoxLayout* videoOptLayout = new QHBoxLayout();
    videoOptLayout->addWidget(new QLabel(tr("FPS:")));
    spinRecordingFPS_ = new QSpinBox(recordBox);
    spinRecordingFPS_->setRange(1, 120);
    spinRecordingFPS_->setValue(25);
    videoOptLayout->addWidget(spinRecordingFPS_);
    videoOptLayout->addStretch();

    videoOptLayout->addWidget(new QLabel(("     Video Dimensions:")), 1, 0);
    spinWidth_ = new QSpinBox(recordBox);
    spinHeight_ = new QSpinBox(recordBox);

    spinWidth_->setRange(32, 4096);
    spinWidth_->setSingleStep(4);
    spinWidth_->setValue(512);
    spinWidth_->setAccelerated(true);

    spinHeight_->setRange(32, 4096);
    spinHeight_->setSingleStep(4);
    spinHeight_->setValue(512);
    spinHeight_->setAccelerated(true);

    videoOptLayout->addWidget(spinWidth_);
    videoOptLayout->addWidget(new QLabel(" x ", 0, 0));
    videoOptLayout->addWidget(spinHeight_);
    layout->addLayout(videoOptLayout);

    QTime temp(0, 0, 0, 0);
    QTime duration = temp.addSecs((int)duration_/30);

    QHBoxLayout* timeOptLayout = new QHBoxLayout();
    spinStartTime_ = new QTimeEdit(QTime(0, 0 , 0, 0));
    spinEndTime_ = new QTimeEdit(duration);
    spinStartTime_->setDisplayFormat("mm:ss");
    spinEndTime_->setDisplayFormat("mm:ss");
    timeOptLayout->addWidget(new QLabel("Start Time: "));
    timeOptLayout->addWidget(spinStartTime_);

    spinStartTime_->setMaximumTime(duration);
    spinEndTime_->setMaximumTime(duration);

    timeOptLayout->addWidget(new QLabel("End Time: "));
    timeOptLayout->addWidget(spinEndTime_);
    layout->addLayout(timeOptLayout);

    // render 2 vid or frameSeq
    saveAsFrameSequenceButton_ = new QPushButton(tr("Save as frame sequence"));
    saveAsVideoButton_ = new QPushButton(tr("Save as video file"));
    saveSpecificFrame_ = new QPushButton(tr("Save a specific frame:"));
    videoSetupButton_ = new QPushButton(tr("Setup video..."));
    cubeMapImages_ = new QCheckBox(tr("Cube Maps From Camera In:"));
    cubeMapFront_ = new QCheckBox(tr("Front"));
    cubeMapBack_ = new QCheckBox(tr("Back"));
    cubeMapLeft_ = new QCheckBox(tr("Left"));
    cubeMapRight_ = new QCheckBox(tr("Right"));
    cubeMapTop_ = new QCheckBox(tr("Top"));
    cubeMapBottom_ = new QCheckBox(tr("Bottom"));
    cubeMapFront_->setChecked(true);
    cubeMapBack_->setChecked(true);
    cubeMapLeft_->setChecked(true);
    cubeMapRight_->setChecked(true);
    cubeMapTop_->setChecked(true);
    cubeMapBottom_->setChecked(true);
    stereoImages_ = new QCheckBox(tr("Stereo Images From Camera In:"));
#ifndef VRN_MODULE_FFMPEG
    saveAsVideoButton_->setVisible(false);
    videoSetupButton_->setVisible(false);
#endif
    specificFrame_ = new QSpinBox(recordBox);
    specificFrame_->setMinimum(1);
    updateFramesSpinBox();
    setSpecificFrameAsFirst_ = new QToolButton(recordBox);
    setSpecificFrameAsLast_ = new QToolButton(recordBox);
    setSpecificFrameAsFirst_->setArrowType(Qt::LeftArrow);
    setSpecificFrameAsLast_->setArrowType(Qt::RightArrow);

    QHBoxLayout* cubeMapsStartLayout = new QHBoxLayout();
    cubeMapsStartLayout->addWidget(cubeMapImages_);
    comboCamerasCubeMap_ = new QComboBox(recordBox);
    cubeMapsStartLayout->addWidget(comboCamerasCubeMap_);
    layout->addLayout(cubeMapsStartLayout);

    QGridLayout *cubeMapsLayout = new QGridLayout();
    cubeMapsLayout->addWidget(cubeMapFront_, 0, 0);
    cubeMapsLayout->addWidget(cubeMapBack_, 1, 0);
    cubeMapsLayout->addWidget(cubeMapLeft_, 0, 1);
    cubeMapsLayout->addWidget(cubeMapRight_, 1, 1);
    cubeMapsLayout->addWidget(cubeMapTop_, 0, 2);
    cubeMapsLayout->addWidget(cubeMapBottom_, 1, 2);
    layout->addLayout(cubeMapsLayout);

    QHBoxLayout* stereoImagesStartLayout = new QHBoxLayout();
    stereoImagesStartLayout->addWidget(stereoImages_);
    comboCamerasStereo_ = new QComboBox(recordBox);
    stereoImagesStartLayout->addWidget(comboCamerasStereo_);
    layout->addLayout(stereoImagesStartLayout);

    QHBoxLayout* stereoImagesLayout = new QHBoxLayout();
    eyeSeparation_ = new QDoubleSpinBox(recordBox);
    eyeSeparation_->setMinimum(0.0);
    eyeSeparation_->setMaximum(20.0);
    eyeSeparation_->setSingleStep(0.5);
    eyeSeparation_->setValue(2.5);
    eyeSeparationLabel_ = new QLabel("Eye Separation (cm): ", recordBox);
    stereoImagesLayout->addWidget(eyeSeparationLabel_);
    stereoImagesLayout->addWidget(eyeSeparation_);
    layout->addLayout(stereoImagesLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveAsFrameSequenceButton_);
    buttonLayout->addWidget(saveAsVideoButton_);
    buttonLayout->addWidget(videoSetupButton_);
    layout->addLayout(buttonLayout);

    QHBoxLayout* specificLayout = new QHBoxLayout();
    specificLayout->addWidget(saveSpecificFrame_);
    specificLayout->addWidget(specificFrame_);
    specificLayout->addWidget(setSpecificFrameAsFirst_);
    specificLayout->addWidget(setSpecificFrameAsLast_);
    layout->addLayout(specificLayout);

    recordBox->setLayout(layout);
    return recordBox;
}

void AnimationExportWidget::recordAnimationFrameSeq() {
    recordAnimation(false);
}

void AnimationExportWidget::recordAnimationVideo() {
    recordAnimation(true);
}

void AnimationExportWidget::saveSpecificFrame() {
    renderSpecificFrame_ = true;
    recordAnimation(false);
    renderSpecificFrame_ = false;
}

void AnimationExportWidget::recordAnimation(bool recordVideo) {
    renderingVideo_ = recordVideo;
    QStringList fileList;
    if (renderingVideo_ == false) {
        renderState_= Snapshot;
        QString s = QFileDialog::getExistingDirectory(this, tr("Select Output Directory"),
                                                      VoreenApplication::app()->getDocumentsPath().c_str());

        if (!s.isEmpty())
            fileList.push_back(s);
    }
#ifdef VRN_MODULE_FFMPEG
    else {
        renderState_= Recording;
        std::vector<std::string> formats = VideoEncoder::getSupportedFormatsByFileEnding();
        std::vector<std::string> descs = VideoEncoder::getSupportedFormatDescriptions();
        QStringList lstFormats;
        for (size_t i = 0; i < formats.size(); ++i) {
            formats[i] = (descs[i] + " (*." + formats[i] + ")");
            lstFormats.append(formats[i].c_str());
        }

        QFileDialog dialog(this);
        dialog.setDefaultSuffix("avi");
        dialog.setWindowTitle(tr("Export As Video File"));
        dialog.setDirectory(VoreenApplication::app()->getDocumentsPath().c_str());
        #if QT_VERSION >= 0x040400
        dialog.setNameFilters(lstFormats);
        #endif
        dialog.setAcceptMode(QFileDialog::AcceptSave);

        if (dialog.exec())
            fileList = dialog.selectedFiles();
    }
#endif // VRN_MODULE_FFMPEG
    if (fileList.size() > 0) {
        recordPathName_ = fileList.first().toStdString();
        startRendering();
    }
}

void AnimationExportWidget::startRendering() {
    // gather some useful params
    if(renderSpecificFrame_){
        fps_ = 1;
        duration_ = startframe_ = endframe_ = currentFrame_ = specificFrame_->value();
    }
    else{
        fps_ = spinRecordingFPS_->value();
        startframe_= fps_ * (spinStartTime_->time().hour() * 3600 + spinStartTime_->time().minute()*60+ spinStartTime_->time().second());
        endframe_ = fps_ * (spinEndTime_->time().hour() * 3600 + spinEndTime_->time().minute()*60+ spinEndTime_->time().second());
        currentFrame_ = startframe_;
        duration_= endframe_;
    }
 
    // set accurate recording dimension
    canvasSize_ = canvas_->getSize();
    canvas_->resize(spinWidth_->value(), spinHeight_->value());
    qApp->processEvents();

#ifdef VRN_MODULE_FFMPEG
    if (renderState_== Recording) {
        tgt::Texture* texture_ = painter_->getCanvasRenderer()->getImageColorTexture();
        try {
            ffmpegEncoder_.startVideoEncoding(recordPathName_.c_str(), fps_, spinWidth_->value(), spinHeight_->value(),
                                              texture_->getFormat(), texture_->getDataType());
        }
        catch (tgt::Exception& e) {
            QMessageBox::critical(this, tr("Video Export Failed"),
                                  tr("Failed to initialize video export:\n%1").arg(e.what()));
            return;
        }
    }
#endif // VRN_MODULE_FFMPEG

    camForCubeMaps_ = 0;
    if(cubeMapImages_->isChecked() && comboCamerasCubeMap_->count() > 0){
        camForCubeMaps_ = reinterpret_cast<CameraProperty*>(comboCamerasCubeMap_->itemData(comboCamerasCubeMap_->currentIndex()).toULongLong());
    }

    camForStereoImages_ = 0;
    if(stereoImages_->isChecked() && comboCamerasStereo_->count() > 0){
        camForStereoImages_ = reinterpret_cast<CameraProperty*>(comboCamerasStereo_->itemData(comboCamerasStereo_->currentIndex()).toULongLong());
    }

    setWidgetState();
    canvas_->resize(spinWidth_->value(), spinHeight_->value());
    QProgressDialog progress(tr("Exporting Animation..."), tr("Abort"), 0, (int)(duration_ /fpsFactor_), this);
    progress.setWindowTitle(tr("Exporting"));
    if (fps_ > 0) {
        for(int i = 0; i < duration_ /fpsFactor_; ++i) {
            renderingStep();
            progress.setValue(i);
            qApp->processEvents();

            if (currentFrame_ > duration_ / fpsFactor_ || progress.wasCanceled()){
                break;
            }
        }
        endRendering();
    }
}

// rotate the camera about an arbitrary axis and angle
void AnimationExportWidget::rotateView(CameraProperty* camProp, float angle, const tgt::vec3& axis, const tgt::vec3& camPos, const tgt::vec3& camLook) {  
    float cosA = cos(angle);
    float sinA = sin(angle);
    float ux2 = axis[0]*axis[0];
    float uy2 = axis[1]*axis[1];
    float uz2 = axis[2]*axis[2];
    float ux_uy = axis[0]*axis[1];
    float ux_uz = axis[0]*axis[2];
    float uy_uz = axis[1]*axis[2];
    tgt::mat3 mat(cosA + ux2*(1-cosA), ux_uy*(1-cosA)-axis[2]*sinA, ux_uz*(1-cosA)+axis[1]*sinA, ux_uy*(1-cosA)+axis[2]*sinA, cosA+uy2*(1-cosA), uy_uz*(1-cosA)-axis[0]*sinA, ux_uz*(1-cosA)-axis[1]*sinA, uy_uz*(1-cosA)+axis[0]*sinA, cosA+uz2*(1-cosA));
    tgt::vec3 look = mat*camLook;

    // set new focus-point
    camProp->setFocus(camPos + look);
}

void AnimationExportWidget::renderingStep(){
    if (animation_ == 0)
        return;
    tgtAssert(canvas_, "No canvas");

    if (currentFrame_ > duration_ / fpsFactor_){
        endRendering();
    }
    else {
        animation_->renderAt((float)currentFrame_*fpsFactor_/ fps_);
        #ifdef VRN_MODULE_FFMPEG
        if ((renderState_== Recording) &&(painter_->getCanvasRenderer())) {
            if (canvas_->getSize() != tgt::ivec2(spinWidth_->value(), spinHeight_->value())) {
                canvas_->resize(spinWidth_->value(), spinHeight_->value());
                canvas_->repaint();
            }

            tgt::Texture* texture = painter_->getCanvasRenderer()->getImageColorTexture();
            if (texture && texture->getDimensions().xy() == tgt::ivec2(spinWidth_->value(), spinHeight_->value())) {
                texture->downloadTexture();
                ffmpegEncoder_.nextFrame(texture->getPixelData());
            }
            else {
                LERRORC("voreenqt.AnimationExportWidget",
                        "Frame texture could not be downloaded or dimensions do not match");
            }
        } else {
        #endif
            char fn[1024];
            std::string eye_str = "";
            int iterations = 1;

            const tgt::Camera& camSI = camForStereoImages_->get();
            float eyeShift = 0.f;
            float frustumShift = 0.f;
            float frustumLeft = 0.f;
            float frustumRight = 0.f;
            tgt::vec3 eyePosLeft = tgt::vec3(0.f);
            tgt::vec3 eyeFocLeft = tgt::vec3(0.f);
            tgt::vec3 eyePosRight = tgt::vec3(0.f);
            tgt::vec3 eyeFocRight = tgt::vec3(0.f);

            // prepare stereo images (left and right) if option is enabled
            if(stereoImages_->isChecked()){
                iterations++;
                eyeShift = 0.5f*static_cast<float>(eyeSeparation_->value()/100);
                frustumShift = eyeShift*(camSI.getNearDist()/camSI.getFocalLength());
                frustumLeft = camSI.getFrustLeft();
                frustumRight = camSI.getFrustRight();
                tgt::vec3 camShift = camSI.getStrafe()*eyeShift;
                eyePosLeft = camSI.getPosition() - camShift;
                eyeFocLeft = camSI.getFocus() - camShift;
                eyePosRight = camSI.getPosition() + camShift;
                eyeFocRight = camSI.getFocus() + camShift;
            }

            // important: save current camera state before using the processor's camera or
            // successive processors will use those settings!
            //
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();

            // render frame to file
            for(int i=0; i<iterations; i++){
                std::string end_str = eye_str + std::string(".png");

                if(stereoImages_->isChecked()){
                    if(i==1){ // specify left eye
                        eye_str = "_L";
                        camForStereoImages_->setFrustum(tgt::Frustum(frustumLeft + frustumShift, frustumRight + frustumShift,  camSI.getFrustBottom(), camSI.getFrustTop(), camSI.getNearDist(), camSI.getFarDist()));
                        camForStereoImages_->setPosition(eyePosLeft);
                        camForStereoImages_->setFocus(eyeFocLeft);
                    }
                    else{ // specify right eye
                        eye_str = "_R";
                        camForStereoImages_->setFrustum(tgt::Frustum(frustumLeft - frustumShift, frustumRight - frustumShift,  camSI.getFrustBottom(), camSI.getFrustTop(), camSI.getNearDist(), camSI.getFarDist()));
                        camForStereoImages_->setPosition(eyePosRight);
                        camForStereoImages_->setFocus(eyeFocRight);
                    }
                    end_str = eye_str + std::string(".jpg");
                }

                // render cube maps
                if(cubeMapImages_->isChecked() && camForCubeMaps_){
                    const tgt::Camera& camCM = camForCubeMaps_->get();
                    tgt::vec3 focusPointCM = camCM.getFocus();
                    tgt::vec3 cameraPosCM = camCM.getPosition();
                    tgt::vec3 lookAtCM = focusPointCM - cameraPosCM;
                    tgt::vec3 cameraUpCM = camCM.getUpVector();

                    tgt::vec3 cameraRightCM = tgt::normalize(tgt::cross(tgt::normalize(lookAtCM), cameraUpCM));
                    cameraUpCM = tgt::cross(cameraRightCM, tgt::normalize(lookAtCM));
                    camForCubeMaps_->setUpVector(cameraUpCM);

                    if(cubeMapFront_->isChecked()){
                        sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/front_").c_str(), currentFrame_, end_str.c_str());
                        try {
                            painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                        } catch (const VoreenException& e) {
                            LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                        }
                    }
                    if(cubeMapRight_->isChecked()){
                        sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/right_").c_str(), currentFrame_, end_str.c_str());
                        try {
                            //right side
                            rotateView(camForCubeMaps_, -tgt::PI/2.f, cameraUpCM, cameraPosCM, lookAtCM);
                            painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                            camForCubeMaps_->setFocus(focusPointCM);
                        } catch (const VoreenException& e) {
                            LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                        }
                    }
                    if(cubeMapLeft_->isChecked()){
                        sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/left_").c_str(), currentFrame_, end_str.c_str());
                        try {
                            //left side
                            rotateView(camForCubeMaps_, tgt::PI/2.f, cameraUpCM, cameraPosCM, lookAtCM);
                            painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                            camForCubeMaps_->setFocus(focusPointCM);
                        } catch (const VoreenException& e) {
                            LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                        }
                    }
                    if(cubeMapBack_->isChecked()){
                        sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/back_").c_str(), currentFrame_, end_str.c_str());
                        try {
                            //back side
                            rotateView(camForCubeMaps_, tgt::PI, cameraUpCM, cameraPosCM, lookAtCM);
                            painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                            camForCubeMaps_->setFocus(focusPointCM);
                        } catch (const VoreenException& e) {
                            LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                        }
                    }
                    if(cubeMapTop_->isChecked()){
                        sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/top_").c_str(), currentFrame_, end_str.c_str());
                        try {
                            //top side
                            rotateView(camForCubeMaps_, tgt::PI/2.f, cameraRightCM, cameraPosCM, lookAtCM);
                            camForCubeMaps_->setUpVector(tgt::cross(cameraRightCM, tgt::normalize(camForCubeMaps_->get().getFocus())));
                            painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                            camForCubeMaps_->setFocus(focusPointCM);
                            camForCubeMaps_->setUpVector(cameraUpCM);
                        } catch (const VoreenException& e) {
                            LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                        }
                    }
                    if(cubeMapBottom_->isChecked()){
                        sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/bottom_").c_str(), currentFrame_, end_str.c_str());
                        try {
                            //top side
                            rotateView(camForCubeMaps_, -tgt::PI/2.f, cameraRightCM, cameraPosCM, lookAtCM);
                            camForCubeMaps_->setUpVector(-tgt::cross(cameraRightCM, tgt::normalize(camForCubeMaps_->get().getFocus())));
                            painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                            camForCubeMaps_->setFocus(focusPointCM);
                            camForCubeMaps_->setUpVector(cameraUpCM);
                        } catch (const VoreenException& e) {
                            LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                        }
                    }
                }
                else{
                    sprintf(fn, "%s%05d%s", std::string(recordPathName_ + "/frame").c_str(), currentFrame_, end_str.c_str());
                    try {
                        painter_->renderToSnapshot(fn, tgt::ivec2(spinWidth_->value(), spinHeight_->value()));
                    } catch (const VoreenException& e) {
                        LERRORC("voreen.AnimationExportWidget", "Failed to write image: " << std::string(fn) << " with error: " << e.what());
                    }
                }
            }

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        #ifdef VRN_MODULE_FFMPEG
        }
        #endif
        ++currentFrame_;
    }
}

void AnimationExportWidget::endRendering(){
    renderState_= Inactive;
#ifdef VRN_MODULE_FFMPEG
    if(renderingVideo_){
        ffmpegEncoder_.stopVideoEncoding();
        renderingVideo_ = false;
    }
#endif
    canvas_->resize(canvasSize_.x, canvasSize_.y);
}

void AnimationExportWidget::videoSetup() {
#ifdef VRN_MODULE_FFMPEG
    int curPreset = ffmpegEncoder_.getPreset();
    int curBitrate = ffmpegEncoder_.getBitrate();
    QDialog* dialog = createVideoSetupDialog(this, curPreset, curBitrate);
    if (dialog->exec() == QDialog::Accepted)
        ffmpegEncoder_.setup(preset_->currentIndex(), bitrate_->value() * 1024);

    delete dialog;
#endif
}

#ifdef VRN_MODULE_FFMPEG

QDialog* AnimationExportWidget::createVideoSetupDialog(QWidget* parent, int curPreset, int curBitrate) {
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(tr("Setup Video"));
    QVBoxLayout* layout = new QVBoxLayout();
    dialog->setLayout(layout);

    // Preset
    QHBoxLayout* rowLayout = new QHBoxLayout();
    preset_ = new QComboBox(dialog);
    rowLayout->addWidget(new QLabel(tr("Preset:")));

    rowLayout->addWidget(preset_);
    const char** ccPairNames = VideoEncoder::getContainerCodecPairNames();
    for (int i = VideoEncoder::GUESS; i < VideoEncoder::LAST; ++i)
        preset_->addItem(ccPairNames[i]);
    preset_->setCurrentIndex(curPreset);
    layout->addLayout(rowLayout);

    // Bitrate
    rowLayout = new QHBoxLayout();
    bitrate_ = new QSpinBox(dialog);
    rowLayout->addWidget(new QLabel(tr("Bitrate (kbit/s):")));
    rowLayout->addWidget(bitrate_);
    bitrate_->setMinimum(400);
    bitrate_->setMaximum(4000);
    bitrate_->setSingleStep(1);
    bitrate_->setValue(curBitrate / 1024);
    layout->addLayout(rowLayout);

    rowLayout = new QHBoxLayout();
    QPushButton* ok = new QPushButton(QObject::tr("OK"));
    QPushButton* cancel = new QPushButton(QObject::tr("Cancel"));

    connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    rowLayout->addWidget(ok);
    rowLayout->addWidget(cancel);
    layout->addLayout(rowLayout);

    return dialog;
}

#endif // VRN_MODULE_FFMPEG

} // namespace voreen

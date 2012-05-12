DEFINES += VRN_MODULE_FFMPEG

VRN_MODULE_CLASSES += FFmpegModule
VRN_MODULE_CLASS_HEADERS += ffmpeg/ffmpegmodule.h
VRN_MODULE_CLASS_SOURCES += ffmpeg/ffmpegmodule.cpp

# external dependency: FFmpeg library
win32 {
    # use ffmpeg lib from module directory, if not otherwise specified
    isEmpty(FFMPEG_DIR) : FFMPEG_DIR = "$${VRN_MODULE_DIR}/ffmpeg/ext/ffmpeg"
    
    INCLUDEPATH += "$${FFMPEG_DIR}/include"
}
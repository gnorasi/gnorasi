
contains(DEFINES, VRN_DYNAMIC_LIBS) {
    DEFINES += VRN_MODULE_FFMPEG_BUILD_DLL
}

SOURCES += \
    $${VRN_MODULE_DIR}/ffmpeg/videoencoder/videoencoder.cpp
    
HEADERS += \
    $${VRN_MODULE_DIR}/ffmpeg/videoencoder/videoencoder.h

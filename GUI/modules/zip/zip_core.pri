
contains(DEFINES, VRN_DYNAMIC_LIBS) {
    DEFINES += VRN_MODULE_ZIP_BUILD_DLL
}

SOURCES += \
    $${VRN_MODULE_DIR}/zip/io/ziparchive.cpp \
    $${VRN_MODULE_DIR}/zip/io/zipvolumereader.cpp
    
HEADERS += \
    $${VRN_MODULE_DIR}/zip/io/ziparchive.h \
    $${VRN_MODULE_DIR}/zip/io/zipvolumereader.h
    
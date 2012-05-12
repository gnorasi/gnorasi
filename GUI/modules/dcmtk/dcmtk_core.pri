
contains(DEFINES, VRN_DYNAMIC_LIBS) : DEFINES += VRN_MODULE_DCMTK_BUILD_DLL

# volume reader/writer
SOURCES += \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkfindscu.cpp \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkmovescu.cpp \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkvolumereader.cpp \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkvolumewriter.cpp

HEADERS += \
    $${VRN_MODULE_DIR}/dcmtk/dcmtkmoduledefine.h \
    $${VRN_MODULE_DIR}/dcmtk/io/voreendcmtk.h \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkvolumereader.h \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkvolumewriter.h \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkfindscu.h \
    $${VRN_MODULE_DIR}/dcmtk/io/dcmtkmovescu.h

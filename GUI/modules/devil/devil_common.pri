DEFINES += VRN_MODULE_DEVIL

VRN_MODULE_CLASSES += DevILModule
VRN_MODULE_CLASS_HEADERS += devil/devilmodule.h
VRN_MODULE_CLASS_SOURCES += devil/devilmodule.cpp

DEFINES += TGT_HAS_DEVIL
  
# external dependency: DevIL library
win32 {
    # use devil lib from module directory, if not otherwise specified
    isEmpty(DEVIL_DIR) : DEVIL_DIR = "$${VRN_MODULE_DIR}/devil/ext/il"
    
    INCLUDEPATH += "$${DEVIL_DIR}/include"
}

unix {
    !isEmpty(DEVIL_DIR) : INCLUDEPATH += "$${DEVIL_DIR}"
}

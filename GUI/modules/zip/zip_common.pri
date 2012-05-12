DEFINES += VRN_MODULE_ZIP

VRN_MODULE_CLASSES += ZIPModule
VRN_MODULE_CLASS_HEADERS += zip/zipmodule.h
VRN_MODULE_CLASS_SOURCES += zip/zipmodule.cpp
  
# external dependency: ZLIB
win32 {
    # use zlib from module directory, if not otherwise specified
    isEmpty(ZLIB_DIR) : ZLIB_DIR = "$${VRN_MODULE_DIR}/zip/ext/zlib"
        
    INCLUDEPATH += "$${ZLIB_DIR}/include"
    
    win64-msvc {
        # 64 bit lib has been built with WINAPI calling conventions
        # (see zlib-1.2.5 source distribution: contrib/vstudio/readme.txt)
        DEFINES += ZLIB_WINAPI
    }
}

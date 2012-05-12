DEFINES += VRN_MODULE_FONTRENDERING

VRN_MODULE_CLASSES += FontRenderingModule
VRN_MODULE_CLASS_HEADERS += fontrendering/fontrenderingmodule.h
VRN_MODULE_CLASS_SOURCES += fontrendering/fontrenderingmodule.cpp

DEFINES += TGT_HAS_FREETYPE
DEFINES += TGT_HAS_FTGL
  
# freetype and ftgl library paths may be overridden in config.txt
win32 {
    # use libs from module directory, if not otherwise specified
    isEmpty(FREETYPE_DIR) : FREETYPE_DIR = "$${VRN_MODULE_DIR}/fontrendering/ext/freetype"
    isEmpty(FTGL_DIR)     : FTGL_DIR = "$${VRN_MODULE_DIR}/fontrendering/ext/ftgl"
    
    INCLUDEPATH += "$${FREETYPE_DIR}/include"
    INCLUDEPATH += "$${FREETYPE_DIR}/include/freetype2"
    INCLUDEPATH += "$${FTGL_DIR}/include"
}

unix {
    !isEmpty(FREETYPE_DIR) : INCLUDEPATH += "$${FREETYPE_DIR}"
    !isEmpty(FTGL_DIR)     : INCLUDEPATH += "$${FTGL_DIR}"
}

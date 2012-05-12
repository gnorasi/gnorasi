
contains(DEFINES, VRN_DYNAMIC_LIBS) {
    DEFINES += VRN_MODULE_PYTHON_BUILD_DLL
}

SOURCES += \
    $${VRN_MODULE_DIR}/python/core/pythonscript.cpp \
    $${VRN_MODULE_DIR}/python/core/pyvoreen.cpp
    
HEADERS += \
    $${VRN_MODULE_DIR}/python/core/pythonscript.h \
    $${VRN_MODULE_DIR}/python/core/pyvoreen.h

### Local Variables:
### mode:conf-unix
### End:

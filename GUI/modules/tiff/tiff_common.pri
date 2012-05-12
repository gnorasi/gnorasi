DEFINES += VRN_MODULE_TIFF

VRN_MODULE_CLASSES += TiffModule
VRN_MODULE_CLASS_HEADERS += tiff/tiffmodule.h
VRN_MODULE_CLASS_SOURCES += tiff/tiffmodule.cpp

# external dependency: libtiff library
INCLUDEPATH += $${VRN_MODULE_DIR}/tiff/ext/libtiff/include

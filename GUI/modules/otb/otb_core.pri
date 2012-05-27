# processor sources
SOURCES += \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagereaderprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagewriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/otbimagefilterprocessor.cpp

# processor headers
HEADERS += \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagereaderprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagewriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/otbimagefilterprocessor.h

# port sources
SOURCES += \
$${VRN_MODULE_DIR}/otb/ports/otbimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otbsmartpointerport.cpp

# port headers
HEADERS += \
$${VRN_MODULE_DIR}/otb/ports/otbimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otbsmartpointerport.h
# processor sources
SOURCES += \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagereaderprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbvectorimagereaderprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagewriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbconvolutionimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmedianimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbsobelimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Visualization/otbsimpleviewerprocessor.cpp


# processor headers
HEADERS += \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagereaderprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbvectorimagereaderprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagewriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbconvolutionimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.h \ 
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmedianimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbsobelimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Visualization/otbsimpleviewerprocessor.h

# port sources
SOURCES += \
$${VRN_MODULE_DIR}/otb/ports/otbimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otblabelimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otbvectorimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otbsmartpointerport.cpp

# port headers
HEADERS += \
$${VRN_MODULE_DIR}/otb/ports/otbimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otblabelimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otbvectorimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otbsmartpointerport.h
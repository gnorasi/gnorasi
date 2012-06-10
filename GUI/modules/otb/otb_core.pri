# processor sources
SOURCES += \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagereaderprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otblabelimagereaderprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbvectorimagereaderprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagewriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbvectorimagewriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otblabelimagewriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagetovectorimagecastprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbcsvwriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbconvolutionimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmedianimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbsobelimagefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinarythresholdfilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinarydilatefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinaryerodefilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinaryopeningfilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinaryclosingfilterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otblabelimagetolabelmapprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otbshapeattributeslabelmapprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otbstatisticsattributeslabelmapprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otblabelmapstatisticwriterprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbndviprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbmsaviprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbndwiprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbvectorimagebandmathprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbtwoimagebandmathprocessor.cpp \
$${VRN_MODULE_DIR}/otb/processors/Visualization/otbsimpleviewerprocessor.cpp


# processor headers
HEADERS += \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagereaderprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otblabelimagereaderprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbvectorimagereaderprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagewriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbvectorimagewriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otblabelimagewriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbimagetovectorimagecastprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/ImageIO/otbcsvwriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbconvolutionimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.h \ 
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmedianimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbsobelimagefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinarythresholdfilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinarydilatefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinaryerodefilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinaryopeningfilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/BasicFilters/otbbinaryclosingfilterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otblabelimagetolabelmapprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otbshapeattributeslabelmapprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otbstatisticsattributeslabelmapprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/OBIA/otblabelmapstatisticwriterprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbndviprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbmsaviprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbndwiprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbvectorimagebandmathprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Radiometry/otbtwoimagebandmathprocessor.h \
$${VRN_MODULE_DIR}/otb/processors/Visualization/otbsimpleviewerprocessor.h

# port sources
SOURCES += \
$${VRN_MODULE_DIR}/otb/ports/otbimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otblabelimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otblabelmapport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otbvectorimageport.cpp \
$${VRN_MODULE_DIR}/otb/ports/otbsmartpointerport.cpp

# port headers
HEADERS += \
$${VRN_MODULE_DIR}/otb/ports/otbimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otblabelimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otblabelmapport.h \
$${VRN_MODULE_DIR}/otb/ports/otbvectorimageport.h \
$${VRN_MODULE_DIR}/otb/ports/otbsmartpointerport.h
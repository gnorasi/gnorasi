PROJECT(TutorialsExamples)
cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
MESSAGE(FATAL_ERROR
"Cannot build OTB project without OTB. Please set OTB_DIR.")
ENDIF(OTB_FOUND)

ADD_EXECUTABLE(SimpleViewer SimpleViewer.cxx )
TARGET_LINK_LIBRARIES(SimpleViewer OTBCommon OTBIO OTBGui OTBVisualization)

ADD_EXECUTABLE(NDVI NDVI.cxx )
TARGET_LINK_LIBRARIES(NDVI OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(NDWI NDWI.cxx )
TARGET_LINK_LIBRARIES(NDWI OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(ImageDifference ImageDifference.cxx )
TARGET_LINK_LIBRARIES(ImageDifference OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(BinaryThreshold BinaryThreshold.cxx )
TARGET_LINK_LIBRARIES(BinaryThreshold OTBCommon OTBIO)

ADD_EXECUTABLE(BinaryErode BinaryErode.cxx )
TARGET_LINK_LIBRARIES(BinaryErode OTBCommon OTBIO)

ADD_EXECUTABLE(BinaryDilate BinaryDilate.cxx )
TARGET_LINK_LIBRARIES(BinaryDilate OTBCommon OTBIO)

ADD_EXECUTABLE(BinaryOpening BinaryOpening.cxx )
TARGET_LINK_LIBRARIES(BinaryOpening OTBCommon OTBIO)

ADD_EXECUTABLE(BinaryClosing BinaryClosing.cxx )
TARGET_LINK_LIBRARIES(BinaryClosing OTBCommon OTBIO)

ADD_EXECUTABLE(OneImageBandMath OneImageBandMath.cxx )
TARGET_LINK_LIBRARIES(OneImageBandMath OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(TwoImagesBandMath TwoImagesBandMath.cxx )
TARGET_LINK_LIBRARIES(TwoImagesBandMath OTBCommon OTBIO OTBBasicFilters)

################################################################################
# External dependency: OTB library
################################################################################

IF(UNIX)
    # TODO: Use FindOTB
    
    # include paths
    SET(MOD_INCLUDE_DIRECTORIES
        /usr/include/gdal
        /usr/include/otb
        /usr/include/otb/Common
        /usr/include/otb/BasicFilters
        /usr/include/otb/Visualization
        /usr/include/otb/IO
        /usr/include/otb/Projections
        /usr/include/otb/Gui
        /usr/include/otb/OBIA
        /usr/include/otb/Radiometry
        /usr/include/otb/FeatureExtraction
        /usr/include/otb/Learning
        /usr/include/otb/Utilities/otbedison
        /usr/include/otb/UtilitiesAdapters/OssimAdapters
        /usr/include/otb/UtilitiesAdapters/CurlAdapters
        /usr/include/otb/Utilities/otbconfigfile
        /usr/include/otb/Utilities/ITK/Common
        /usr/include/otb/Utilities/ITK
        /usr/include/otb/Utilities/ITK/IO
        /usr/include/otb/Utilities/ITK/BasicFilters
        /usr/include/otb/Utilities/ITK/Algorithms
        /usr/include/otb/Utilities/ITK/Review
        /usr/include/otb/Utilities/ITK/Utilities
        /usr/include/otb/Utilities/ITK/Utilities/vxl/vcl
        /usr/include/otb/Utilities/ITK/Utilities/vxl/core
        /usr/include/otb/Utilities/ITK/Numerics/Statistics
    )

    # link against OTB + ITK V3.0
    SET(MOD_LIBRARIES
        -L/usr/lib64/otb
        -lOTBCommon
        -lOTBIO
        -lOTBBasicFilters
        -lOTBGui
        -lOTBVisualization
        -lotbedison
        -lOTBLearning
        -lOTBOBIA
        -lITKCommon
        -lITKIO
        -lITKStatistics
        -lITKBasicFilters
        -lITKAlgorithms
        -lITKNumerics

    )
    
ENDIF()


################################################################################
# Core module resources 
################################################################################
SET(MOD_CORE_MODULECLASS OTBModule)

SET(MOD_CORE_SOURCES 
    ${MOD_DIR}/ports/otbimageport.cpp
    ${MOD_DIR}/ports/otblabelimageport.cpp
    ${MOD_DIR}/ports/otblabelmapport.cpp
    ${MOD_DIR}/ports/otbvectorimageport.cpp
    ${MOD_DIR}/ports/otbsmartpointerport.cpp
    ${MOD_DIR}/processors/ImageIO/otbimagereaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otblabelimagereaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbvectorimagereaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbimagewriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otblabelimagewriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbcsvwriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbcsvreaderprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbconvolutionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbmeanimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbmedianimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbsobelimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinarythresholdfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinarydilatefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaledilatefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinaryerodefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleerodefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinaryopeningfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleopeningfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinaryclosingfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleclosingfilterprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelimagetolabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbshapeattributeslabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbstatisticsattributeslabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelmapstatisticwriterprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbobjectclassificationtolabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelmaptoclassimageprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbndviprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbmsaviprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbndwiprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbvectorimagebandmathprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbtwoimagebandmathprocessor.cpp
    ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.cpp
)

SET(MOD_CORE_HEADERS 
    ${MOD_DIR}/ports/otbimageport.h
    ${MOD_DIR}/ports/otblabelimageport.h
    ${MOD_DIR}/ports/otblabelmapport.h
    ${MOD_DIR}/ports/otbvectorimageport.h
    ${MOD_DIR}/ports/otbsmartpointerport.h
    ${MOD_DIR}/processors/ImageIO/otbimagereaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otblabelimagereaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbvectorimagereaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbimagewriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otblabelimagewriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbcsvwriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbcsvreaderprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbconvolutionimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbmeanimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbmedianimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbsobelimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinarythresholdfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinarydilatefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaledilatefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinaryerodefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleerodefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinaryopeningfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleopeningfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinaryclosingfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleclosingfilterprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelimagetolabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otbshapeattributeslabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otbstatisticsattributeslabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelmapstatisticwriterprocessor.h
    ${MOD_DIR}/processors/OBIA/otbobjectclassificationtolabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelmaptoclassimageprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbndviprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbmsaviprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbndwiprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbvectorimagebandmathprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbtwoimagebandmathprocessor.h
    ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.h
)

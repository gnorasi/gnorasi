
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
SET(MOD_CORE_MODULECLASS ITKModule)

SET(MOD_CORE_SOURCES 
    ${MOD_DIR}/utils/itkwrapper.cpp
    #${MOD_DIR}/io/itkvolumereader.cpp
    ${MOD_DIR}/processors/anisotropicdiffusion.cpp
    ${MOD_DIR}/processors/doublethreshold.cpp
    ${MOD_DIR}/processors/gradientvectorflow.cpp
    ${MOD_DIR}/processors/itkprocessor.cpp
    ${MOD_DIR}/processors/volumefilter_itk.cpp
    ${MOD_DIR}/processors/valuedregionalmaximaimagefilter.cpp
    ${MOD_DIR}/processors/vesselness.cpp
    ${MOD_DIR}/processors/mutualinformationregistration.cpp
    ${MOD_DIR}/processors/fastmarchingimagefilter.cpp
    ${MOD_DIR}/processors/curveslevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/geodesicactivecontourlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/geodesicactivecontourshapepriorlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/narrowbandcurveslevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/narrowbandthresholdsegmentationlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/shapedetectionlevelsetimagefilterworkflow.cpp
    ${MOD_DIR}/processors/watershedimagefilter.cpp
    ${MOD_DIR}/processors/thresholdlabelerimagefilter.cpp
    ${MOD_DIR}/processors/labelmapoverlayimagefilter.cpp
    ${MOD_DIR}/processors/labelmapcontouroverlayimagefilter.cpp
    ${MOD_DIR}/processors/bayesianclassifierimagefilter.cpp
)

SET(MOD_CORE_HEADERS 
    ${MOD_DIR}/utils/itkwrapper.h
    #${MOD_DIR}/io/itkvolumereader.h
    ${MOD_DIR}/processors/anisotropicdiffusion.h
    ${MOD_DIR}/processors/doublethreshold.h
    ${MOD_DIR}/processors/gradientvectorflow.h
    ${MOD_DIR}/processors/itkprocessor.h
    ${MOD_DIR}/processors/volumefilter_itk.h
    ${MOD_DIR}/processors/valuedregionalmaximaimagefilter.h
    ${MOD_DIR}/processors/vesselness.h
    ${MOD_DIR}/processors/mutualinformationregistration.h
    ${MOD_DIR}/processors/fastmarchingimagefilter.h
    ${MOD_DIR}/processors/curveslevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/geodesicactivecontourlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/geodesicactivecontourshapepriorlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/narrowbandcurveslevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/narrowbandthresholdsegmentationlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/shapedetectionlevelsetimagefilterworkflow.h
    ${MOD_DIR}/processors/watershedimagefilter.h
    ${MOD_DIR}/processors/thresholdlabelerimagefilter.h
    ${MOD_DIR}/processors/labelmapoverlayimagefilter.h
    ${MOD_DIR}/processors/labelmapcontouroverlayimagefilter.h
    ${MOD_DIR}/processors/bayesianclassifierimagefilter.h
)

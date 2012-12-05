
################################################################################
# External dependency: OTB library
################################################################################

IF(UNIX)
    find_package(OTB)

    #include(${OTB_USE_FILE})


    # include paths
    SET(MOD_INCLUDE_DIRECTORIES ${OTB_INCLUDE_DIRS})

    FIND_LIBRARY(OTBCommon_LIBRARY
                 NAMES libOTBCommon.so
                 HINTS ${OTB_LIBRARY_DIRS}
                 PATHS /usr/lib/otb
                       /usr/lib64/otb
                 )

        IF(NOT OTBCommon_LIBRARY)
                # In case library not found set path
                SET(OTBCommon_LIBRARY_PATH /usr/lib/otb)
        ELSE()
                GET_FILENAME_COMPONENT(OTBCommon_LIBRARY_PATH
                                                           ${OTBCommon_LIBRARY}
                                                           PATH)
        ENDIF()


    # link against OTB + ITK V3.0
    SET(MOD_LIBRARIES
        -L${OTBCommon_LIBRARY_PATH}
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

ELSEIF(WIN32)

    # include paths
    SET(MOD_INCLUDE_DIRECTORIES
        ${MOD_DIR}/ext/orfeotoolbox/include
    )

	# libraries
    IF(VRN_WIN32)
        SET(ORFEOTOOLBOX_LIB_DIR ${MOD_DIR}/ext/orfeotoolbox/lib/win32)
    ELSEIF(VRN_WIN64)
        SET(ORFEOTOOLBOX_LIB_DIR ${MOD_DIR}/ext/orfeotoolbox/lib/win64)
    ELSE()
        MESSAGE(FATAL_ERROR "Neither VRN_WIN32 nor VRN_WIN64 defined!")
    ENDIF()

    # link against OrfeoToolBox
    SET(MOD_DEBUG_LIBRARIES
                ${ORFEOTOOLBOX_LIB_DIR}/debug/fltk.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/fltk_forms.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/fltk_gl.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/fltk_images.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/fltk_png.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/fltk_zlib.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/InsightJournal.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKAlgorithms.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKBasicFilters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKCommon.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKFEM.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKIO.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKIOReview.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkNetlibSlatec.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKNumerics.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKQuadEdgeMesh.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKSpatialObject.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/ITKStatistics.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itksys.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkv3p_lsqr.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkv3p_netlib.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkvcl.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkvnl.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkvnl_algo.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/itkvnl_inst.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OpenThreads.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otb6S.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBBasicFilters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBChangeDetection.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBCommon.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbconfigfile.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBCurlAdapters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBDisparityMap.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbedison.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBFeatureExtraction.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBFusion.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBFuzzy.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBGui.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBHyperspectral.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBIO.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbkml.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBLearning.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbliblas.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBMarkov.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBMultiScale.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBMultiTemporal.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbmuparser.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBOBIA.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBObjectDetection.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBOGRAdapters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbopenjpeg.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbossim.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBOssimAdapters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbossimplugins.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBProjections.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBRadiometry.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBSARPolarimetry.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBSegmentation.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBSimulation.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBSpatialReasoning.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbsvm.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbTestDriver.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBTesting.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbtinyXML.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/otbViewer.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBVisu.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/OTBVisualization.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/siftfast.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/gdal_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/geotiff_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/jpeg_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/jpeg12_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/libtiff_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/debug/libcurl_imp.lib
    )

	# link against OrfeoToolBox
    SET(MOD_RELEASE_LIBRARIES
                ${ORFEOTOOLBOX_LIB_DIR}/release/fltk.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/fltk_forms.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/fltk_gl.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/fltk_images.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/fltk_png.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/fltk_zlib.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/InsightJournal.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKAlgorithms.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKBasicFilters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKCommon.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKFEM.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKIO.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKIOReview.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkNetlibSlatec.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKNumerics.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKQuadEdgeMesh.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKSpatialObject.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/ITKStatistics.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itksys.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkv3p_lsqr.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkv3p_netlib.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkvcl.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkvnl.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkvnl_algo.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/itkvnl_inst.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OpenThreads.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otb6S.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBBasicFilters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBChangeDetection.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBCommon.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbconfigfile.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBCurlAdapters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBDisparityMap.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbedison.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBFeatureExtraction.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBFusion.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBFuzzy.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBGui.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBHyperspectral.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBIO.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbkml.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBLearning.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbliblas.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBMarkov.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBMultiScale.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBMultiTemporal.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbmuparser.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBOBIA.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBObjectDetection.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBOGRAdapters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbopenjpeg.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbossim.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBOssimAdapters.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbossimplugins.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBProjections.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBRadiometry.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBSARPolarimetry.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBSegmentation.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBSimulation.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBSpatialReasoning.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbsvm.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbTestDriver.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBTesting.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbtinyXML.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/otbViewer.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBVisu.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/OTBVisualization.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/siftfast.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/gdal_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/geotiff_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/jpeg_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/jpeg12_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/libtiff_i.lib
                ${ORFEOTOOLBOX_LIB_DIR}/release/libcurl_imp.lib
    )

ENDIF(UNIX)


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
    ${MOD_DIR}/processors/ImageIO/otblabelimagewriterprocessor.cpp
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
    ${MOD_DIR}/processors/Visualization/otbimageviewerprocessor.cpp
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
    ${MOD_DIR}/processors/ImageIO/otblabelimagewriterprocessor.h
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
    ${MOD_DIR}/processors/Visualization/otbimageviewerprocessor.h
)

#
#   Edit
#
#   Some classed were causing build errors on windows palform.
#   Platform check and comment those classes in ordet the code to be maintained
#
#   TODO.
#   Fix those errors and uncomment those classes on windows case or just add them to
#   the precvious set functions
#
IF(UNIX)
    LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.cpp)
    LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.cpp)
    LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.cpp)

    LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.h)
    LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.h)
    LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.h)
ELSE(UNIX)
    # LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.cpp)
    # LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.cpp)
    # LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.cpp)

    # LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.h)
    # LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.h)
    # LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.h)
ENDIF(UNIX)

################################################################################
# Qt module resources
################################################################################
SET(MOD_QT_MODULECLASS OtbModuleQt)


################################################################################
# Qt sources
################################################################################
SET(MOD_QT_SOURCES
    ${MOD_DIR}/qt/qotbprocessorwidgetfactory.cpp
    ${MOD_DIR}/qt/widgets/qotbimageviewerwidget.cpp
    ${MOD_DIR}/qt/viewer/itiotbimagemanager.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbaimagewidget.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbaimageviewer.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewerfactory.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewer.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbaimageviewerfactory.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanel.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpaneldatatab.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanelsetuptab.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanelhistogramtab.cpp
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanelpixeldescriptiontab.cpp
    ${MOD_DIR}/qt/viewer/commands/command.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcolorcomposition.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancement.cpp
    ${MOD_DIR}/qt/viewer/itiotbfocusregion.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbafocusregion.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbascrollablewidget.cpp
    ${MOD_DIR}/qt/viewer/itiotbfocusregionobserver.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbazoomablewidget.cpp
    ${MOD_DIR}/qt/viewer/itiotbrgbafullwidget.cpp
)
################################################################################
# Qt headers
################################################################################
SET(MOD_QT_HEADERS
    ${MOD_DIR}/qt/widgets/qotbimageviewerwidget.h
    ${MOD_DIR}/qt/viewer/itiotbimagemanager.h/
    ${MOD_DIR}/qt/viewer/itiotbrgbaimagewidget.h
    ${MOD_DIR}/qt/viewer/itiotbrgbaimageviewer.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewerfactory.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewer.h
    ${MOD_DIR}/qt/viewer/itiotbrgbaimageviewerfactory.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanel.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpaneldatatab.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanelsetuptab.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanelhistogramtab.h
    ${MOD_DIR}/qt/viewer/itiotbimageviewerpanelpixeldescriptiontab.h
    ${MOD_DIR}/qt/viewer/commands/command.h
    ${MOD_DIR}/qt/viewer/commands/commandcolorcomposition.h
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancement.h
    ${MOD_DIR}/qt/viewer/itiotbfocusregion.h
    ${MOD_DIR}/qt/viewer/itiotbrgbafocusregion.h
    ${MOD_DIR}/qt/viewer/itiotbrgbascrollablewidget.h
    ${MOD_DIR}/qt/viewer/itiotbfocusregionobserver.h
    ${MOD_DIR}/qt/viewer/itiotbrgbazoomablewidget.h
    ${MOD_DIR}/qt/viewer/itiotbrgbafullwidget.h
)

SET(MOD_QT_HEADERS_NONMOC
    ${MOD_DIR}/qt/qotbprocessorwidgetfactory.h
)

SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTGUI TRUE)

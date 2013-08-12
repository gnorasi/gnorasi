
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
                ${ORFEOTOOLBOX_LIB_DIR}/debug/qwtd.lib
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
                ${ORFEOTOOLBOX_LIB_DIR}/release/qwt.lib
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
    ${MOD_DIR}/ports/otbkeypointsetport.cpp
    ${MOD_DIR}/ports/otbvectordataport.cpp
    ${MOD_DIR}/ports/otbsommapport.cpp
    ${MOD_DIR}/processors/ImageIO/otbimagereaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otblabelimagereaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbvectorimagereaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbimagewriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otblabelimagewriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbcsvwriterprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbcsvreaderprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbmultichannelextractroiprocessor.cpp
    ${MOD_DIR}/processors/ImageIO/otbsommapreaderprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbconvolutionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbmeanimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbmedianimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbsobelimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinarythresholdfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinarydilatefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaledilatefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinaryerodefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleerodefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinaryopeningfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleopeningfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbbinaryclosingfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleclosingfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbdiscretegaussianimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbleeimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbscalarimagetotexturesfilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgradientmagnitudeimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbhistogrammatchingimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbspectralangledistanceimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbgreylevelcooccurrencematrixadvancedtexturecoefficientscalculatorprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbestimateinnerproductpcaimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbperbandvectorimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbmdmdnmfimagefilterprocessor.cpp
    ${MOD_DIR}/processors/BasicFilters/otbsigmoidimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscaleconnectedopeningimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscaleconnectedclosingimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscalegeodesicdilateimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscalegeodesicerodeimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbcurvatureanisotropicdiffusionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbmorphologicalwatershedimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbattributesmapopeninglabelmapfilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otblmvmpansharpeningfusionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbrgbtoluminanceimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbbinarythinningimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ImageEnhancement/otbgeodesicmorphologylevelingfilterprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelimagetolabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbshapeattributeslabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbstatisticsattributeslabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelmapstatisticwriterprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otbobjectclassificationtolabelmapprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelmaptoclassimageprocessor.cpp
    ${MOD_DIR}/processors/OBIA/otblabelmaptobinaryimagefilterprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbndviprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbmsaviprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbndwiprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbvectorimagebandmathprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbtwoimagebandmathprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbbayesianfusionfilterprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otblaindviprocessor.cpp
    ${MOD_DIR}/processors/Radiometry/otbdemcaracteristicsextractor.cpp
    ${MOD_DIR}/processors/Visualization/otbimageviewerprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbimagetosurfkeypointsetfilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbimagetosiftkeypointsetfilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otblineratiodetectorimagefilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otblinesegmentdetectorprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbroadextractionprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbzerocrossingbasededgedetectionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbhoughtransform2dcirclesimagefilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbhoughtransform2dlinesimagefilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbclouddetectionimagefilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbgenericroadextractionfilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbcloudestimatorfilterprocessor.cpp
    ${MOD_DIR}/processors/FeatureExtraction/otbderivativeimagefilterprocessor.cpp
    ${MOD_DIR}/processors/Segmentation/otbmeanshiftimagesegmentationprocessor.cpp
    ${MOD_DIR}/processors/Segmentation/otbwatershedsegmentationfilterprocessor.cpp
    ${MOD_DIR}/processors/Segmentation/otblaplaciansegmentationlevelsetimagefilterprocessor.cpp
    ${MOD_DIR}/processors/Segmentation/otbgeodesicactivecontourlevelsetimagefilterprocessor.cpp
    ${MOD_DIR}/processors/Segmentation/otbshapedetectionlevelsetimagefilterprocessor.cpp
    ${MOD_DIR}/processors/Classification/otbconfusionmatrixcalculatorprocessor.cpp
    ${MOD_DIR}/processors/Classification/otbkmeansimageclassificationfilterprocessor.cpp
    ${MOD_DIR}/processors/Classification/otbbayesianclassifierimagefilterprocessor.cpp
    ${MOD_DIR}/processors/Classification/otbmrfenergygaussianclassificationfilterprocessor.cpp
    ${MOD_DIR}/processors/Classification/otbsomimageclassificationfilterprocessor.cpp
    ${MOD_DIR}/processors/ChangeDetection/otbmultialterationdetectorimagefilterprocessor.cpp
    ${MOD_DIR}/processors/ChangeDetection/otbcbamichangedetectorprocessor.cpp
#    ${MOD_DIR}/processors/Geometry/otborthorectifyprocessor.cpp
)

SET(MOD_CORE_HEADERS
    ${MOD_DIR}/ports/otbimageport.h
    ${MOD_DIR}/ports/otblabelimageport.h
    ${MOD_DIR}/ports/otblabelmapport.h
    ${MOD_DIR}/ports/otbvectorimageport.h
    ${MOD_DIR}/ports/otbsmartpointerport.h
    ${MOD_DIR}/ports/otbkeypointsetport.h
    ${MOD_DIR}/ports/otbvectordataport.h
    ${MOD_DIR}/ports/otbsommapport.h
    ${MOD_DIR}/processors/ImageIO/otbimagereaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otblabelimagereaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbvectorimagereaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbimagewriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otblabelimagewriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbcsvwriterprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbcsvreaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbsommapreaderprocessor.h
    ${MOD_DIR}/processors/ImageIO/otbmultichannelextractroiprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbconvolutionimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbfftconvolutionimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbmeanimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbmedianimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbsobelimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinarythresholdfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinarydilatefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaledilatefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinaryerodefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleerodefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinaryopeningfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleopeningfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbbinaryclosingfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgrayscaleclosingfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbdiscretegaussianimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbleeimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbscalarimagetotexturesfilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgradientmagnitudeimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbhistogrammatchingimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbspectralangledistanceimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbgreylevelcooccurrencematrixadvancedtexturecoefficientscalculatorprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbestimateinnerproductpcaimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbperbandvectorimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbmdmdnmfimagefilterprocessor.h
    ${MOD_DIR}/processors/BasicFilters/otbsigmoidimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscaleconnectedopeningimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscaleconnectedclosingimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscalegeodesicdilateimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbgrayscalegeodesicerodeimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbcurvatureanisotropicdiffusionimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbmorphologicalwatershedimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbattributesmapopeninglabelmapfilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otblmvmpansharpeningfusionimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbrgbtoluminanceimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbbinarythinningimagefilterprocessor.h
    ${MOD_DIR}/processors/ImageEnhancement/otbgeodesicmorphologylevelingfilterprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelimagetolabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otbshapeattributeslabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otbstatisticsattributeslabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelmapstatisticwriterprocessor.h
    ${MOD_DIR}/processors/OBIA/otbobjectclassificationtolabelmapprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelmaptoclassimageprocessor.h
    ${MOD_DIR}/processors/OBIA/otblabelmaptobinaryimagefilterprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbndviprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbmsaviprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbndwiprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbvectorimagebandmathprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbtwoimagebandmathprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbbayesianfusionfilterprocessor.h 
    ${MOD_DIR}/processors/Radiometry/otblaindviprocessor.h
    ${MOD_DIR}/processors/Radiometry/otbdemcaracteristicsextractor.h
    ${MOD_DIR}/processors/Visualization/otbimageviewerprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbimagetosurfkeypointsetfilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbimagetosiftkeypointsetfilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otblineratiodetectorimagefilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otblinesegmentdetectorprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbroadextractionprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbzerocrossingbasededgedetectionimagefilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbhoughtransform2dcirclesimagefilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbhoughtransform2dlinesimagefilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbclouddetectionimagefilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbgenericroadextractionfilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbcloudestimatorfilterprocessor.h
    ${MOD_DIR}/processors/FeatureExtraction/otbderivativeimagefilterprocessor.h
    ${MOD_DIR}/processors/Segmentation/otbmeanshiftimagesegmentationprocessor.h
    ${MOD_DIR}/processors/Segmentation/otbwatershedsegmentationfilterprocessor.h
    ${MOD_DIR}/processors/Segmentation/otblaplaciansegmentationlevelsetimagefilterprocessor.h
    ${MOD_DIR}/processors/Segmentation/otbgeodesicactivecontourlevelsetimagefilterprocessor.h
    ${MOD_DIR}/processors/Segmentation/otbshapedetectionlevelsetimagefilterprocessor.h
    ${MOD_DIR}/processors/Classification/otbconfusionmatrixcalculatorprocessor.h
    ${MOD_DIR}/processors/Classification/otbkmeansimageclassificationfilterprocessor.h
    ${MOD_DIR}/processors/Classification/otbbayesianclassifierimagefilterprocessor.h
    ${MOD_DIR}/processors/Classification/otbmrfenergygaussianclassificationfilterprocessor.h
    ${MOD_DIR}/processors/Classification/otbsomimageclassificationfilterprocessor.h
    ${MOD_DIR}/processors/ChangeDetection/otbmultialterationdetectorimagefilterprocessor.h
    ${MOD_DIR}/processors/ChangeDetection/otbcbamichangedetectorprocessor.h
#    ${MOD_DIR}/processors/Geometry/otborthorectifyprocessor.h
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
     LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.cpp)
    LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.cpp)
    # LIST(APPEND MOD_CORE_SOURCES ${MOD_DIR}/processors/Visualization/otbsimpleviewerprocessor.cpp)

     LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/ImageIO/otbvectorimagewriterprocessor.h)
    LIST(APPEND MOD_CORE_HEADERS ${MOD_DIR}/processors/ImageIO/otbimagetovectorimagecastprocessor.h)
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
    ${MOD_DIR}/qt/viewer/utils/itiotbimagemanager.cpp
    ${MOD_DIR}/qt/viewer/widgets/itiotbimageviewer.cpp
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaimageviewer.cpp
    ${MOD_DIR}/qt/viewer/factories/itiotbimageviewerfactory.cpp
#    ${MOD_DIR}/qt/viewer/factories/itiotbrgbaimageviewerfactory.cpp
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanel.cpp
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpaneldatatab.cpp
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanelsetuptab.cpp
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanelhistogramtab.cpp
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanelpixeldescriptiontab.cpp
    ${MOD_DIR}/qt/viewer/commands/command.cpp
    ${MOD_DIR}/qt/viewer/observables/itiviewerobservable.cpp
    ${MOD_DIR}/qt/viewer/observables/itiviewerobservableregion.cpp
    ${MOD_DIR}/qt/viewer/widgets/itiviewerobserver.cpp
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaqglwidgetscrollable.cpp
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaqglwidgetfullview.cpp
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaqglwidgetzoomable.cpp
    ${MOD_DIR}/qt/viewer/widgets/itiviewerpixelinfowidget.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbimagechannelprovider.cpp
#    ${MOD_DIR}/qt/viewer/utils/itiotbimagergbachannelprovider.cpp
    ${MOD_DIR}/qt/viewer/factories/itiotbvectorimageviewerfactory.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbimagevectorchannelprovider.cpp
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorimageviewer.cpp
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorqglwidgetscrollable.cpp
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorqglwidgetfullview.cpp
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorqglwidgetzoomable.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementgaussian.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementlinear0255.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementlinearxperc.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementsquareroot.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbImageModelRendererScrollable.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbImageModelRendererZoomable.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbImageModelRendererFullView.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbImageViewManipulatorZoomable.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbImageViewManipulatorFullView.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbImageViewManipulatorScrollable.cpp
    ${MOD_DIR}/qt/viewer/models/itiotbAbstractImageModel.cpp
    ${MOD_DIR}/qt/viewer/models/itiotbAbstractModel.cpp
    ${MOD_DIR}/qt/viewer/models/itiotbVectorImageModel.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcolorcompositiongreyscale.cpp
    ${MOD_DIR}/qt/viewer/commands/commandcolorcompositionrgb.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotblevelutility.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotblevel.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotbregion.cpp
    ${MOD_DIR}/qt/viewer/utils/itiotblabelmapparser.cpp
#    ${MOD_DIR}/qt/viewer/utils/itiotblabelimageparser.cpp
    ${MOD_DIR}/qt/viewer/commands/commandtoggleclassificationlabelvisibiltiy.cpp
#    ${MOD_DIR}/qt/orthorectify/otborthorectifyutility.cpp
#    ${MOD_DIR}/qt/widgets/qotborthorectifywidget.cpp
    ${MOD_DIR}/qt/histogram/histogramgenerator.cpp
    ${MOD_DIR}/qt/histogram/histogramview.cpp
)
################################################################################
# Qt headers
################################################################################
SET(MOD_QT_HEADERS
    ${MOD_DIR}/qt/viewer/rgba_globaldefs.h
    ${MOD_DIR}/qt/widgets/qotbimageviewerwidget.h
    ${MOD_DIR}/qt/viewer/utils/itiotbimagemanager.h
    ${MOD_DIR}/qt/viewer/widgets/itiotbimageviewer.h
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaimageviewer.h
    ${MOD_DIR}/qt/viewer/factories/itiotbimageviewerfactory.h
#    ${MOD_DIR}/qt/viewer/factories/itiotbrgbaimageviewerfactory.h
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanel.h
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpaneldatatab.h
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanelsetuptab.h
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanelhistogramtab.h
    ${MOD_DIR}/qt/viewer/widgets/panel/itiotbimageviewerpanelpixeldescriptiontab.h
    ${MOD_DIR}/qt/viewer/commands/command.h
    ${MOD_DIR}/qt/viewer/observables/itiviewerobservable.h
    ${MOD_DIR}/qt/viewer/observables/itiviewerobservableregion.h
    ${MOD_DIR}/qt/viewer/widgets/itiviewerobserver.h
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaqglwidgetscrollable.h
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaqglwidgetfullview.h
#    ${MOD_DIR}/qt/viewer/widgets/rgb/itiotbrgbaqglwidgetzoomable.h
    ${MOD_DIR}/qt/viewer/widgets/itiviewerpixelinfowidget.h
    ${MOD_DIR}/qt/viewer/utils/itiotbimagechannelprovider.h
#    ${MOD_DIR}/qt/viewer/utils/itiotbimagergbachannelprovider.h
    ${MOD_DIR}/qt/viewer/factories/itiotbvectorimageviewerfactory.h
    ${MOD_DIR}/qt/viewer/vector_globaldefs.h
    ${MOD_DIR}/qt/viewer/utils/itiotbimagevectorchannelprovider.h
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorimageviewer.h
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorqglwidgetscrollable.h
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorqglwidgetfullview.h
    ${MOD_DIR}/qt/viewer/widgets/vector/itiotbvectorqglwidgetzoomable.h
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementgaussian.h
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementlinear0255.h
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementlinearxperc.h
    ${MOD_DIR}/qt/viewer/commands/commandcontrastenhancementsquareroot.h
    ${MOD_DIR}/qt/viewer/utils/itiotbImageModelRendererScrollable.h
    ${MOD_DIR}/qt/viewer/utils/itiotbImageModelRendererZoomable.h
    ${MOD_DIR}/qt/viewer/utils/itiotbImageModelRendererFullView.h
    ${MOD_DIR}/qt/viewer/utils/itiotbImageViewManipulatorZoomable.h
    ${MOD_DIR}/qt/viewer/utils/itiotbImageViewManipulatorFullView.h
    ${MOD_DIR}/qt/viewer/utils/itiotbImageViewManipulatorScrollable.h
    ${MOD_DIR}/qt/viewer/models/itiotbAbstractImageModel.h
    ${MOD_DIR}/qt/viewer/models/itiotbAbstractModel.h
    ${MOD_DIR}/qt/viewer/models/itiotbVectorImageModel.h
    ${MOD_DIR}/qt/viewer/commands/commandcolorcompositiongreyscale.h
    ${MOD_DIR}/qt/viewer/commands/commandcolorcompositionrgb.h
    ${MOD_DIR}/qt/viewer/utils/itiotblevelutility.h
    ${MOD_DIR}/qt/viewer/utils/itiotblevel.h
    ${MOD_DIR}/qt/viewer/utils/itiotbregion.h
    ${MOD_DIR}/qt/viewer/utils/itiotblabelmapparser.h
    ${MOD_DIR}/qt/viewer/commands/commandtoggleclassificationlabelvisibiltiy.h
    ${MOD_DIR}/qt/histogram/histogramgenerator.h
    ${MOD_DIR}/qt/histogram/histogramview.h
#    ${MOD_DIR}/qt/viewer/utils/itiotblabelimageparser.h
#    ${MOD_DIR}/qt/orthorectify/otborthorectifyutility.h
#    ${MOD_DIR}/qt/widgets/qotborthorectifywidget.h
)

SET(MOD_QT_HEADERS_NONMOC
    ${MOD_DIR}/qt/qotbprocessorwidgetfactory.h
)

SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTGUI TRUE)

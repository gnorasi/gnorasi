
################################################################################
# Core module resources 
################################################################################
SET(MOD_CORE_MODULECLASS KnowledgeModule)

SET(MOD_CORE_SOURCES 
    ${MOD_DIR}/processors/dummysegmentationprocessor.cpp
    ${MOD_DIR}/processors/classifierwsprocessor.cpp
    ${MOD_DIR}/processors/retrieveclassificationdataprocessor.cpp
)

SET(MOD_CORE_HEADERS 
    ${MOD_DIR}/processors/dummysegmentationprocessor.h
    ${MOD_DIR}/processors/classifierwsprocessor.h
    ${MOD_DIR}/processors/retrieveclassificationdataprocessor.h
)

################################################################################
# Qt module resources 
################################################################################
SET(MOD_QT_MODULECLASS KnowledgeModuleQt)

SET(MOD_QT_SOURCES
    ${MOD_DIR}/qt/processorwidgets/knowledgeprocessorwidgetfactory.cpp
    ${MOD_DIR}/qt/processorwidgets/dummysegmentationwidget.cpp
    ${MOD_DIR}/qt/processorwidgets/classifierwswidget.cpp
    ${MOD_DIR}/qt/processorwidgets/retrieveclassificationdatawidget.cpp
)

SET(MOD_QT_HEADERS
    ${MOD_DIR}/qt/processorwidgets/dummysegmentationwidget.h
    ${MOD_DIR}/qt/processorwidgets/classifierwswidget.h
    ${MOD_DIR}/qt/processorwidgets/retrieveclassificationdatawidget.h
)

SET(MOD_QT_HEADERS_NONMOC
    ${MOD_DIR}/qt/processorwidgets/knowledgeprocessorwidgetfactory.h
)

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)

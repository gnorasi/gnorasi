
################################################################################
# Core module resources 
################################################################################
SET(MOD_CORE_MODULECLASS KnowledgeModule)

SET(MOD_CORE_SOURCES 
    ${MOD_DIR}/processors/dummysegmentationprocessor.cpp
    ${MOD_DIR}/processors/classifierwsprocessor.cpp
    ${MOD_DIR}/processors/retrieveclassificationdataprocessor.cpp
    ${MOD_DIR}/processors/ontologycreatorprocessor.cpp
    ${MOD_DIR}/processors/featureclassificationprocessor.cpp
    ${MOD_DIR}/processors/geospatialclassificationprocessor.cpp
)

SET(MOD_CORE_HEADERS 
    ${MOD_DIR}/processors/dummysegmentationprocessor.h
    ${MOD_DIR}/processors/classifierwsprocessor.h
    ${MOD_DIR}/processors/retrieveclassificationdataprocessor.h
    ${MOD_DIR}/processors/ontologycreatorprocessor.h
    ${MOD_DIR}/processors/featureclassificationprocessor.h
    ${MOD_DIR}/processors/geospatialclassificationprocessor.h
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
    ${MOD_DIR}/qt/processorwidgets/ontologycreatorwidget.cpp
    ${MOD_DIR}/qt/models/ontologyclassmodel.cpp
    ${MOD_DIR}/qt/models/ontologyclassitem.cpp
    ${MOD_DIR}/qt/processorwidgets/ontologyclassview.cpp
    ${MOD_DIR}/qt/utils/owlwriter.cpp
    ${MOD_DIR}/qt/utils/owlparser.cpp
    ${MOD_DIR}/qt/utils/owlhelperitem.cpp
    ${MOD_DIR}/qt/processorwidgets/geospatialclassificationwidget.cpp
    ${MOD_DIR}/qt/processorwidgets/rulewidget.cpp
    ${MOD_DIR}/qt/processorwidgets/ruleitemview.cpp
    ${MOD_DIR}/qt/models/ruleitem.cpp
    ${MOD_DIR}/qt/models/rule.cpp
    ${MOD_DIR}/qt/utils/ruleutility.cpp
    ${MOD_DIR}/qt/processorwidgets/comboboxdelegate.cpp
    ${MOD_DIR}/qt/utils/rulewriter.cpp
    ${MOD_DIR}/qt/utils/ruleparser.cpp
)

SET(MOD_QT_HEADERS
    ${MOD_DIR}/qt/processorwidgets/dummysegmentationwidget.h
    ${MOD_DIR}/qt/processorwidgets/classifierwswidget.h
    ${MOD_DIR}/qt/processorwidgets/retrieveclassificationdatawidget.h
    ${MOD_DIR}/qt/processorwidgets/ontologycreatorwidget.h
    ${MOD_DIR}/qt/models/ontologyclassmodel.h
    ${MOD_DIR}/qt/models/ontologyclassitem.h
    ${MOD_DIR}/qt/processorwidgets/ontologyclassview.h
    ${MOD_DIR}/qt/utils/owlwriter.h
    ${MOD_DIR}/qt/utils/owlparser.h
    ${MOD_DIR}/qt/utils/owlhelperitem.h
    ${MOD_DIR}/qt/processorwidgets/geospatialclassificationwidget.h
    ${MOD_DIR}/qt/processorwidgets/rulewidget.h
    ${MOD_DIR}/qt/processorwidgets/ruleitemview.h
    ${MOD_DIR}/qt/models/ruleitem.h
    ${MOD_DIR}/qt/models/rule.h
    ${MOD_DIR}/qt/utils/ruleutility.h
    ${MOD_DIR}/qt/processorwidgets/comboboxdelegate.h
    ${MOD_DIR}/qt/utils/rulewriter.h
    ${MOD_DIR}/qt/utils/ruleparser.h
)

SET(MOD_QT_HEADERS_NONMOC
    ${MOD_DIR}/qt/processorwidgets/knowledgeprocessorwidgetfactory.h
)

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)

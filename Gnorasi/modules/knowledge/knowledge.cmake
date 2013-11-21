
################################################################################
# Core module resources 
################################################################################
SET(MOD_CORE_MODULECLASS KnowledgeModule)

SET(MOD_CORE_SOURCES
    ${MOD_DIR}/processors/knowledgewebserviceprocessor.cpp
    ${MOD_DIR}/ports/ontologyport.cpp
    ${MOD_DIR}/ports/georuleport.cpp
    ${MOD_DIR}/ports/fuzzyruleport.cpp
    ${MOD_DIR}/processors/ontologyclassificationprocessor.cpp
)

SET(MOD_CORE_HEADERS
    ${MOD_DIR}/ports/ontologyport.h
    ${MOD_DIR}/ports/georuleport.h
    ${MOD_DIR}/ports/fuzzyruleport.h
    ${MOD_DIR}/processors/ontologyclassificationprocessor.h

)

################################################################################
# Qt module resources 
################################################################################
SET(MOD_QT_MODULECLASS KnowledgeModuleQt)

SET(MOD_QT_SOURCES
    ${MOD_DIR}/qt/processorwidgets/knowledgeprocessorwidgetfactory.cpp
    ${MOD_DIR}/qt/processorwidgets/ontologyclassificationprocessorwidget.cpp
    ${MOD_DIR}/qt/processorwidgets/ontologyclassview.cpp
    ${MOD_DIR}/qt/utils/owlwriter.cpp
    ${MOD_DIR}/qt/utils/owlparser.cpp
    ${MOD_DIR}/qt/utils/owlhelperitem.cpp
    ${MOD_DIR}/qt/models/ruleitem.cpp
    ${MOD_DIR}/qt/processorwidgets/comboboxdelegate.cpp
    ${MOD_DIR}/qt/utils/fuzzylabelmaputility.cpp
    ${MOD_DIR}/qt/processorwidgets/doublespinboxdelegate.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionrightshoulder.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionleftshoulder.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionfactory.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunction.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyattribute.cpp
    ${MOD_DIR}/qt/processorwidgets/lineeditfornumbersdelegate.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionleftshoulder.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionrightshoulder.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctiontriangular.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctiontrapezoidal.cpp
    ${MOD_DIR}/qt/models/ontologyclass.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyrule.cpp
    ${MOD_DIR}/qt/utils/ontologyclassificationmanager.cpp
    ${MOD_DIR}/qt/utils/objectattribute.cpp
    ${MOD_DIR}/qt/utils/objectattributemanager.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyoperator.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyoperatormanager.cpp
    ${MOD_DIR}/qt/utils/objectlevel.cpp
    ${MOD_DIR}/qt/utils/objectlevelmanager.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionmanager.cpp
    ${MOD_DIR}/qt/processorwidgets/classhierarchywidget.cpp
    ${MOD_DIR}/qt/processorwidgets/classdescriptiondialog.cpp
    ${MOD_DIR}/qt/processorwidgets/fuzzyruleview.cpp
    ${MOD_DIR}/qt/processorwidgets/membershipfunctiondialog.cpp
    ${MOD_DIR}/qt/processorwidgets/insertexpressiondialog.cpp
    ${MOD_DIR}/qt/processorwidgets/objectattributesview.cpp
    ${MOD_DIR}/qt/fuzzy/fuzzyrulemanager.cpp
    ${MOD_DIR}/qt/processorwidgets/selectoperatordialog.cpp
    ${MOD_DIR}/qt/utils/ontologyclassificationfilereader.cpp
)

SET(MOD_QT_HEADERS
    ${MOD_DIR}/qt/processorwidgets/ontologyclassificationprocessorwidget.h
    ${MOD_DIR}/qt/processorwidgets/ontologyclassview.h
    ${MOD_DIR}/qt/utils/owlwriter.h
    ${MOD_DIR}/qt/utils/owlparser.h
    ${MOD_DIR}/qt/utils/owlhelperitem.h
    ${MOD_DIR}/qt/models/ruleitem.h
    ${MOD_DIR}/qt/processorwidgets/comboboxdelegate.h
    ${MOD_DIR}/qt/utils/fuzzylabelmaputility.h
    ${MOD_DIR}/qt/processorwidgets/doublespinboxdelegate.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionrightshoulder.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionleftshoulder.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionfactory.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunction.h
    ${MOD_DIR}/qt/fuzzy/fuzzyattribute.h
    ${MOD_DIR}/qt/processorwidgets/lineeditfornumbersdelegate.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionleftshoulder.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionrightshoulder.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctiontriangular.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctiontrapezoidal.h
    ${MOD_DIR}/qt/models/ontologyclass.h
    ${MOD_DIR}/qt/fuzzy/fuzzyrule.h
    ${MOD_DIR}/qt/utils/ontologyclassificationmanager.h
    ${MOD_DIR}/qt/utils/objectattribute.h
    ${MOD_DIR}/qt/utils/objectattributemanager.h
    ${MOD_DIR}/qt/fuzzy/fuzzyoperator.h
    ${MOD_DIR}/qt/fuzzy/fuzzyoperatormanager.h
    ${MOD_DIR}/qt/utils/objectlevel.h
    ${MOD_DIR}/qt/utils/objectlevelmanager.h
    ${MOD_DIR}/qt/fuzzy/fuzzyfunctionmanager.h
    ${MOD_DIR}/qt/processorwidgets/classhierarchywidget.h
    ${MOD_DIR}/qt/processorwidgets/classdescriptiondialog.h
    ${MOD_DIR}/qt/processorwidgets/fuzzyruleview.h
    ${MOD_DIR}/qt/processorwidgets/membershipfunctiondialog.h
    ${MOD_DIR}/qt/processorwidgets/insertexpressiondialog.h
    ${MOD_DIR}/qt/processorwidgets/objectattributesview.h
    ${MOD_DIR}/qt/fuzzy/fuzzyrulemanager.h
    ${MOD_DIR}/qt/processorwidgets/selectoperatordialog.h
    ${MOD_DIR}/qt/utils/ontologyclassificationfilereader.h
)

SET(MOD_QT_HEADERS_NONMOC
    ${MOD_DIR}/qt/processorwidgets/knowledgeprocessorwidgetfactory.h
)

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)

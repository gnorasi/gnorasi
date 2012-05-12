# Qt module class  
VRN_QT_MODULE_CLASSES += KnowledgeModuleQt
VRN_QT_MODULE_CLASS_HEADERS += knowledge/knowledgemoduleqt.h
VRN_QT_MODULE_CLASS_SOURCES += knowledge/knowledgemoduleqt.cpp

# Qt resources
HEADERS += $${VRN_MODULE_DIR}/knowledge/qt/processorwidgets/knowledgeprocessorwidgetfactory.h
SOURCES += $${VRN_MODULE_DIR}/knowledge/qt/processorwidgets/knowledgeprocessorwidgetfactory.cpp

HEADERS += $${VRN_MODULE_DIR}/knowledge/qt/processorwidgets/dummysegmentationwidget.h
SOURCES += $${VRN_MODULE_DIR}/knowledge/qt/processorwidgets/dummysegmentationwidget.cpp

HEADERS += $${VRN_MODULE_DIR}/knowledge/qt/processorwidgets/classifierwswidget.h
SOURCES += $${VRN_MODULE_DIR}/knowledge/qt/processorwidgets/classifierwswidget.cpp

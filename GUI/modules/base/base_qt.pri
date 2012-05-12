# Qt module class  
VRN_QT_MODULE_CLASSES += BaseModuleQt
VRN_QT_MODULE_CLASS_HEADERS += base/basemoduleqt.h
VRN_QT_MODULE_CLASS_SOURCES += base/basemoduleqt.cpp

# Qt resources
HEADERS += $${VRN_MODULE_DIR}/base/qt/processorwidgets/baseprocessorwidgetfactory.h
SOURCES += $${VRN_MODULE_DIR}/base/qt/processorwidgets/baseprocessorwidgetfactory.cpp

HEADERS += $${VRN_MODULE_DIR}/base/qt/processorwidgets/dynamicglslwidget.h
SOURCES += $${VRN_MODULE_DIR}/base/qt/processorwidgets/dynamicglslwidget.cpp
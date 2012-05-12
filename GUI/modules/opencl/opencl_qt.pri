# Qt module class  
VRN_QT_MODULE_CLASSES += OpenCLModuleQt
VRN_QT_MODULE_CLASS_HEADERS += opencl/openclmoduleqt.h
VRN_QT_MODULE_CLASS_SOURCES += opencl/openclmoduleqt.cpp

# Qt resources
SOURCES += $${VRN_MODULE_DIR}/opencl/qt/openclpropertywidgetfactory.cpp
SOURCES += $${VRN_MODULE_DIR}/opencl/qt/openclhighlighter.cpp
SOURCES += $${VRN_MODULE_DIR}/opencl/qt/openclpropertywidget.cpp
SOURCES += $${VRN_MODULE_DIR}/opencl/qt/openclplugin.cpp
SOURCES += $${VRN_MODULE_DIR}/opencl/qt/openclprocessorwidgetfactory.cpp
SOURCES += $${VRN_MODULE_DIR}/opencl/qt/dynamicopenclwidget.cpp

HEADERS += $${VRN_MODULE_DIR}/opencl/qt/openclpropertywidgetfactory.h
HEADERS += $${VRN_MODULE_DIR}/opencl/qt/openclhighlighter.h
HEADERS += $${VRN_MODULE_DIR}/opencl/qt/openclpropertywidget.h
HEADERS += $${VRN_MODULE_DIR}/opencl/qt/openclplugin.h
HEADERS += $${VRN_MODULE_DIR}/opencl/qt/openclprocessorwidgetfactory.h
HEADERS += $${VRN_MODULE_DIR}/opencl/qt/dynamicopenclwidget.h

### Local Variables:
### mode:conf-unix
### End:

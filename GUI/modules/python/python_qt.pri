
contains(DEFINES, VRN_DYNAMIC_LIBS) {
    DEFINES += VRN_MODULE_PYTHON_BUILD_DLL
}

# Qt module class  
VRN_QT_MODULE_CLASSES += PythonModuleQt
VRN_QT_MODULE_CLASS_HEADERS += python/pythonmoduleqt.h
VRN_QT_MODULE_CLASS_SOURCES += python/pythonmoduleqt.cpp

SOURCES += $${VRN_MODULE_DIR}/python/qt/pyvoreenqt.cpp
HEADERS += $${VRN_MODULE_DIR}/python/qt/pyvoreenqt.h

SOURCES += $${VRN_MODULE_DIR}/python/qt/pythonhighlighter.cpp
HEADERS += $${VRN_MODULE_DIR}/python/qt/pythonhighlighter.h

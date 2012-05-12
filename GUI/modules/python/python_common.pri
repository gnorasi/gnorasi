DEFINES += VRN_MODULE_PYTHON

# module class  
VRN_MODULE_CLASSES += PythonModule
VRN_MODULE_CLASS_HEADERS += python/pythonmodule.h
VRN_MODULE_CLASS_SOURCES += python/pythonmodule.cpp

# python lib
win32 {
    # use python lib from module directory, if not otherwise specified
    isEmpty(PYTHON_DIR) : PYTHON_DIR = "$${VRN_MODULE_DIR}/python/ext/python27"
    
    INCLUDEPATH += "$${PYTHON_DIR}/include"
}
unix {
    !isEmpty(PYTHON_DIR)  : INCLUDEPATH += "$${PYTHON_DIR}"
    !isEmpty(PYTHON_LIBS) : LIBS += "$${PYTHON_LIBS}"
}
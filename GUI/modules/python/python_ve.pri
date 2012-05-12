
# VE module class  
VRN_VOREENVE_MODULE_CLASSES += PythonModuleVE
VRN_VOREENVE_MODULE_CLASS_HEADERS += python/pythonmoduleve.h
VRN_VOREENVE_MODULE_CLASS_SOURCES += python/pythonmoduleve.cpp

SOURCES += $${VRN_MODULE_DIR}/python/veplugin/pythoneditor.cpp
HEADERS += $${VRN_MODULE_DIR}/python/veplugin/pythoneditor.h

RESOURCES += $${VRN_MODULE_DIR}/python/veplugin/python.qrc

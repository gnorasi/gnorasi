DEFINES += VRN_MODULE_OPENMP

# module class  
VRN_MODULE_CLASSES += OpenMPModule
VRN_MODULE_CLASS_HEADERS += openmp/openmpmodule.h
VRN_MODULE_CLASS_SOURCES += openmp/openmpmodule.cpp

# enable OpenMP
win32-msvc: QMAKE_CXXFLAGS += /openmp
unix: QMAKE_CXXFLAGS += -fopenmp

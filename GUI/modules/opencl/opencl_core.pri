
contains(DEFINES, VRN_DYNAMIC_LIBS) {
    DEFINES += VRN_MODULE_OPENCL_BUILD_DLL
}

SOURCES += \
    $${VRN_MODULE_DIR}/opencl/utils/clwrapper.cpp \
    $${VRN_MODULE_DIR}/opencl/processors/dynamicclprocessor.cpp \
    $${VRN_MODULE_DIR}/opencl/processors/grayscale_cl.cpp \
    $${VRN_MODULE_DIR}/opencl/processors/raycaster_cl.cpp \
    $${VRN_MODULE_DIR}/opencl/processors/raytracingentryexitpoints.cpp \
    $${VRN_MODULE_DIR}/opencl/processors/volumegradient_cl.cpp \
    $${VRN_MODULE_DIR}/opencl/utils/voreenblascl.cpp

HEADERS += \
    $${VRN_MODULE_DIR}/opencl/utils/clwrapper.h \
    $${VRN_MODULE_DIR}/opencl/processors/dynamicclprocessor.h \
    $${VRN_MODULE_DIR}/opencl/processors/grayscale_cl.h \
    $${VRN_MODULE_DIR}/opencl/processors/raycaster_cl.h \
    $${VRN_MODULE_DIR}/opencl/processors/raytracingentryexitpoints.h \
    $${VRN_MODULE_DIR}/opencl/processors/volumegradient_cl.h \
    $${VRN_MODULE_DIR}/opencl/utils/voreenblascl.h

# properties
SOURCES += $${VRN_MODULE_DIR}/opencl/properties/openclproperty.cpp
HEADERS += $${VRN_MODULE_DIR}/opencl/properties/openclproperty.h

SHADER_SOURCES += \
    $${VRN_MODULE_DIR}/opencl/cl/grayscale.cl \
    $${VRN_MODULE_DIR}/opencl/cl/gradient.cl \
    $${VRN_MODULE_DIR}/opencl/cl/mod_gradients.cl \
    $${VRN_MODULE_DIR}/opencl/cl/mod_samplers.cl \
    $${VRN_MODULE_DIR}/opencl/cl/mod_volumewrite.cl \
    $${VRN_MODULE_DIR}/opencl/cl/raycaster.cl \
    $${VRN_MODULE_DIR}/opencl/cl/raytracingentryexitpoints.cl \
    $${VRN_MODULE_DIR}/opencl/cl/simple.cl \
    $${VRN_MODULE_DIR}/opencl/cl/voreenblas.cl

### Local Variables:
### mode:conf-unix
### End:

# libraries to link

win32 {
    isEmpty(NVIDIA_GPU_COMPUTING_SDK) {
        warning("OpenCL module: NVIDIA_GPU_COMPUTING_SDK not set")
    }
    
    win32-msvc {
        LIBS += -L$${NVIDIA_GPU_COMPUTING_SDK}/OpenCL/common/lib/Win32
        LIBS += -L$${NVIDIA_GPU_COMPUTING_SDK}/lib/Win32
    }
    
    win64-msvc {
        LIBS += -L$${NVIDIA_GPU_COMPUTING_SDK}/OpenCL/common/lib/x64
        LIBS += -L$${NVIDIA_GPU_COMPUTING_SDK}/lib/x64
    }
    
    LIBS += -L$${NVIDIA_GPU_COMPUTING_SDK}/lib
    LIBS += -L$${NVIDIA_GPU_COMPUTING_SDK}
    LIBS += -lOpenCL
}

unix : !macx {
    #QMAKE_LFLAGS += -lOpenCL
    LIBS += -lOpenCL
}

macx {
    LIBS += -framework OpenCL
}

### Local Variables:
### mode:conf-unix
### End:

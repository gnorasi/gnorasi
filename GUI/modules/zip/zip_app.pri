# link against ZLIB

win32 {
    isEmpty(ZLIB_DIR) : warn('ZLIB_DIR not set')

    # VC++ 32 bit
    win32-msvc {
        LIBS += "$${ZLIB_DIR}/lib/win32/zdll.lib"
        VRN_MODULE_DLLS += $${ZLIB_DIR}\\lib\\win32\\zlib1.dll 
    }

    # VC++ 64 bit
    win64-msvc {
        LIBS += "$${ZLIB_DIR}/lib/win64/zlibwapi.lib"
        VRN_MODULE_DLLS += $${ZLIB_DIR}\\lib\\win64\\zlibwapi.dll 
    }
    
    # MinGW 32 bit
    win32-g++ {
        LIBS += "$${ZLIB_DIR}/lib/win32/zdll.lib"
        VRN_MODULE_DLLS += $${ZLIB_DIR}\\lib\\win32\\zlib1.dll 
    }
}

unix {  
    LIBS += -lz
}

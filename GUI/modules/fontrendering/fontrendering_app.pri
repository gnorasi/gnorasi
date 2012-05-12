# link against freetype and ftgl libraries

win32 {
    isEmpty(FREETYPE_DIR) : warn('FREETYPE_DIR not set')
    isEmpty(FTGL_DIR) :     warn('FTGL_DIR not set')
    
    # VC++/MinGW 32 bit
    win32-msvc | win32-g++ {
        LIBS += "$${FREETYPE_DIR}/lib/win32/$${WIN32_CONFIG_NAME}/freetype.lib"
        VRN_MODULE_DLLS += "$${FREETYPE_DIR}\\lib\\win32\\$${WIN32_CONFIG_NAME}\\freetype.dll"
        
        # link ftgl statically, since dynamic linking yields linking errors
        LIBS += "$${FTGL_DIR}/lib/win32/$${WIN32_CONFIG_NAME}/ftgl.lib"
        VRN_MODULE_DLLS += "$${FTGL_DIR}\\lib\\win32\\$${WIN32_CONFIG_NAME}\\ftgl.dll"
    }

    # VC++ 64 bit
    win64-msvc {
        LIBS += "$${FREETYPE_DIR}/lib/win64/$${WIN64_CONFIG_NAME}/freetype.lib"
        VRN_MODULE_DLLS += "$${FREETYPE_DIR}\\lib\\win64\\$${WIN64_CONFIG_NAME}\\freetype.dll"
        
        # link ftgl statically, since dynamic linking yields linking errors
        LIBS += "$${FTGL_DIR}/lib/win64/$${WIN64_CONFIG_NAME}/ftgl.lib"
        VRN_MODULE_DLLS += "$${FTGL_DIR}\\lib\\win64\\$${WIN64_CONFIG_NAME}\\ftgl.dll"
    }

}

unix {
    LIBS += -lftgl -lfreetype 
}

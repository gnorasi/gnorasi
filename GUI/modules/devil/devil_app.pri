# link against DevIL library

win32 {
    isEmpty(DEVIL_DIR) : warn('DEVIL_DIR not set')

    !win64-msvc {
        LIBS += "$${DEVIL_DIR}/lib/win32/DevIL.lib"
        
        VRN_MODULE_DLLS += \
            $${DEVIL_DIR}\\lib\\win32\\DevIL.dll \
            $${DEVIL_DIR}\\lib\\win32\\ILU.dll \
            $${DEVIL_DIR}\\lib\\win32\\ILUT.dll
    }
    else {
        LIBS += "$${DEVIL_DIR}/lib/win64/DevIL.lib"
        
        VRN_MODULE_DLLS += \
            $${DEVIL_DIR}\\lib\\win64\\DevIL.dll \
            $${DEVIL_DIR}\\lib\\win64\\ILU.dll \
            $${DEVIL_DIR}\\lib\\win64\\ILUT.dll
    }
    
    VRN_MODULE_DLLS += $${VRN_MODULE_DIR}\\devil\\ext\\jpeg\\jpeg62.dll
}

unix {  
    LIBS += -lIL
}

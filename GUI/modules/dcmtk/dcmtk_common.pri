# module availability macro
DEFINES += VRN_MODULE_DCMTK

# module class  
VRN_MODULE_CLASSES += DcmtkModule
VRN_MODULE_CLASS_HEADERS += dcmtk/dcmtkmodule.h
VRN_MODULE_CLASS_SOURCES += dcmtk/dcmtkmodule.cpp

# check version define
!contains(DEFINES, VRN_DCMTK_VERSION_354) : !contains(DEFINES, VRN_DCMTK_VERSION_360) {
    error("DCMTK module: DCMTK version not specified. \
           Please add 'DEFINES += VRN_DCMTK_VERSION_354' or 'DEFINES += VRN_DCMTK_VERSION_360' \
           to your config.txt!")
}

# windows include path
win32 {
    #
    # VC++ 32 bit build 
    #
    win32-msvc { 
        # DCMTK
        INCLUDEPATH += "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/include"
        !exists("$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/include/dcmtk/config/osconfig.h") {
            error("DCMTK module: DCMTK library not found (osconfig.h). \
                   Copy DCMTK headers to $${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/include/ (see README.txt).")
        }

        # OpenSSL (only for v3.6.0)
        contains(DEFINES, VRN_DCMTK_VERSION_360) {
            INCLUDEPATH += "$${VRN_MODULE_DIR}/dcmtk/ext/openssl/include"
            !exists("$${VRN_MODULE_DIR}/dcmtk/ext/openssl/include/openssl/opensslconf.h") {
                error("DCMTK module: OpenSSL library not found (opensslconf.h). \
                       Copy OpenSSL headers to $${VRN_MODULE_DIR}/dcmtk/ext/openssl/include/ (see README.txt).")
            }
        }
    }
    
    #
    # VC++ 64 bit build (without dependencies, esp. OpenSSL)
    #
    win64-msvc { 
        # DCMTK
        INCLUDEPATH += "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/include/win64"
        !exists("$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/include/win64/dcmtk/config/osconfig.h") {
            error("DCMTK module: DCMTK library not found (osconfig.h). \
                   Copy DCMTK headers to $${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/include/win64/")
        }
    }
}

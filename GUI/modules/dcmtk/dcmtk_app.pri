# specify libraries to link

unix {
    LIBS += -lz -lssl
    !without_libwrap: LIBS += -lwrap

    LIBS += -ldcmimage -ldcmimgle -ldcmnet -ldcmdata \
            -ldcmjpeg -lijg8 -lijg12 -lijg16 -lofstd
    contains(DEFINES, VRN_DCMTK_VERSION_360) {
        LIBS += -loflog
    }
    
    LIBS += -ldcmtls

    macx: LIBS += -lcrypto
}

win32 {
    #
    # VC++ 32 bit build 
    #
    win32-msvc { 
        # DCMTK
        exists("$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/Debug/dcmimage.lib") {
            LIBS += "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmdata.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmdsig.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmimage.lib" \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmimgle.lib" \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmjpeg.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmnet.lib"   \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmpstat.lib" \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmqrdb.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmsr.lib"    \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmtls.lib"   \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/dcmwlm.lib"   \            
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/ijg8.lib"     \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/ijg12.lib"    \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/ijg16.lib"    \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/ofstd.lib"   

            contains(DEFINES, VRN_DCMTK_VERSION_360) {
                LIBS += "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/$${WIN32_CONFIG_NAME}/oflog.lib"
            }
        }
        else { 
            error("DCMTK module: DCMTK library not found (dcmimage.lib). \ 
                   Copy DCMTK libs to $${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win32/[Debug|Release]/ (see README.txt)")      
        }  
  
        # OpenSSL (only for v3.6.0)
        contains(DEFINES, VRN_DCMTK_VERSION_360) {
            exists("$${VRN_MODULE_DIR}/dcmtk/ext/openssl/lib/win32/dcmtkssl_d.lib") {
                LIBS += "$${VRN_MODULE_DIR}/dcmtk/ext/openssl/lib/win32/dcmtkssl_d.lib"  
                LIBS += "$${VRN_MODULE_DIR}/dcmtk/ext/openssl/lib/win32/dcmtkeay_d.lib"
    
                VRN_MODULE_DLLS += "$${VRN_MODULE_DIR}\\dcmtk\\ext\\openssl\\lib\\win32\\dcmtkssl.dll"
                VRN_MODULE_DLLS += "$${VRN_MODULE_DIR}\\dcmtk\\ext\\openssl\\lib\\win32\\dcmtkeay.dll"
            }  
            else { 
                error("DCMTK module: OpenSSL library not found (dcmtkssl_d.lib). \ 
                       Copy OpenSSL libs to $${VRN_MODULE_DIR}/dcmtk/ext/openssl/lib/win32/ (see README.txt)")      
            }
        }
    }
    
    #
    # VC++ 64 bit build
    #
    win64-msvc { 
        # DCMTK
        exists("$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/Debug/dcmimage.lib") {
            LIBS += "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmdata.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmdsig.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmimage.lib" \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmimgle.lib" \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmjpeg.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmnet.lib"   \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmpstat.lib" \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmqrdb.lib"  \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmsr.lib"    \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmtls.lib"   \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/dcmwlm.lib"   \            
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/ijg8.lib"     \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/ijg12.lib"    \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/ijg16.lib"    \
                    "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/ofstd.lib"   

            contains(DEFINES, VRN_DCMTK_VERSION_360) {
                LIBS += "$${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/$${WIN64_CONFIG_NAME}/oflog.lib"
            }
        }
        else { 
            error("DCMTK module: DCMTK library not found (dcmimage.lib). \ 
                   Copy DCMTK libs to $${VRN_MODULE_DIR}/dcmtk/ext/dcmtk/lib/win64/[Debug|Release]/ (see README.txt)")      
        }  
    }

} # win32

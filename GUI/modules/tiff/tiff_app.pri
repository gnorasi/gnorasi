# link against libtiff

win32 {
  
  win32-msvc | win32-g++ {
    # dynamic lib
    LIBS += "$${VRN_MODULE_DIR}/tiff/ext/libtiff/lib/win32/$${WIN32_CONFIG_NAME}/libtiff.lib"
    VRN_MODULE_DLLS += $${VRN_MODULE_DIR}\\tiff\\ext\\libtiff\\lib\\win32\\$${WIN32_CONFIG_NAME}\\libtiff.dll
    
    # static lib (use in case of problems with dynamic linking, e.g. manifest issues)
    #LIBS += "$${VRN_MODULE_DIR}/tiff/ext/libtiff/lib/win32/$${WIN32_CONFIG_NAME}/libtiff_s.lib"
  }
  
  win64-msvc {
    # dynamic lib
    LIBS += "$${VRN_MODULE_DIR}/tiff/ext/libtiff/lib/win64/$${WIN64_CONFIG_NAME}/libtiff.lib"
    VRN_MODULE_DLLS += $${VRN_MODULE_DIR}\\tiff\\ext\\libtiff\\lib\\win64\\$${WIN64_CONFIG_NAME}\\libtiff.dll
    
    # static lib (use in case of problems with dynamic linking, e.g. manifest issues)
    #LIBS += "$${VRN_MODULE_DIR}/tiff/ext/libtiff/lib/win64/$${WIN64_CONFIG_NAME}/libtiff_s.lib"
  }
  
}

unix {
  LIBS += -ltiff
}

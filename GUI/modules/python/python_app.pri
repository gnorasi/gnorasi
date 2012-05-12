
win32 {
    isEmpty(PYTHON_DIR) : warn('PYTHON_DIR not set')

    # VC++/MinGW 32 bit
    win32-msvc | win32-g++ {
        LIBS += "$${PYTHON_DIR}/lib/win32/$${WIN32_CONFIG_NAME}/python27.lib"
        VRN_MODULE_DLLS += $${PYTHON_DIR}\\lib\\win32\\$${WIN32_CONFIG_NAME}\\python27.dll
    }

    # VC++ 64 bit
    win64-msvc {
        LIBS += "$${PYTHON_DIR}/lib/win64/$${WIN64_CONFIG_NAME}/python27.lib"
        VRN_MODULE_DLLS += $${PYTHON_DIR}\\lib\\win64\\$${WIN64_CONFIG_NAME}\\python27.dll
    }
}
#######################################################
# Generic project file for all Voreen applications
# Make sure to include 'commonconf.pri' before this one 
# in the project file!
#######################################################

# environment checks
win32 {
  isEmpty(WIN32_CONFIG_NAME) : isEmpty(WIN64_CONFIG_NAME) : error("voreenapp.pri: neither WIN32_CONFIG_NAME nor WIN64_CONFIG_NAME is set. Did you forget to include commonconf.pri in your project file?")
}
  
# set output directory to default value, if not specified
isEmpty(VRN_APP_DIRECTORY) {
  VRN_APP_DIRECTORY = "$${VRN_HOME}/bin" 
}

macx: DESTDIR = "$${VRN_APP_DIRECTORY}"
unix: DESTDIR = "$${VRN_APP_DIRECTORY}"
win32: {
  CONFIG(debug, debug|release) {
    DESTDIR = "$${VRN_APP_DIRECTORY}/Debug"
  }
  else {
    DESTDIR = "$${VRN_APP_DIRECTORY}/Release"
  }
}

# pre-compiled headers
contains(DEFINES, VRN_PRECOMPILE_HEADER) { 
  qt {
    PRECOMPILED_HEADER = ../pch_qtapp.h
  }
  else {
    PRECOMPILED_HEADER = ../../pch.h
  }  CONFIG += precompile_header
}
else {
  CONFIG -= precompile_header
}


####################################################
# Platform-dependant configuration
####################################################

win32 {
  QMAKE_LIBDIR += "$${VRN_HOME}/ext/glew/lib"
  
  LIBS += -lnetapi32 -lopengl32 -lglu32
  
  win32-msvc { # win 32 bit configuration 
    qt: LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/voreen_qt.lib" -lqtmain
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/voreen_core.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/tgt.lib"
    
    LIBS += "$${VRN_HOME}/ext/glew/lib/win32/vs$${VS_VERSION_NUMBER}/$${WIN32_CONFIG_NAME}/glew32s.lib"
    
    # no libc for vc++ since we build a multithreaded executable
    LIBS += /NODEFAULTLIB:libc.lib

    # these libs are not automatically added when building with nmake
    nmake: LIBS *= advapi32.lib shell32.lib
    
    # Windows Management Instrumentation (WMI) for hardware detection
    contains(DEFINES, TGT_WITH_WMI) : LIBS += -lWbemUuid
  }
  
  win64-msvc { # win 64 bit configuration
    qt: LIBS += "$${VRN_HOME}/bin/$${WIN64_CONFIG_NAME}/voreen_qt.lib" -lqtmain
    LIBS += "$${VRN_HOME}/bin/$${WIN64_CONFIG_NAME}/voreen_core.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN64_CONFIG_NAME}/tgt.lib"
    
    LIBS += "$${VRN_HOME}/ext/glew/lib/win64/vs$${VS_VERSION_NUMBER}/$${WIN64_CONFIG_NAME}/glew64s.lib"
    
    # no libc for vc++ since we build a multithreaded executable
    LIBS += /NODEFAULTLIB:libc.lib

    # these libs are not automatically added when building with nmake
    nmake: LIBS *= advapi32.lib shell32.lib
    
    # Windows Management Instrumentation (WMI) for hardware detection
    contains(DEFINES, TGT_WITH_WMI) : LIBS += -lWbemUuid
  }

  win32-g++ {
    qt: LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/libvoreen_qt.a"
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/libvoreen_core.a"
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/libtgt.a"
    
    LIBS += "$${VRN_HOME}/ext/glew/lib/mingw/glew32s.lib"
  }
   

  # For reading file version, file date and making registry calls
  # via Windows API
  LIBS += -lVersion
}

unix {
  DEFINES += LINUX

  qt : LIBS += -lvoreen_qt
  LIBS += -lvoreen_core
  LIBS += -ltgt
  !macx: LIBS += -lGL -lGLU
  LIBS += -lGLEW

  contains(DEFINES, VRN_WITH_LZO) {
    LIBS += -llzo2
  }

  !isEmpty(INSTALL_PREFIX) {
    target.path = $$INSTALLPATH_BIN
    INSTALLS += target
  }
}

macx {
  LIBS += -framework OpenGL
  LIBS += -framework ApplicationServices
  LIBS += -framework CoreFoundation
}

########################################################################
# Include modules which are selected in local configuration. The entry
# 'foo' in VRN_MODULES must correspond to a subdir 'modules/foo' and a
# file 'foo_app.pri' there.
########################################################################
for(i, VRN_MODULES) : exists($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri) {
  include($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri)
}

# Copy Win32 DLLs to application directory (Visual Studio post build action)
win32-msvc || win64-msvc {
  EXTDIR = $${VRN_HOME}/ext
  VRN_DEST_DIR = $${PWD}/$${VRN_APP_DIRECTORY}/$${WIN32_CONFIG_NAME}

  for(dll, VRN_MODULE_DLLS) : {
    QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$${dll}\" \"$${DESTDIR}\" > NUL $$escape_expand(\\n))
  }
}

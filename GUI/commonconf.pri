##########################################
# Configurations common for all projects #
##########################################

CONFIG += stl rtti thread
 
# directories for installation on unix
unix: !isEmpty(INSTALL_PREFIX) {
  DEFINES += 'VRN_INSTALL_PREFIX="$$INSTALL_PREFIX"'
  INSTALLPATH_BIN = $$INSTALL_PREFIX/bin
  INSTALLPATH_LIB = $$INSTALL_PREFIX/lib
  INSTALLPATH_SHARE = $$INSTALL_PREFIX/share/voreen
  INSTALLPATH_DOC = $$INSTALL_PREFIX/share/doc/voreen
  DEFINES *= VRN_DEPLOYMENT
}

# detect Microsoft Compiler
MSC_VER = $$find(QMAKE_COMPILER_DEFINES, "_MSC_VER")
!isEmpty(MSC_VER) {

  !contains(QMAKE_HOST.arch, x86_64) {
    # MSVC++ 32 bit build
    CONFIG += win32-msvc  
  }
  else {
    # MSVC++ 64 bit build
    CONFIG += win64-msvc  
  }  

  # are we running through Visual Studio or nmake?
  contains(TEMPLATE, "vcapp") {
    CONFIG += visual_studio
  }  else:contains(TEMPLATE, "vclib") {
    CONFIG += visual_studio
  } else {
    CONFIG += nmake
  }
  
  contains(MSC_VER, 1600) {
    VS_VERSION_NUMBER = 2010
  }
  else {
    VS_VERSION_NUMBER = 2008
  }
  
  visual_studio {
    # Contains "Release" or "Debug" as selected in the IDE
    win32-msvc: WIN32_CONFIG_NAME = $(ConfigurationName)
    win64-msvc: WIN64_CONFIG_NAME = $(ConfigurationName)
  }
  else {
    CONFIG(debug, debug|release) {
      win32-msvc: WIN32_CONFIG_NAME = Debug
      win64-msvc: WIN64_CONFIG_NAME = Debug
    } else {
      win32-msvc: WIN32_CONFIG_NAME = Release
      win64-msvc: WIN64_CONFIG_NAME = Release
    }
  }
 
  contains(DEFINES, VRN_DYNAMIC_LIBS) {
    # Linking against Windows DLLs requires explicit instantiation of templates
    DEFINES += DLL_TEMPLATE_INST
  }
}

# build configuration for mingw compiler
win32-g++ {
    CONFIG(debug, debug|release) {
        WIN32_CONFIG_NAME = Debug
    } 
    else {
        WIN32_CONFIG_NAME = Release
    }
}

#####################
# tgt configuration #
#####################

# we don't use tgt's build system
DEFINES += TGT_WITHOUT_DEFINES

# tgt defines #
contains(DEFINES, VRN_DEBUG) {
  DEFINES += TGT_DEBUG
}

###########
# end tgt #
###########

contains(DEFINES, VRN_PRECOMPILE_HEADER) : contains(QMAKE_CC, "icc") {
  warning("Disabled precompiled headers because compiler is icc.")
  DEFINES -= VRN_PRECOMPILE_HEADER
}

win32 {
  CONFIG(debug, debug|release): DEFINES += _DEBUG
} else {
  debug : DEFINES += _DEBUG
}

INCLUDEPATH += \
  "$${VRN_HOME}" \
  "$${VRN_HOME}/include" \
  "$${VRN_HOME}/ext"

# This variable contains the list of all directories to look in to resolve
# dependencies. This will be used when crawling through included files.
DEPENDPATH += \
  "$${VRN_HOME}/include" \
  "$${VRN_HOME}/modules" \
  "$${VRN_HOME}/ext"

# some definitions for modules
VRN_MODULE_DIR=$${VRN_HOME}/modules

# meta-modules that include default modules 
# (= modules without dependencies on external libs)
contains(VRN_MODULES, most) {
  warning("Meta-module 'most' has been replaced by 'default', please adapt your config.txt")
  VRN_MODULES += default
}
contains(VRN_MODULES, default) {
  include(modules/default.pri)
  !contains(DEFINES, VRN_DEPLOYMENT) : exists(modules/default-internal.pri) : include(modules/default-internal.pri)
}
# remove meta-modules and duplicates
VRN_MODULES -= all most default
VRN_MODULES = $$unique(VRN_MODULES)

contains(DEFINES, VRN_NO_MODULE_AUTO_REGISTRATION) {
   DEFINES -= VRN_NO_MODULE_AUTO_REGISTRATION
   DEFINES += VRN_NO_REGISTRATION_HEADER_GENERATION
}

########################
# Module configuration #
########################

# Include modules which are selected in local configuration. 
# The entry 'foo' in VRN_MODULES must correspond to a subdir 'modules/foo'.

# First, look for dependency files 'foo_depends.pri' in the module subdir 
# specifying which other modules a specific module depends on. This file is optional.
for(i, VRN_MODULES) : {
  exists($${VRN_MODULE_DIR}/$${i}/$${i}_depends.pri) {
    include($${VRN_MODULE_DIR}/$${i}/$${i}_depends.pri)
  }
}

contains(VRN_MODULES, core) {
  warning("Module core was renamed to base, please adapt your config.txt")
  VRN_MODULES -= core
  VRN_MODULES += base
}

contains(DEFINES, VRN_WITH_DCMTK) {
  warning("DICOM support is now provided by the 'dcmtk' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_DCMTK
  VRN_MODULES += dcmtk
}
contains(VRN_MODULES, dicom) {
  warning("The 'dicom' module has been renamed to 'dcmtk', please adapt your config.txt")
  VRN_MODULES -= dicom
  VRN_MODULES += dcmtk
}

contains(DEFINES, VRN_WITH_DEVIL) {
  warning("DevIL support is now provided by the 'devil' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_DEVIL
  VRN_MODULES += devil
}

contains(DEFINES, VRN_WITH_FFMPEG) {
  warning("FFmpeg support is now provided by the 'ffmpeg' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_FFMPEG
  VRN_MODULES += ffmpeg
}

contains(DEFINES, VRN_WITH_FONTRENDERING) {
  warning("Font rendering is now provided by the 'fontrendering' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_FONTRENDERING
  VRN_MODULES += fontrendering
}

contains(DEFINES, VRN_WITH_TIFF) {
  warning("TIFF support is now provided by the 'tiff' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_TIFF
  VRN_MODULES += tiff
}

contains(DEFINES, VRN_WITH_PYTHON) {
  warning("Python bindings are now provided by the 'python' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_PYTHON
  VRN_MODULES += python
}

contains(DEFINES, VRN_WITH_ZLIB) {
  warning("ZIP support is now provided by the 'zip' module, please adapt your config.txt")
  DEFINES -= VRN_WITH_ZLIB
  VRN_MODULES += zip
}

# again remove duplicates
VRN_MODULES = $$unique(VRN_MODULES)

# Include module configuration files 'foo_common.pri' in the module subdir.
# This file is mandatory.
for(i, VRN_MODULES) : include($${VRN_MODULE_DIR}/$${i}/$${i}_common.pri)

##############
# Libraries  #
##############

  
####################################
# Platform-dependent configuration #
####################################

####################
# Windows settings #
####################
win32 {
  !win32-msvc2010: CONFIG -= flat
  
  DEFINES += _WINDOWS
  DEFINES -= UNICODE 

  # disable vc 2005 deprecated warnings
  DEFINES += _CRT_SECURE_NO_DEPRECATE

  # we don't like the windows.h min/max macros
  DEFINES += NOMINMAX

  # use static libraries
  DEFINES += GLEW_STATIC

  INCLUDEPATH += "$${VRN_HOME}/ext/glew/include"

  # common compiler/linker flags for VC++ 32/64 bit builds
  win32-msvc | win64-msvc {
    # Allow 2^32 instead of 2^16 addressable sections per obj-file
    QMAKE_CXXFLAGS += /bigobj
   
    # Disable warnings for Microsoft compiler:
    # C4305: 'identifier' : truncation from 'type1' to 'type2'
    # C4800: 'type' : forcing value to bool 'true' or 'false' (performance warning
    # C4290: C++ exception specification ignored except to indicate a function is
    #       not __declspec(nothrow)
    # C4068: unknown pragma
    # C4251 class needs to have dll interface (used for std classes)
    QMAKE_CXXFLAGS += /wd4305 /wd4800 /wd4290 /wd4251
    
    # C4355: 'this' : used in base member initializer list 
    # Occurs in processors' constructors when initializing event properties, 
    # but is safe there, since the 'this' pointer is only stored and not accessed.
    QMAKE_CXXFLAGS += /wd4355 
    
    # Shift warning
    # C4706: assignment within conditional expression
    # to warning level 3 which is used for compiling Voreen
    QMAKE_CXXFLAGS += /w34706
  
    # Disable optimization in debug build
    QMAKE_CXXFLAGS_DEBUG += /Od /GR
  
    # Enable edit and continue in debug build
    QMAKE_CXXFLAGS_DEBUG -= -Zi
    QMAKE_CXXFLAGS_DEBUG -= /Zi
    QMAKE_CXXFLAGS_DEBUG += /ZI

    # Enable parallel compiler processes with Visual Studio
    QMAKE_CXXFLAGS += /MP 
    # Enable static code analysis with PREfast in Visual Studio
    # QMAKE_CXXFLAGS += /analyze

    # Disable warnings for Microsoft linker:
    # LNK4049 / LNK4217: locally defined 'symbol' imported (occurs for some external libs) 
    QMAKE_LFLAGS += /ignore:4049 /ignore:4217
    
    # No libcmt when using DCMTK
    contains(VRN_MODULES, dcmtk){
      LIBS += /NODEFAULTLIB:libcmt.lib
    }
  }
  
  # VC++ 32 bit settings
  win32-msvc {
    QMAKE_LFLAGS += /MACHINE:X86
  }
  
  # VC++ 64 bit settings
  win64-msvc {
    QMAKE_LFLAGS += /MACHINE:X64
  }
  
  CONFIG(debug, debug|release) {
    OBJECTS_DIR = Debug
    DESTDIR = Debug
  } else {
    OBJECTS_DIR = Release
    DESTDIR = Release
  }

  # Disable optimizations for builds with bugtrap support
  contains (DEFINES, VRN_WITH_BUGTRAP) {
    QMAKE_CXXFLAGS_RELEASE -= /O2
    QMAKE_CXXFLAGS_RELEASE += /Od /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG
  }
}

#################
# Unix settings #
#################
unix {
  DEFINES += UNIX

  # The RPATH is used to find the Voreen libraries at runtime. It is
  # set to the library position in to source directory only when we
  # are not installing, i.e., the INSTALL_PREFIX is not set.
  QMAKE_RPATHDIR += $${LIBDIR}
  isEmpty(INSTALL_PREFIX) {
    QMAKE_RPATHDIR += $${VRN_HOME}/bin
  } else {
    QMAKE_RPATHDIR += $${INSTALLPATH_LIB}
  }

  LIBDIR += $${VRN_HOME}/bin

  for(dir, LIBDIR) {
    LIBS  += "-L$$dir"
  }
  !macx {
  	LIBS += -lrt
  }

  isEmpty(OBJECTS_DIR) {
    OBJECTS_DIR = .obj
  }
  
  MOC_DIR = .moc
  UI_DIR = .ui

  
#################################
# additional Macintosh settings #
#################################
  macx {   
    # workaround for inconsistent symbol visibility flags
    # set by Xcode for executables and libraries
    QMAKE_CXXFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility-inlines-hidden
    LIBS += -framework GLUT
    LIBS += -framework OpenGL

    INCLUDEPATH += $${GLEW_DIR}
    LIBS += $${GLEW_LIBS}

    # support for macports
    INCLUDEPATH += /opt/local/include/
    LIBS += -L/opt/local/lib
  }
}

################
# SVN settings #
################
contains(DEFINES, VRN_WITH_SVNVERSION) {
  win32 {
    revtarget.target     = $${VRN_HOME}/include/voreen/core/svnversion.h
    revtarget.commands   = $${VRN_HOME}/tools/svnversion.bat $${VRN_HOME} $${revtarget.target}
    revtarget.CONFIG += recursive
    revtarget.OUTPUT =  $${VRN_HOME}/include/voreen/core/svnversion.h
    PRE_TARGETDEPS      += $${revtarget.target}
    QMAKE_EXTRA_TARGETS += revtarget
  }

  unix {
    QMAKE_EXTRA_TARGETS += revtarget
    revtarget.target     = $${VRN_HOME}/include/voreen/core/svnversion.h
    revtarget.commands   = $${VRN_HOME}/tools/svnversion.sh $${VRN_HOME} $${revtarget.target}
    PRE_TARGETDEPS      += $${revtarget.target}
  }

  # The following must be placed in the .pro file, after all source and header file have been added
  # contains(DEFINES, VRN_WITH_SVNVERSION) : revtarget.depends = $$SOURCES $$HEADERS $$FORMS
}


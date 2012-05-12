TARGET = simple-glut
TEMPLATE = app
LANGUAGE = C++
CONFIG += console
CONFIG -= qt

# Include local configuration
include(../../config.txt)

# Include common configuration
include(../../commonconf.pri)

# Include generic app configuration
include(../voreenapp.pri)

win32 {
  INCLUDEPATH += \
    $${VRN_HOME}/ext/glut/ \
    $${VRN_HOME}
  
  DEFINES += GLUT_NO_LIB_PRAGMA
  
  win32-msvc | win32-g++ {
    LIBS += $${VRN_HOME}/ext/glut/lib/win32/glut32.lib
    QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$${VRN_HOME}\\ext\\glut\\lib\\win32\\glut32.dll\" \"$${DESTDIR}\" > NUL $$escape_expand(\\n))
  }
  win64-msvc {
    LIBS += $${VRN_HOME}/ext/glut/lib/win64/glut64.lib
    QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$${VRN_HOME}\\ext\\glut\\lib\\win64\\glut64.dll\" \"$${DESTDIR}\" > NUL $$escape_expand(\\n))
  }
}

unix {
  LIBS += -lglut
}

SOURCES += \
    simple-glut.cpp \
    $${VRN_HOME}/ext/tgt/glut/glutcanvas.cpp \
    $${VRN_HOME}/ext/tgt/glut/glutmouse.cpp

HEADERS += \
    $${VRN_HOME}/ext/tgt/glut/glutcanvas.h \
    $${VRN_HOME}/ext/tgt/glut/glutmouse.h 

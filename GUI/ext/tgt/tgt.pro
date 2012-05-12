TEMPLATE = lib
TARGET = tgt
unix: VERSION = 1.0

CONFIG += thread
CONFIG -= qt

# include config
!exists(../../config.txt) {
  error("config.txt not found! copy config-default.txt to config.txt and edit!")
}
include(../../config.txt)

# Include common configuration
include(../../commonconf.pri)

win32|macx {
	contains(DEFINES, VRN_DYNAMIC_LIBS) {
        CONFIG += dll
        CONFIG -= static embed_manifest_dll
        DEFINES += TGT_BUILD_DLL
    }
    else {
        CONFIG += static
        CONFIG -= dll
    }
}

win32 { # common configuration for win 32 and 64 bit builds    
    QMAKE_LIBDIR += "$${VRN_HOME}/ext/glew/lib"

    LIBS += -lnetapi32 -lopengl32 -lglu32
    LIBS += -lVersion

    nmake: LIBS *= advapi32.lib shell32.lib

    # no libc for vc++ since we build a multithreaded executable
    LIBS += /NODEFAULTLIB:libc.lib
    
    contains(DEFINES, TGT_WITH_WMI) {
        LIBS += -lWbemUuid
        # for CoInitializeSecurity() to be defined
        DEFINES += _WIN32_DCOM
    }

    for(i, VRN_MODULES) : exists($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri) {
        include($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri)
    }
}

win32-msvc { # win 32 bit configuration
    LIBS += "$${VRN_HOME}/ext/glew/lib/win32/vs$${VS_VERSION_NUMBER}/$${WIN32_CONFIG_NAME}/glew32s.lib"
}
  
win64-msvc { # win 64 bit configuration
    LIBS += "$${VRN_HOME}/ext/glew/lib/win64/vs$${VS_VERSION_NUMBER}/$${WIN64_CONFIG_NAME}/glew64s.lib"
}

# Destination directory for the library
macx: {
	DESTDIR = "$${VRN_HOME}/bin"
	QMAKE_LFLAGS_SONAME = -install_name$${LITERAL_WHITESPACE}$${VRN_HOME}/bin/
}

unix: DESTDIR = "$${VRN_HOME}/bin"
win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = "$${VRN_HOME}/bin/Debug"
    }
    else {
        DESTDIR = "$${VRN_HOME}/bin/Release"
    }
}

SOURCES += \
    assert.cpp \
    bounds.cpp \
    bspline.cpp \
    camera.cpp \
    catmullromspline.cpp \
    curve.cpp \
    exception.cpp \
    filesystem.cpp \
    font.cpp \
    framebufferobject.cpp \
    frustum.cpp \
    glcanvas.cpp \
    gpucapabilities.cpp \
    gpucapabilitieswindows.cpp \
    init.cpp \
    light.cpp \
    naturalcubicspline.cpp \
    painter.cpp \
    physmem.cpp \
    shadermanager.cpp \
    spline.cpp \
    stopwatch.cpp \
    tesselator.cpp \
    texturemanager.cpp \
    texture.cpp \
    texturereader.cpp \
    texturereaderdevil.cpp \
    texturereadertga.cpp \
    textureunit.cpp \
    tgt_gl.cpp \
    timer.cpp \
    event/eventhandler.cpp \
    event/eventlistener.cpp \
    event/keyevent.cpp \
    event/mouseevent.cpp \
    event/timeevent.cpp \
    logmanager.cpp \
    navigation/trackball.cpp \
    navigation/navigation.cpp

HEADERS += \
    assert.h \
    attribute.h \
    bounds.h \
    bspline.h \
    camera.h \
    catmullromspline.h \
    curve.h \
    exception.h \
    filesystem.h \
    font.h \
    framebufferobject.h \
    framecounter.h \
    frustum.h \
    glcanvas.h \
    glmath.h \
    gpucapabilities.h \
    gpucapabilitieswindows.h \
    init.h \
    light.h \
    line.h \
    manager.h \
    material.h \
    tgt_math.h \
    matrix.h \
    mouse.h \
    naturalcubicspline.h \
    painter.h \
    physmem.h \
    plane.h \
    quadric.h \
    quadtree.h \
    quaternion.h \
    renderable.h \
    singleton.h \
    spline.h \
    stopwatch.h \
    timer.h \
    tesselator.h \
    tgt_gl.h \
    types.h \
    texturemanager.h \
    texturereader.h \
    texturereaderdevil.h \
    texturereadertga.h \
    texture.h \
    textureunit.h \
    shadermanager.h \
    timer.h \
    vector.h \
    vertex.h \
    event/event.h \
    event/eventhandler.h \
    event/eventlistener.h \
    event/keyevent.h \
    event/mouseevent.h \
    event/timeevent.h \
    event/exitevent.h \
    logmanager.h \
    navigation/trackball.h \
    navigation/navigation.h 

####################################################
# Project file for the Voreen-Qt library
####################################################
TEMPLATE = lib
TARGET = voreen_qt
unix: VERSION = 1.0

CONFIG += qt thread
QT += opengl network

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
    	DEFINES += VRN_QT_BUILD_DLL
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
    
    contains(DEFINES, TGT_WITH_WMI) : LIBS += -lWbemUuid

    for(i, VRN_MODULES) : exists($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri) {
        include($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri)
    }
}

win32-msvc { # win 32 bit configuration
    LIBS += "$${VRN_HOME}/ext/glew/lib/win32/vs$${VS_VERSION_NUMBER}/$${WIN32_CONFIG_NAME}/glew32s.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/tgt.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/voreen_core.lib"
}
  
win64-msvc { # win 64 bit configuration
    LIBS += "$${VRN_HOME}/ext/glew/lib/win64/vs$${VS_VERSION_NUMBER}/$${WIN64_CONFIG_NAME}/glew64s.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN64_CONFIG_NAME}/tgt.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN64_CONFIG_NAME}/voreen_core.lib"
}

unix {
  LIBS += -lvoreen_core
  LIBS += -ltgt
}


contains(DEFINES, VRN_PRECOMPILE_HEADER) {
  PRECOMPILED_HEADER = pch_qt.h
  CONFIG += precompile_header
}
else {
  CONFIG -= precompile_header
}

# Destination directory for the library
macx {
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

# Include modules which are selected in local configuration. The entry
# 'foo' in VRN_MODULES must correspond to a subdir 'modules/foo' and a
# file 'foo_qt.pri' there.
for(i, VRN_MODULES) : exists($${VRN_MODULE_DIR}/$${i}/$${i}_qt.pri) {
  include($${VRN_MODULE_DIR}/$${i}/$${i}_qt.pri)
}

#
# Generate qt module registration file
#
!contains(DEFINES, VRN_NO_REGISTRATION_HEADER_GENERATION) {
    MODULE_REGISTRATION_FILE = "$${VRN_MODULE_DIR}/gen_moduleregistration_qt.h"
    message ("Generating 'modules/gen_moduleregistration_qt.h'...")

    REGIST_LINES += "$${LITERAL_HASH}include \"voreen/qt/voreenapplicationqt.h\""
    REGIST_LINES += "// module class headers"
    for(i, VRN_QT_MODULE_CLASS_HEADERS) {
        REGIST_LINES += "$${LITERAL_HASH}include \"modules/$${i}\""
    }

    REGIST_LINES += "// instantiate Qt module classes"
    REGIST_LINES += "namespace voreen \{"
    REGIST_LINES += "void addAllQtModules(VoreenApplicationQt* vappQt) \{"
    for(i, VRN_QT_MODULE_CLASSES) {
        REGIST_LINES += "(*vappQt).addQtModule(new $${i}());"
    }
    REGIST_LINES += "}}"

    # write lines to module registration file
    win32 {
        system(echo "// WARNING: This file is auto-generated!" > \"$${MODULE_REGISTRATION_FILE}\")
        for(i, REGIST_LINES) : system(echo $${i}  >> \"$${MODULE_REGISTRATION_FILE}\")
    }
    else:unix {
        system(echo "\"// WARNING: This file is auto-generated!\"" > \"$${MODULE_REGISTRATION_FILE}\")
        for(i, REGIST_LINES) : system(echo \'$${i}\'  >> \"$${MODULE_REGISTRATION_FILE}\")
    }
    else {
        warning("Unknown platform: registration file not generated")
        DEFINES += VRN_NO_REGISTRATION_HEADER_GENERATION
    }

    # add generated file to headers if exists, otherwise use static registration file as fallback
    exists($${MODULE_REGISTRATION_FILE}) {
        HEADERS += "$${VRN_MODULE_DIR}/gen_moduleregistration_qt.h"
    }
    else {
        warning("$${MODULE_REGISTRATION_FILE} not found. Using static fallback!")
        DEFINES += VRN_NO_REGISTRATION_HEADER_GENERATION
    }
}

contains(DEFINES, VRN_NO_REGISTRATION_HEADER_GENERATION) {
    message("Using static module registration file 'modules/moduleregistration_qt.h'")
    HEADERS += "$${VRN_MODULE_DIR}/moduleregistration_qt.h"
}

# add Qt module class source/headers
for(i, VRN_QT_MODULE_CLASS_HEADERS) : HEADERS += $${VRN_MODULE_DIR}/$${i}
for(i, VRN_QT_MODULE_CLASS_SOURCES) : SOURCES += $${VRN_MODULE_DIR}/$${i}


# include resource files
RESOURCES = $${VRN_HOME}/resource/qt/vrn_qt.qrc

# please insert new files in alphabetical order!
##############
# Sources
##############
SOURCES += \
    aboutbox.cpp \
    coremoduleqt.cpp \
    helpbrowser.cpp \
    progressdialog.cpp \
    versionqt.cpp \
    voreenapplicationqt.cpp \
    voreenmoduleqt.cpp
SOURCES += \
    widgets/choicelistcombobox.cpp \
    widgets/clickablelabel.cpp \
    widgets/codeedit.cpp \
    widgets/consoleplugin.cpp \
    widgets/customlabel.cpp \
    widgets/documentationwidget.cpp \
    widgets/enterexitpushbutton.cpp \
    widgets/eventpropertywidget.cpp \
    widgets/expandableheaderbutton.cpp \
    widgets/glslhighlighter.cpp \
    widgets/inputmappingdialog.cpp \
    widgets/keydetectorwidget.cpp \
    widgets/labelingwidgetqt.cpp \
    widgets/lightwidget.cpp \
    widgets/lineeditresetwidget.cpp \
    widgets/performancerecordwidget.cpp \
    widgets/processorlistwidget.cpp \
    widgets/propertylistwidget.cpp \
    widgets/propertyselectiondialog.cpp \
    widgets/propertystatewidget.cpp \
    widgets/rawvolumewidget.cpp \
    widgets/rendertargetviewer.cpp \
    widgets/shaderplugin.cpp \
    widgets/sliderspinboxwidget.cpp \
    widgets/snapshotwidget.cpp \
    widgets/stringselectiondialog.cpp \
    widgets/syntaxhighlighter.cpp \
    widgets/volumecontainerwidget.cpp \
    widgets/volumeiohelper.cpp \
    widgets/volumeloadbutton.cpp \
    widgets/volumelistingdialog.cpp \
    widgets/volumereaderselectiondialog.cpp \
    widgets/volumeviewhelper.cpp \
    widgets/voreentoolwindow.cpp \
    widgets/animation/animationeditor.cpp \
    widgets/animation/animationexportwidget.cpp \
    widgets/animation/currentframegraphicsitem.cpp \
    widgets/animation/keyframegraphicsitem.cpp \
    widgets/animation/nodechainwidget.cpp \
    widgets/animation/overviewwidget.cpp \
    widgets/animation/processortimelinewidget.cpp \
    widgets/animation/propertytimelineview.cpp \
    widgets/animation/propertytimelinewidget.cpp \
    widgets/animation/templatepropertytimelinewidget.cpp \
    widgets/animation/timelinewidget.cpp \
    widgets/processor/canvasrendererwidget.cpp \
    widgets/processor/coreprocessorwidgetfactory.cpp \
    widgets/processor/qprocessorwidget.cpp \
    widgets/property/boolpropertywidget.cpp \
    widgets/property/buttonpropertywidget.cpp \
    widgets/property/camerapropertywidget.cpp \
    widgets/property/camerawidget.cpp \
    widgets/property/colorpropertywidget.cpp \
    widgets/property/corepropertywidgetfactory.cpp \
    widgets/property/filedialogpropertywidget.cpp \
    widgets/property/floatmat2propertywidget.cpp \
    widgets/property/floatmat3propertywidget.cpp \
    widgets/property/floatmat4propertywidget.cpp \
    widgets/property/floatpropertywidget.cpp \
    widgets/property/floatvec2propertywidget.cpp \
    widgets/property/floatvec3propertywidget.cpp \
    widgets/property/floatvec4propertywidget.cpp \
    widgets/property/fontpropertywidget.cpp \
    widgets/property/intpropertywidget.cpp \
    widgets/property/intvec2propertywidget.cpp \
    widgets/property/intvec3propertywidget.cpp \
    widgets/property/intvec4propertywidget.cpp \
    widgets/property/lightpropertywidget.cpp \
    widgets/property/matrixpropertywidget.cpp \
    widgets/property/optionpropertywidget.cpp \
    widgets/property/processorpropertieswidget.cpp \
    widgets/property/propertyvectorwidget.cpp \
    widgets/property/qpropertywidget.cpp \
    widgets/property/qpropertywidgetwitheditorwindow.cpp \
    widgets/property/shaderpropertywidget.cpp \
    widgets/property/stringexpressionpropertywidget.cpp \
    widgets/property/stringpropertywidget.cpp \
    widgets/property/grouppropertywidget.cpp \
    widgets/property/transfuncpropertywidget.cpp \
    widgets/property/vecpropertywidget.cpp \
    widgets/property/voxeltypepropertywidget.cpp \
    widgets/property/volumecollectionpropertywidget.cpp \
    widgets/property/volumehandlepropertywidget.cpp \
    widgets/transfunc/colorluminancepicker.cpp \
    widgets/transfunc/colorpicker.cpp \
    widgets/transfunc/doubleslider.cpp \
    widgets/transfunc/histogrampainter.cpp \
    widgets/transfunc/transfunceditor.cpp \
    widgets/transfunc/transfunceditorintensity.cpp \
    widgets/transfunc/transfunceditorintensitygradient.cpp \
    widgets/transfunc/transfunceditorintensitypet.cpp \
    widgets/transfunc/transfunceditorintensityramp.cpp \
    widgets/transfunc/transfuncintensitygradientpainter.cpp \
    widgets/transfunc/transfuncmappingcanvas.cpp \
    widgets/transfunc/transfuncmappingcanvasramp.cpp \
    widgets/transfunc/transfuncplugin.cpp \
    widgets/transfunc/transfunctexturepainter.cpp

SOURCES += \
    ../../ext/tgt/qt/qtcanvas.cpp \
    ../../ext/tgt/qt/qttimer.cpp

#SHADER_SOURCES += \

SHADER_SOURCES_RENDERTARGETVIEWER += \
    glsl/rendertargetviewer/color.frag \
    glsl/rendertargetviewer/inversecolor.frag

visual_studio: !win32-msvc2010: SOURCES += $$SHADER_SOURCES $$SHADER_SOURCES_RENDERTARGETVIEWER

##############
# Headers
##############

HEADERS += \
    ../../include/voreen/qt/aboutbox.h \
    ../../include/voreen/qt/coremoduleqt.h \
    ../../include/voreen/qt/helpbrowser.h \
    ../../include/voreen/qt/progressdialog.h \
    ../../include/voreen/qt/versionqt.h \
    ../../include/voreen/qt/voreenapplicationqt.h \
    ../../include/voreen/qt/voreenmoduleqt.h \
    ../../include/voreen/qt/voreenqtdefine.h \
    ../../include/voreen/qt/widgets/choicelistcombobox.h \
    ../../include/voreen/qt/widgets/clickablelabel.h \
    ../../include/voreen/qt/widgets/codeedit.h \
    ../../include/voreen/qt/widgets/consoleplugin.h \
    ../../include/voreen/qt/widgets/customlabel.h \
    ../../include/voreen/qt/widgets/documentationwidget.h \
    ../../include/voreen/qt/widgets/enterexitpushbutton.h \
    ../../include/voreen/qt/widgets/eventpropertywidget.h \
    ../../include/voreen/qt/widgets/expandableheaderbutton.h \
    ../../include/voreen/qt/widgets/glslhighlighter.h \
    ../../include/voreen/qt/widgets/inputmappingdialog.h \
    ../../include/voreen/qt/widgets/keydetectorwidget.h \
    ../../include/voreen/qt/widgets/labelingwidgetqt.h \
    ../../include/voreen/qt/widgets/lightwidget.h \
    ../../include/voreen/qt/widgets/lineeditresetwidget.h \
    ../../include/voreen/qt/widgets/performancerecordwidget.h \
    ../../include/voreen/qt/widgets/processorlistwidget.h \
    ../../include/voreen/qt/widgets/propertylistwidget.h \
    ../../include/voreen/qt/widgets/propertyselectiondialog.h \
    ../../include/voreen/qt/widgets/propertystatewidget.h \
    ../../include/voreen/qt/widgets/rawvolumewidget.h \
    ../../include/voreen/qt/widgets/rendertargetviewer.h \
    ../../include/voreen/qt/widgets/shaderplugin.h \
    ../../include/voreen/qt/widgets/sliderspinboxwidget.h \
    ../../include/voreen/qt/widgets/snapshotwidget.h \
    ../../include/voreen/qt/widgets/stringselectiondialog.h \
    ../../include/voreen/qt/widgets/syntaxhighlighter.h \
    ../../include/voreen/qt/widgets/volumecontainerwidget.h \
    ../../include/voreen/qt/widgets/volumeiohelper.h \
    ../../include/voreen/qt/widgets/volumeloadbutton.h \
    ../../include/voreen/qt/widgets/volumelistingdialog.h \
    ../../include/voreen/qt/widgets/volumereaderselectiondialog.h \
    ../../include/voreen/qt/widgets/volumeviewhelper.h \
    ../../include/voreen/qt/widgets/voreentoolwindow.h \
    ../../include/voreen/qt/widgets/animation/animationeditor.h \
    ../../include/voreen/qt/widgets/animation/animationexportwidget.h \
    ../../include/voreen/qt/widgets/animation/currentframegraphicsitem.h \
    ../../include/voreen/qt/widgets/animation/keyframegraphicsitem.h \
    ../../include/voreen/qt/widgets/animation/nodechainwidget.h \
    ../../include/voreen/qt/widgets/animation/overviewwidget.h \
    ../../include/voreen/qt/widgets/animation/processortimelinewidget.h \
    ../../include/voreen/qt/widgets/animation/propertytimelineview.h \
    ../../include/voreen/qt/widgets/animation/propertytimelinewidget.h \
    ../../include/voreen/qt/widgets/animation/templatepropertytimelinewidget.h \
    ../../include/voreen/qt/widgets/animation/timelinewidget.h \
    ../../include/voreen/qt/widgets/processor/canvasrendererwidget.h \
    ../../include/voreen/qt/widgets/processor/coreprocessorwidgetfactory.h \
    ../../include/voreen/qt/widgets/processor/qprocessorwidget.h \
    ../../include/voreen/qt/widgets/property/boolpropertywidget.h \
    ../../include/voreen/qt/widgets/property/buttonpropertywidget.h \
    ../../include/voreen/qt/widgets/property/camerapropertywidget.h \
    ../../include/voreen/qt/widgets/property/camerawidget.h \
    ../../include/voreen/qt/widgets/property/colorpropertywidget.h \
    ../../include/voreen/qt/widgets/property/corepropertywidgetfactory.h \
    ../../include/voreen/qt/widgets/property/filedialogpropertywidget.h \
    ../../include/voreen/qt/widgets/property/floatmat2propertywidget.h \
    ../../include/voreen/qt/widgets/property/floatmat3propertywidget.h \
    ../../include/voreen/qt/widgets/property/floatmat4propertywidget.h \
    ../../include/voreen/qt/widgets/property/floatpropertywidget.h \
    ../../include/voreen/qt/widgets/property/floatvec2propertywidget.h \
    ../../include/voreen/qt/widgets/property/floatvec3propertywidget.h \
    ../../include/voreen/qt/widgets/property/floatvec4propertywidget.h \
    ../../include/voreen/qt/widgets/property/fontpropertywidget.h \
    ../../include/voreen/qt/widgets/property/intpropertywidget.h \
    ../../include/voreen/qt/widgets/property/intvec2propertywidget.h \
    ../../include/voreen/qt/widgets/property/intvec3propertywidget.h \
    ../../include/voreen/qt/widgets/property/intvec4propertywidget.h \
    ../../include/voreen/qt/widgets/property/lightpropertywidget.h \
    ../../include/voreen/qt/widgets/property/matrixpropertywidget.h \
    ../../include/voreen/qt/widgets/property/optionpropertywidget.h \
    ../../include/voreen/qt/widgets/property/processorpropertieswidget.h \
    ../../include/voreen/qt/widgets/property/propertyvectorwidget.h \
    ../../include/voreen/qt/widgets/property/qpropertywidget.h \
    ../../include/voreen/qt/widgets/property/qpropertywidgetwitheditorwindow.h \
    ../../include/voreen/qt/widgets/property/shaderpropertywidget.h \
    ../../include/voreen/qt/widgets/property/stringexpressionpropertywidget.h \
    ../../include/voreen/qt/widgets/property/stringpropertywidget.h \
    ../../include/voreen/qt/widgets/property/grouppropertywidget.h \
    ../../include/voreen/qt/widgets/property/transfuncpropertywidget.h \
    ../../include/voreen/qt/widgets/property/vecpropertywidget.h \
    ../../include/voreen/qt/widgets/property/voxeltypepropertywidget.h \
    ../../include/voreen/qt/widgets/property/volumecollectionpropertywidget.h \
    ../../include/voreen/qt/widgets/property/volumehandlepropertywidget.h \
    ../../include/voreen/qt/widgets/transfunc/colorluminancepicker.h \
    ../../include/voreen/qt/widgets/transfunc/colorpicker.h \
    ../../include/voreen/qt/widgets/transfunc/doubleslider.h \
    ../../include/voreen/qt/widgets/transfunc/histogrampainter.h \
    ../../include/voreen/qt/widgets/transfunc/transfunceditor.h \
    ../../include/voreen/qt/widgets/transfunc/transfunceditorintensity.h \
    ../../include/voreen/qt/widgets/transfunc/transfunceditorintensitygradient.h \
    ../../include/voreen/qt/widgets/transfunc/transfunceditorintensitypet.h \
    ../../include/voreen/qt/widgets/transfunc/transfunceditorintensityramp.h \
    ../../include/voreen/qt/widgets/transfunc/transfuncintensitygradientpainter.h \
    ../../include/voreen/qt/widgets/transfunc/transfuncmappingcanvas.h \
    ../../include/voreen/qt/widgets/transfunc/transfuncmappingcanvasramp.h \
    ../../include/voreen/qt/widgets/transfunc/transfuncplugin.h \
    ../../include/voreen/qt/widgets/transfunc/transfunctexturepainter.h


HEADERS += \
    ../../ext/tgt/qt/qtcanvas.h \
    ../../ext/tgt/qt/qttimer.h

FORMS += aboutbox.ui


# this must come after all SOURCES, HEADERS and FORMS have been added
contains(DEFINES, VRN_WITH_SVNVERSION) : revtarget.depends = $$SOURCES $$HEADERS $$FORMS

# installation
unix {
  !isEmpty(INSTALL_PREFIX) {
    target.path = $$INSTALLPATH_LIB

    shaders.path = $$INSTALLPATH_SHARE/shaders
    shaders.files += $$SHADER_SOURCES
    shaders_rtv.path = $$INSTALLPATH_SHARE/shaders/rendertargetviewer
    shaders_rtv.files += $$SHADER_SOURCES_RENDERTARGETVIEWER

    INSTALLS += target shaders shaders_rtv
  }
}


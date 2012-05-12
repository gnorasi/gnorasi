TARGET = voreenve
TEMPLATE = app
LANGUAGE = C++

CONFIG += qt
QT += opengl network xml

# include config
!exists(../../config.txt) {
  error("config.txt not found! copy config-default.txt to config.txt and edit!")
}
include(../../config.txt)

# Include common configuration
include(../../commonconf.pri)

# Set output directory of the executable
VRN_APP_DIRECTORY = "$${VRN_HOME}/bin"

# Include generic app configuration
include(../voreenapp.pri)

win32 {
  contains(DEFINES, VRN_DEBUG) {
    # link against console subsystem for debug builds (opens console)
    CONFIG += console
  }
}

RESOURCES = "$${VRN_HOME}/resource/vrn_share/voreenve.qrc"
RESOURCES += "$${VRN_HOME}/resource/qt/vrn_qt.qrc"

# Include modules which are selected in local configuration. The entry
# 'foo' in VRN_MODULES must correspond to a subdir 'modules/foo' and a
# file 'foo_ve.pri' there.
for(i, VRN_MODULES) : exists($${VRN_MODULE_DIR}/$${i}/$${i}_ve.pri) {
    include($${VRN_MODULE_DIR}/$${i}/$${i}_ve.pri)
}

#
# Generate VoreenVE module registration file
#
!contains(DEFINES, VRN_NO_REGISTRATION_HEADER_GENERATION) {
    MODULE_REGISTRATION_FILE = "$${VRN_MODULE_DIR}/gen_moduleregistration_ve.h"
    message ("Generating 'modules/gen_moduleregistration_ve.h'...")

    REGIST_LINES += "$${LITERAL_HASH}include \"apps/voreenve/voreenveapplication.h\""
    REGIST_LINES += "// module class headers"
    for(i, VRN_VOREENVE_MODULE_CLASS_HEADERS) {
        REGIST_LINES += "$${LITERAL_HASH}include \"modules/$${i}\""
    }

    REGIST_LINES += "// instantiate VoreenVE module classes"
    REGIST_LINES += "namespace voreen \{"
    REGIST_LINES += "void addAllVEModules(VoreenVEApplication* vappVE) \{"
    for(i, VRN_VOREENVE_MODULE_CLASSES) {
        REGIST_LINES += "(*vappVE).addVEModule(new $${i}());"
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
        HEADERS += "$${VRN_MODULE_DIR}/gen_moduleregistration_ve.h"
    }
    else {
        warning("$${MODULE_REGISTRATION_FILE} not found. Using static fallback!")
        DEFINES += VRN_NO_REGISTRATION_HEADER_GENERATION
    }
}

contains(DEFINES, VRN_NO_REGISTRATION_HEADER_GENERATION) {
    message("Using static module registration file 'modules/moduleregistration_ve.h'")
    HEADERS += "$${VRN_MODULE_DIR}/moduleregistration_ve.h"
}

# add Qt module class source/headers
for(i, VRN_VOREENVE_MODULE_CLASS_HEADERS) : HEADERS += $${VRN_MODULE_DIR}/$${i}
for(i, VRN_VOREENVE_MODULE_CLASS_SOURCES) : SOURCES += $${VRN_MODULE_DIR}/$${i}


SOURCES += \
    main.cpp \
    settingsdialog.cpp \
    startupscreen.cpp \
    templateworkspacefetcher.cpp \
    tutorialvideofetcher.cpp \
    voreenmainwindow.cpp \
    voreenmoduleve.cpp \
    voreenveapplication.cpp \
    voreenveplugin.cpp \
    voreenvisualization.cpp \
    networkeditor/annotationclosebutton.cpp \
    networkeditor/aggregationgraphicsitem.cpp \
    networkeditor/annotationfencegraphicsitem.cpp \
    networkeditor/annotationgraphicsitem.cpp \
    networkeditor/arrowgraphicsitem.cpp \
    networkeditor/arrowheadselectiongraphicsitem.cpp \
    networkeditor/linkarrowgraphicsitem.cpp \
    networkeditor/linkarrowgraphicsitemstub.cpp \
    networkeditor/linkdialogarrowgraphicsitem.cpp \
    networkeditor/linkdialoggraphicsscene.cpp \
    networkeditor/linkdialoggraphicsview.cpp \
    networkeditor/linkdialogprocessorgraphicsitem.cpp \
    networkeditor/linkdialogpropertygraphicsitem.cpp \
    networkeditor/networkeditor.cpp \
    networkeditor/openpropertylistbutton.cpp \
    networkeditor/portarrowgraphicsitem.cpp \
    networkeditor/portgraphicsitem.cpp \
    networkeditor/processorgraphicsitem.cpp \
    networkeditor/progressbargraphicsitem.cpp \
    networkeditor/propertygraphicsitem.cpp \
    networkeditor/propertylinkdialog.cpp \
    networkeditor/propertylistgraphicsitem.cpp \
    networkeditor/rootgraphicsitem.cpp \
    networkeditor/textgraphicsitem.cpp \
    networkeditor/widgetindicatorbutton.cpp

HEADERS += \
    settingsdialog.h \
    startupscreen.h \
    templateworkspacefetcher.h \
    tutorialvideofetcher.h \
    voreenmainwindow.h \
    voreenmoduleve.h \
    voreenveapplication.h \
    voreenveplugin.h \
    voreenvisualization.h \
    networkeditor/annotationclosebutton.h \
    networkeditor/aggregationgraphicsitem.h \
    networkeditor/annotationfencegraphicsitem.h \
    networkeditor/annotationgraphicsitem.h \
    networkeditor/arrowgraphicsitem.h \
    networkeditor/arrowheadselectiongraphicsitem.h \
    networkeditor/hastooltip.h \
    networkeditor/linkarrowgraphicsitem.h \
    networkeditor/linkarrowgraphicsitemstub.h \
    networkeditor/linkdialogarrowgraphicsitem.h \
    networkeditor/linkdialoggraphicsscene.h \
    networkeditor/linkdialoggraphicsview.h \
    networkeditor/linkdialogprocessorgraphicsitem.h \
    networkeditor/linkdialogpropertygraphicsitem.h \
    networkeditor/networkeditor.h \
    networkeditor/networkeditor_common.h \
    networkeditor/openpropertylistbutton.h \
    networkeditor/portarrowgraphicsitem.h \
    networkeditor/portgraphicsitem.h \
    networkeditor/processorgraphicsitem.h \
    networkeditor/progressbargraphicsitem.h \
    networkeditor/propertygraphicsitem.h \
    networkeditor/propertylinkdialog.h \
    networkeditor/propertylistgraphicsitem.h \
    networkeditor/rootgraphicsitem.h \
    networkeditor/textgraphicsitem.h \
    networkeditor/widgetindicatorbutton.h
    
win32 {
    # icon description file for windows-exe
    RC_FILE = "../../resource/vrn_share/icons/winicon.rc"
}

macx {  
  # icon for the application bundle
  ICON = "$${VRN_HOME}/resource/vrn_share/icons/icon.icns"

  # installer target for creating distribution DMG
  contains(DEFINES, VRN_DISTRIBUTION) {
    deploy.path = ./
    deploy.extra = ../../tools/macdeployment/macdeployment.sh
    INSTALLS += deploy
  }  
}

unix {
    # Prevent a namespace clash
   DEFINES += QT_CLEAN_NAMESPACE
   UI_DIR = .ui
}


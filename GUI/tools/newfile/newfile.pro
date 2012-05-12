TARGET = newfile
TEMPLATE = app
LANGUAGE = C++

CONFIG += qt

# check qmake version
QMAKE_VERS = $$[QMAKE_VERSION]
QMAKE_VERSION_CHECK = $$find(QMAKE_VERS, "^[234]\\.")
isEmpty(QMAKE_VERSION_CHECK) {
   error("Your qmake version '$$QMAKE_VERS' is too old, qmake from Qt 4 is required!")
}

HEADERS += \
    functions.h \
    mainwindow.h \
    newfilepairwidget.h \
	newfilesinglewidget.h \
    newmodulewidget.h \
    newprocessorwidget.h \
	newwidget.h

SOURCES += \
    functions.cpp \
    main.cpp \
    mainwindow.cpp \
    newfilepairwidget.cpp \
	newfilesinglewidget.cpp \
    newmodulewidget.cpp \
    newprocessorwidget.cpp \
	newwidget.cpp
    
win32 {
    # icon description file for windows-exe
    RC_FILE = "../../resource/vrn_share/icons/winicon.rc"
}

unix {
    # Prevent a namespace clash
   DEFINES += QT_CLEAN_NAMESPACE
}

### Local Variables:
### mode:conf-unix
### End:

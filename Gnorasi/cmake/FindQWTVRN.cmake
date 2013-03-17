# Try to find QWT library and include path. Once done this will define:
# QWT_FOUND
# QWT_DEFINITIONS
# QWT_INCLUDE_DIRS
# QWT_LIBRARIES (containing both debug and release libraries on win32)
# win32: QWT_DEBUG_LIBRARIES, QWT_RELEASE_LIBRARIES, QWT_DEBUG_DLLS, QWT_RELEASE_DLLS

IF (WIN32)
    SET(QWT_DIR "${VRN_HOME}/ext/qwt" CACHE PATH "If qwt is not found, set this path")
    
#    SET(QWT_DEFINITIONS "-DQWT_ALL_NO_LIB")
    SET(QWT_DEFINITIONS "-DQWT_DLL")

    SET(QWT_INCLUDE_DIRS "${QWT_DIR}/include")

    # set debug and release libraries
    IF(VRN_WIN32)
        IF(VRN_MSVC2008)
            SET(QWT_LIB_DIR "${QWT_DIR}/lib/vs2008")
        ELSE()
            SET(QWT_LIB_DIR "${QWT_DIR}/lib/win32")
        ENDIF()
    ELSEIF(VRN_WIN64)
        SET(QWT_LIB_DIR "${QWT_DIR}/lib/win64")
    ELSE(VRN_WIN32)
        MESSAGE(FATAL_ERROR "Neither VRN_WIN32 nor VRN_WIN64 defined!")
    ENDIF(VRN_WIN32)

    LIST(APPEND QWT_DEBUG_LIBRARIES   "${QWT_LIB_DIR}/debug/qwtd.lib")
    LIST(APPEND QWT_DEBUG_DLLS        "${QWT_LIB_DIR}/debug/qwtd.dll")
    LIST(APPEND QWT_DEBUG_LIBRARIES   "${QWT_LIB_DIR}/debug/qwtmathmld.lib")
    LIST(APPEND QWT_DEBUG_DLLS        "${QWT_LIB_DIR}/debug/qwtmathmld.dll")
    LIST(APPEND QWT_RELEASE_LIBRARIES "${QWT_LIB_DIR}/release/qwt.lib")
    LIST(APPEND QWT_RELEASE_DLLS      "${QWT_LIB_DIR}/release/qwt.dll")
    LIST(APPEND QWT_RELEASE_LIBRARIES   "${QWT_LIB_DIR}/release/qwtmathml.lib")
    LIST(APPEND QWT_RELEASE_DLLS        "${QWT_LIB_DIR}/release/qwtmathml.dll")
    
    FOREACH(lib ${QWT_DEBUG_LIBRARIES})
        LIST(APPEND QWT_LIBRARIES debug ${lib})
    ENDFOREACH()
    FOREACH(lib ${QWT_RELEASE_LIBRARIES})
        LIST(APPEND QWT_LIBRARIES optimized ${lib})
    ENDFOREACH()
   
    IF(QWT_DEBUG_LIBRARIES AND QWT_RELEASE_LIBRARIES)
        SET(QWT_FOUND TRUE)
    ELSE()
        SET(QWT_FOUND FALSE)
    ENDIF()

ELSE(WIN32)
    FIND_PATH(QWT_INCLUDE_DIRS qwt.h 
	/usr/include/qwt
	/usr/include/qwt6
	/usr/local/include/qwt
	/usr/local/include/qwt6
	/sw/include/qwt
	/sw/include/qwt6
	)
    FIND_LIBRARY(QWT_LIBRARIES 
	NAMES libqwt.so
	PATHS /usr/lib
	      /usr/lib64
	      /usr/local/lib
	      /usr/local/lib64
	      /sw/lib
	      /sw/lib64
	)

    IF(QWT_INCLUDE_DIRS AND QWT_LIBRARIES) 
      SET(QWT_FOUND TRUE)
    ELSE(QWT_INCLUDE_DIRS AND QWT_LIBRARIES) 
      SET(QWT_FOUND FALSE)
    ENDIF(QWT_INCLUDE_DIRS AND QWT_LIBRARIES) 
    
    #FIND_PACKAGE(QWT 6.40.0 REQUIRED date_time prg_exec_monitor program_options regex thread unit_test_framework)
ENDIF(WIN32)

MARK_AS_ADVANCED(QWT_DIR QWT_INCLUDE_DIRS)

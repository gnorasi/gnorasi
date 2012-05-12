# link against FFmpeg library

win32 {
    isEmpty(FFMPEG_DIR) : warn('FFMPEG_DIR not set')
    
    LIBS += "$${FFMPEG_DIR}/lib/win32/avcodec.lib"
    LIBS += "$${FFMPEG_DIR}/lib/win32/avdevice.lib"
    LIBS += "$${FFMPEG_DIR}/lib/win32/avformat.lib"
    LIBS += "$${FFMPEG_DIR}/lib/win32/avutil.lib"
    LIBS += "$${FFMPEG_DIR}/lib/win32/swscale.lib"
    
    VRN_MODULE_DLLS += \
        $${FFMPEG_DIR}\\lib\\win32\\avcodec-52.dll \
        $${FFMPEG_DIR}\\lib\\win32\\avdevice-52.dll \
        $${FFMPEG_DIR}\\lib\\win32\\avformat-52.dll \
        $${FFMPEG_DIR}\\lib\\win32\\avutil-50.dll \
        $${FFMPEG_DIR}\\lib\\win32\\swscale-0.dll
}

unix {
    # It is important that this comes after linking the voreen_* libs.
    LIBS += -lbz2 -lavformat -lavcodec -lavutil -lswscale
}

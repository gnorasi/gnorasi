# libraries to link

win32 {  
  LIBS += "$${VRN_MODULE_DIR}/hpmc/ext/hpmc/win32/$${WIN32_CONFIG_NAME}/hpmc.lib"
}

unix {  
  LIBS += -lhpmc
}  

### Local Variables:
### mode:conf-unix
### End:

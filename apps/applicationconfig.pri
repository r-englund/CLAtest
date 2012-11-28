isEmpty(IVW_APPLICATION_PATH) {
    IVW_APPLICATION_PATH = "$${IVW_ROOT}/bin"
}

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = "$${IVW_APPLICATION_PATH}/debug"
    } else {
        DESTDIR = "$${IVW_APPLICATION_PATH}/release"
    }

    win32-msvc {
		LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/ticpp.lib"
        LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/inviwo-core.lib"
        LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/inviwo-qtwidgets.lib"
        LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/inviwo-editor.lib"
        LIBS += /NODEFAULTLIB:libc.lib
    }
    
    win64-msvc {
		LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/ticpp.lib"	
        LIBS += "$${IVW_ROOT}/bin/$${WIN64_CONFIG_NAME}/inviwo-core.lib"
        LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/inviwo-qtwidgets.lib"
        LIBS += "$${IVW_ROOT}/bin/$${WIN32_CONFIG_NAME}/inviwo-editor.lib"
        LIBS += /NODEFAULTLIB:libc.lib
    }
}

unix {
  DESTDIR = "$${IVW_APPLICATION_PATH}"
  
  LIBS += -lticpp
  LIBS += -linviwo-core
  qt : LIBS += -linviwo-qtwidgets
  qt : LIBS += -linviwo-editor
}

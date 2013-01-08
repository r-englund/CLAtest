###########################################
# general qmake project configuration     #
###########################################


###########################################
### Set Visual Studio configurations    ###
###########################################
MSC_VER = $$find(QMAKE_COMPILER_DEFINES, "_MSC_VER")
!isEmpty(MSC_VER) {
    !contains(QMAKE_HOST.arch, x86_64) {
        CONFIG += win32-msvc
    } else {
        CONFIG += win64-msvc
    }

	contains(TEMPLATE, "vcapp") {
		CONFIG += visual_studio
	} else:contains(TEMPLATE, "vclib") {
		CONFIG += visual_studio
	}
  
    visual_studio {
        win32-msvc: WIN32_CONFIG_NAME = $(ConfigurationName)
        win64-msvc: WIN64_CONFIG_NAME = $(ConfigurationName)
    }
}
#                                         #
###########################################



###########################################
### Set include and dependency paths    ###
###########################################
INCLUDEPATH += \
   "$${IVW_ROOT}" \
   "$${IVW_ROOT}/include" \
   "$${IVW_ROOT}/ext"

DEPENDPATH += \
   "$${IVW_ROOT}/include" \
   "$${IVW_ROOT}/ext"
#                                         #
###########################################



###########################################
### Incorporate files from modules      ###
###########################################
IVW_MODULE_PATH = $${IVW_ROOT}/modules

IVW_MODULE_LIST += default
contains(IVW_MODULE_LIST, default) {
    include(modules/default.pri)
}
IVW_MODULE_LIST -= default
IVW_MODULE_LIST = $$unique(IVW_MODULE_LIST)

for(i, IVW_MODULE_LIST) : {
    exists($${IVW_MODULE_PATH}/$${i}/$${i}_depends.pri) {
        include($${IVW_MODULE_PATH}/$${i}/$${i}_depends.pri)
    }
}
IVW_MODULE_LIST = $$unique(IVW_MODULE_LIST)

for(i, IVW_MODULE_LIST) : include($${IVW_MODULE_PATH}/$${i}/$${i}_common.pri)
#                                         #
###########################################



###########################################
### Specify build directories           ###
###########################################
win32 {
    DEFINES += _WINDOWS

    win32-msvc {
        QMAKE_LFLAGS += /MACHINE:X86
    }
    win64-msvc {
        QMAKE_LFLAGS += /MACHINE:X64
    }
    
    !win32-msvc2010: CONFIG -= flat    
  
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = debug
        DESTDIR = debug
    } else {
        OBJECTS_DIR = release
        DESTDIR = release
    }
    
    QMAKE_CXXFLAGS_DEBUG += /Z7
    QMAKE_LFLAGS += /ignore:4217
}

win32 {
    DEFINES += TIXML_USE_TICPP
    DEFINES += _CRT_SECURE_NO_DEPRECATE
}

#                                         #
###########################################



###########################################
### Add libraries                       ###
###########################################
win32 {
    win32-msvc {
        LIBS += $${IVW_ROOT}/modules/opengl/ext/glew/lib/win32/debug/glew32s.lib
    }
    
    win64-msvc {
        LIBS += $${IVW_ROOT}/modules/opengl/ext/glew/lib/win64/debug/glew64s.lib
    }
}
#                                         #
###########################################
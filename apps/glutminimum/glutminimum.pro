###########################################
# core qmake project configuration        #
###########################################
TARGET = glutminimum
TEMPLATE = app

###########################################
# Include global project configurations   #
###########################################
!exists(../../config.txt) {
    error(ERROR: "config.txt" cannot be found! Please rename and modify "config-default.txt" within the Inviwo root directory.")
}
include(../../config.txt)
include(../../globalconfig.pri)
include(../applicationconfig.pri)
#                                         #
###########################################



###########################################
# Application specific configuration      #
###########################################
CONFIG += console
CONFIG -= qt
#                                         #
###########################################



###########################################
# Add include path and libraries          #
###########################################
win32 {
    DEFINES += GLUT_NO_LIB_PRAGMA

    INCLUDEPATH += $${IVW_ROOT}/modules/glut/ext/glut/include
       
    win32-msvc {
        LIBS += $${IVW_ROOT}/modules/glut/ext/glut/lib/win32/glut32.lib
    }
    
    win64-msvc {
        LIBS += $${IVW_ROOT}/modules/glut/ext/glut/lib/win64/glut64.lib
    }
}
#                                         #
###########################################



###########################################
# Add source and header files             #
###########################################
SOURCES += glutminimum.cpp
#                                         #
###########################################

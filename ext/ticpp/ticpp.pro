###########################################
# core qmake project configuration        #
###########################################
TARGET = ticpp
TEMPLATE = lib

###########################################
# Include global project configurations   #
###########################################
!exists(../../config.txt) {
    error(ERROR: "config.txt" cannot be found! Please rename and modify "config-default.txt" within the Inviwo root directory.")
}
include(../../config.txt)
include(../../globalconfig.pri)
#                                         #
###########################################



###########################################
# Set win32 specifics                     #
###########################################
win32 {
    CONFIG += static

    CONFIG(debug, debug|release) {
        DESTDIR = "$${IVW_ROOT}/bin/debug"
    } else {
        DESTDIR = "$${IVW_ROOT}/bin/release"
    }
    
    # allow to build multithreaded executable
    LIBS += /NODEFAULTLIB:libc.lib    
}
#                                         #
###########################################

HEADERS += \
    ../../ext/ticpp/ticpp.h \
    ../../ext/ticpp/ticpprc.h \
    ../../ext/ticpp/tinystr.h \
    ../../ext/ticpp/tinyxml.h
	
SOURCES += \
    ../../ext/ticpp/ticpp.cpp \
    ../../ext/ticpp/tinystr.cpp \
    ../../ext/ticpp/tinyxml.cpp	\
    ../../ext/ticpp/tinyxmlerror.cpp \
    ../../ext/ticpp/tinyxmlparser.cpp
	
HEADERS = $$unique(HEADERS)
SOURCES = $$unique(SOURCES)
#                                         #
###########################################

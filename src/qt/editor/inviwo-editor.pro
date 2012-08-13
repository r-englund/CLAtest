###########################################
# core qmake project configuration        #
###########################################
TARGET = inviwo-editor
TEMPLATE = lib

###########################################
# Include global project configurations   #
###########################################
!exists(../../../config.txt) {
    error(ERROR: "config.txt" cannot be found! Please rename and modify "config-default.txt" within the Inviwo root directory.")
}
include(../../../config.txt)
include(../../../globalconfig.pri)
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



###########################################
# Add header and source files             #
###########################################
HEADERS += \
    ../../../include/inviwo/qt/editor/consolewidget.h \
    ../../../include/inviwo/qt/editor/connectiongraphicsitem.h \
    ../../../include/inviwo/qt/editor/networkeditor.h \
    ../../../include/inviwo/qt/editor/networkeditorview.h \
    ../../../include/inviwo/qt/editor/processorlistwidget.h \
    ../../../include/inviwo/qt/editor/processorgraphicsitem.h \
    ../../../include/inviwo/qt/editor/propertylistwidget.h

SOURCES += \
    consolewidget.cpp \
    connectiongraphicsitem.cpp \
    networkeditor.cpp \
    networkeditorview.cpp \
    processorlistwidget.cpp \
    processorgraphicsitem.cpp \
    propertylistwidget.cpp
        
HEADERS = $$unique(HEADERS)
SOURCES = $$unique(SOURCES)
#                                         #
###########################################

###########################################
# qtwidgets qmake project configuration   #
###########################################
TARGET = inviwo-qtwidgets
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
    ../../../include/inviwo/qt/widgets/canvasqt.h \
    ../../../include/inviwo/qt/widgets/inviwoapplicationqt.h \
    ../../../include/inviwo/qt/widgets/processors/canvasprocessorwidget.h \
    ../../../include/inviwo/qt/widgets/processors/processorwidgetqt.h \
    ../../../include/inviwo/qt/widgets/properties/camerapropertywidgetqt.h \
    ../../../include/inviwo/qt/widgets/properties/filepropertywidgetqt.h \
    ../../../include/inviwo/qt/widgets/properties/floatpropertywidgetqt.h \
    ../../../include/inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h \
    ../../../include/inviwo/qt/widgets/properties/intpropertywidgetqt.h \
    ../../../include/inviwo/qt/widgets/properties/propertywidgetfactoryqt.h \
    ../../../include/inviwo/qt/widgets/properties/propertywidgetqt.h

SOURCES += \
    canvasqt.cpp \
    inviwoapplicationqt.cpp \
    processors/canvasprocessorwidget.cpp \
    processors/processorwidgetqt.cpp \
    properties/camerapropertywidgetqt.cpp \
    properties/filepropertywidgetqt.cpp \
    properties/floatpropertywidgetqt.cpp \
    properties/floatvec3propertywidgetqt.cpp \
    properties/intpropertywidgetqt.cpp \
    properties/propertywidgetfactoryqt.cpp \
    properties/propertywidgetqt.cpp
        
HEADERS = $$unique(HEADERS)
SOURCES = $$unique(SOURCES)
#                                         #
###########################################

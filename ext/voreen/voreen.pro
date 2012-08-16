###########################################
# core qmake project configuration        #
###########################################
TARGET = voreen
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
    CONFIG -= unicode

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
# Add source and header files             #
###########################################
SOURCES += \
    serialization/networkserializer.cpp \
    serialization/xmldeserializer.cpp \
    serialization/xmlserializationconstants.cpp \
    serialization/xmlserializer.cpp \
    serialization/xmlserializerbase.cpp \
    serialization/meta/aggregationmetadata.cpp \
    serialization/meta/metadatacontainer.cpp \
    serialization/meta/metadatafactory.cpp \
    serialization/meta/positionmetadata.cpp \
    serialization/meta/windowstatemetadata.cpp \
    serialization/meta/zoommetadata.cpp
    
HEADERS += \
    serialization/abstractserializable.h \
    serialization/networkserializer.h \
    serialization/serializable.h \
    serialization/serializablefactory.h \
    serialization/serialization.h \
    serialization/serializationexceptions.h \
    serialization/xmldeserializer.h \
    serialization/xmlprocessor.h \
    serialization/xmlserializationconstants.h \
    serialization/xmlserializationconstants.h \
    serialization/xmlserializer.h \
    serialization/xmlserializerbase.h \
    serialization/meta/aggregationmetadata.h \
    serialization/meta/metadatabase.h \
    serialization/meta/metadatacontainer.h \
    serialization/meta/metadatafactory.h \
    serialization/meta/positionmetadata.h \
    serialization/meta/primitivemetadata.h \
    serialization/meta/selectionmetadata.h \
    serialization/meta/windowstatemetadata.h \
    serialization/meta/zoommetadata.h

#
# tgt files    
#    
HEADERS += \
    ext/tgt/matrix.h \
    ext/tgt/quaternion.h \
    ext/tgt/tgt_math.h \
    ext/tgt/types.h \
    ext/tgt/vector.h
    
#
# tinyxml files    
#
SOURCES += \
    ext/tinyxml/tinystr.cpp \
    ext/tinyxml/tinyxml.cpp \
    ext/tinyxml/tinyxmlerror.cpp \
    ext/tinyxml/tinyxmlparser.cpp
    
HEADERS += \
    ext/tinyxml/tinystr.h \
    ext/tinyxml/tinyxml.h
    
HEADERS = $$unique(HEADERS)
SOURCES = $$unique(SOURCES)
#                                         #
###########################################

###########################################
# core qmake project configuration        #
###########################################
TARGET = inviwo-core
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



###########################################
# Prepare module registration             #
###########################################
for(i, IVW_MODULE_LIST) : include($${IVW_MODULE_PATH}/$${i}/$${i}_core.pri)

MODULE_REGISTRATION_HEADER = "$${IVW_MODULE_PATH}/moduleregistration.h"
message("Creating 'modules/moduleregistration.h'...")

HEADER_CONTENT += "$${LITERAL_HASH}include \"inviwo/core/inviwoapplication.h\""
for(i, IVW_MODULE_CLASS_HEADERS) {
    HEADER_CONTENT += "$${LITERAL_HASH}include \"modules/$${i}\""
}

HEADER_CONTENT += "namespace inviwo \{"
HEADER_CONTENT += "void registerAllModules(InviwoApplication* app) \{"
for(i, IVW_MODULE_CLASSES) {
    HEADER_CONTENT += "    (*app).registerModule(new $${i}());"
}
HEADER_CONTENT += "}}"

win32 {
    system(echo "// Automatically generated file do not change!" > \"$${MODULE_REGISTRATION_HEADER}\")
    for(i, HEADER_CONTENT) : system(echo $${i}  >> \"$${MODULE_REGISTRATION_HEADER}\")
} else:unix {
    system(echo "\"// Automatically generated file do not change!\"" > \"$${MODULE_REGISTRATION_HEADER}\")
    for(i, HEADER_CONTENT) : system(echo \'$${i}\'  >> \"$${MODULE_REGISTRATION_HEADER}\")
} else {
    error("ERROR: Unknown platform could not generate module registration header $${IVW_MODULE_PATH}/$${MODULE_REGISTRATION_HEADER}.")
}

exists($${MODULE_REGISTRATION_HEADER}) {
    HEADERS += "$${IVW_MODULE_PATH}/moduleregistration.h"
} else {
    error("ERROR: Module registration header $${IVW_MODULE_PATH}/$${MODULE_REGISTRATION_HEADER} not found.")
}

for(i, IVW_MODULE_CLASS_HEADERS) : HEADERS += $${IVW_MODULE_PATH}/$${i}
for(i, IVW_MODULE_CLASS_SOURCES) : SOURCES += $${IVW_MODULE_PATH}/$${i}
#                                         #
###########################################



###########################################
# Add header and source files             #
###########################################
HEADERS += \
    ../../include/inviwo/core/inviwo.h \
    ../../include/inviwo/core/inviwoapplication.h \
    ../../include/inviwo/core/inviwocore.h \
    ../../include/inviwo/core/inviwomodule.h \
    ../../include/inviwo/core/datastructures/data.h \
    ../../include/inviwo/core/datastructures/datarepresentation.h \
    ../../include/inviwo/core/datastructures/image.h \
    ../../include/inviwo/core/datastructures/imagedisk.h \
    ../../include/inviwo/core/datastructures/imageram.h \
    ../../include/inviwo/core/datastructures/imagerepresentation.h \
    ../../include/inviwo/core/datastructures/representationconverter.h \
    ../../include/inviwo/core/datastructures/representationconverterfactory.h \
    ../../include/inviwo/core/datastructures/volume.h \
    ../../include/inviwo/core/datastructures/volumedisk.h \
    ../../include/inviwo/core/datastructures/volumeram.h \
    ../../include/inviwo/core/datastructures/volumerepresentation.h \
    ../../include/inviwo/core/interaction/interactionhandler.h \
    ../../include/inviwo/core/interaction/trackball.h \
    ../../include/inviwo/core/interaction/events/event.h \
    ../../include/inviwo/core/interaction/events/eventhandler.h \
    ../../include/inviwo/core/interaction/events/eventlistener.h \
    ../../include/inviwo/core/interaction/events/keyboardevent.h \
    ../../include/inviwo/core/interaction/events/mouseevent.h \
    ../../include/inviwo/core/interaction/events/timerevent.h \
    ../../include/inviwo/core/network/portconnection.h \
    ../../include/inviwo/core/network/processornetwork.h \
    ../../include/inviwo/core/network/processornetworkevaluator.h \
    ../../include/inviwo/core/ports/dataport.h \
    ../../include/inviwo/core/ports/port.h \
    ../../include/inviwo/core/ports/imageport.h \
    ../../include/inviwo/core/ports/volumeport.h \
    ../../include/inviwo/core/processors/canvasprocessor.h \
    ../../include/inviwo/core/processors/processor.h \
    ../../include/inviwo/core/processors/processorfactory.h \
    ../../include/inviwo/core/processors/processorwidget.h \
    ../../include/inviwo/core/processors/datasource/datasourceprocessor.h \
    ../../include/inviwo/core/processors/datasource/volumesource.h \
    ../../include/inviwo/core/properties/cameraproperty.h \
    ../../include/inviwo/core/properties/compositeproperty.h \
    ../../include/inviwo/core/properties/fileproperty.h \
    ../../include/inviwo/core/properties/ordinalproperty.h \
    ../../include/inviwo/core/properties/properties.h \
    ../../include/inviwo/core/properties/property.h \
    ../../include/inviwo/core/properties/propertyowner.h \
    ../../include/inviwo/core/properties/scalarproperties.h \
    ../../include/inviwo/core/properties/templateproperty.h \
    ../../include/inviwo/core/properties/vectorproperties.h \
    ../../include/inviwo/core/io/datareader.h \
    ../../include/inviwo/core/io/datawriter.h \
    ../../include/inviwo/core/util/canvas.h \
    ../../include/inviwo/core/util/assertion.h \
    ../../include/inviwo/core/util/exception.h \
    ../../include/inviwo/core/util/logger.h \
    ../../include/inviwo/core/util/project.h

SOURCES += \
    inviwoapplication.cpp \
    inviwocore.cpp \
    inviwomodule.cpp \
    datastructures/data.cpp \
    datastructures/datarepresentation.cpp \
    datastructures/image.cpp \
    datastructures/imagedisk.cpp \
    datastructures/imageram.cpp \
    datastructures/imagerepresentation.cpp \
    datastructures/representationconverter.cpp \
    datastructures/representationconverterfactory.cpp \
    datastructures/volume.cpp \
    datastructures/volumedisk.cpp \
    datastructures/volumeram.cpp \
    datastructures/volumerepresentation.cpp \
    interaction/interactionhandler.cpp \
    interaction/trackball.cpp \
    interaction/events/event.cpp \
    interaction/events/eventhandler.cpp \
    interaction/events/eventlistener.cpp \
    interaction/events/keyboardevent.cpp \
    interaction/events/mouseevent.cpp \
    interaction/events/timerevent.cpp \
    network/portconnection.cpp \
    network/processornetwork.cpp \
    network/processornetworkevaluator.cpp \
    ports/port.cpp \
    ports/imageport.cpp \
    ports/volumeport.cpp \
    processors/canvasprocessor.cpp \
    processors/processor.cpp \
    processors/processorfactory.cpp \
    processors/processorwidget.cpp \
    processors/datasource/datasourceprocessor.cpp \
    processors/datasource/volumesource.cpp \
    properties/cameraproperty.cpp \
    properties/compositeproperty.cpp \
    properties/fileproperty.cpp \
    properties/property.cpp \
    properties/propertyowner.cpp \
    properties/scalarproperties.cpp \
    properties/vectorproperties.cpp \
    io/datareader.cpp \
    io/datawriter.cpp \
    util/canvas.cpp \
    util/assertion.cpp \
    util/exception.cpp \
    util/logger.cpp \
    util/project.cpp
        
HEADERS = $$unique(HEADERS)
SOURCES = $$unique(SOURCES)
#                                         #
###########################################

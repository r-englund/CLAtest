INCLUDEPATH += $${IVW_ROOT}/modules/opengl/ext/glew/include

# TODO: check if 32 or 64 bit lib is used
DEFINES += GLEW_STATIC
LIBS += glew32s.lib
QMAKE_LIBDIR += $${IVW_ROOT}/modules/opengl/ext/glew/lib/win32/debug

SOURCES += \
    $${IVW_MODULE_PATH}/opengl/canvasgl.cpp \
    $${IVW_MODULE_PATH}/opengl/canvasprocessorgl.cpp \
    $${IVW_MODULE_PATH}/opengl/imagegl.cpp \
    $${IVW_MODULE_PATH}/opengl/imageglconverter.cpp \
    $${IVW_MODULE_PATH}/opengl/inviwoopengl.cpp \
    $${IVW_MODULE_PATH}/opengl/processorgl.cpp \
    $${IVW_MODULE_PATH}/opengl/volumegl.cpp \
    $${IVW_MODULE_PATH}/opengl/glwrap/framebufferobject.cpp \
    $${IVW_MODULE_PATH}/opengl/glwrap/shader.cpp \
    $${IVW_MODULE_PATH}/opengl/glwrap/shaderobject.cpp \
    $${IVW_MODULE_PATH}/opengl/glwrap/texture2d.cpp \
    $${IVW_MODULE_PATH}/opengl/glwrap/texture3d.cpp

HEADERS += \
    $${IVW_MODULE_PATH}/opengl/canvasgl.h \
    $${IVW_MODULE_PATH}/opengl/canvasprocessorgl.h \
    $${IVW_MODULE_PATH}/opengl/imagegl.h \
    $${IVW_MODULE_PATH}/opengl/imageglconverter.h \
    $${IVW_MODULE_PATH}/opengl/inviwoopengl.h \
    $${IVW_MODULE_PATH}/opengl/processorgl.h \
    $${IVW_MODULE_PATH}/opengl/volumegl.h \
    $${IVW_MODULE_PATH}/opengl/glwrap/framebufferobject.h \
    $${IVW_MODULE_PATH}/opengl/glwrap/shader.h \
    $${IVW_MODULE_PATH}/opengl/glwrap/shaderobject.h \
    $${IVW_MODULE_PATH}/opengl/glwrap/texture2d.h \
    $${IVW_MODULE_PATH}/opengl/glwrap/texture3d.h


SOURCES += $${IVW_MODULE_PATH}/opengl/ext/tgaload/tgaload.cpp
HEADERS += $${IVW_MODULE_PATH}/opengl/ext/tgaload/tgaload.h

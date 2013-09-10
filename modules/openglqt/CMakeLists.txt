if(DESIRED_QT_VERSION MATCHES 5)
        string(REGEX REPLACE "bin\\\\qmake([^ ]*)$" "lib\\\\cmake" QT5_PATH "${QT_QMAKE_EXECUTABLE}")
        set(CMAKE_PREFIX_PATH "${QT5_PATH}")
        find_package(Qt5OpenGL QUIET REQUIRED)
else()
        set(QT_USE_QTOPENGL TRUE)
endif()

#--------------------------------------------------------------------
# Inviwo OpenglQt Module
ivw_module(OpenglQt)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/openglqt/canvasqt.h
    ${IVW_MODULE_DIR}/openglqt/openglqtinfo.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmoduledefine.h
    ${IVW_MODULE_DIR}/openglqt/processors/canvasprocessorwidgetqt.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/openglqt/canvasqt.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtinfo.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.cpp
    ${IVW_MODULE_DIR}/openglqt/processors/canvasprocessorwidgetqt.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

if(DESIRED_QT_VERSION MATCHES 5)
    qt5_use_modules(inviwo-module-openglqt Core Widgets Gui OpenGL)
endif()

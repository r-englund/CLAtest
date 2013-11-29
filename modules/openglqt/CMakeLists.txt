if(DESIRED_QT_VERSION MATCHES 5)
        string(REGEX REPLACE "bin\\\\qmake([^ ]*)$" "lib\\\\cmake" QT5_PATH "${QT_QMAKE_EXECUTABLE}")
        set(CMAKE_PREFIX_PATH "${QT5_PATH}")
else()
        set(QT_USE_QTOPENGL TRUE)
endif()

#--------------------------------------------------------------------
# Inviwo OpenglQt Module
ivw_module(OpenglQt)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_MODULE_DIR}/openglqt/canvasqt.h
    ${IVW_MODULE_DIR}/openglqt/processors/canvasprocessorwidgetqt.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/openglqt/openglqtcapabilities.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmoduledefine.h
    ${MOC_FILES}
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/openglqt/canvasqt.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtcapabilities.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.cpp
    ${IVW_MODULE_DIR}/openglqt/processors/canvasprocessorwidgetqt.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

ivw_define_qt_definitions()
ivw_add_definition(QT_OPENGL_LIB)

# Create module
ivw_create_module(${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
